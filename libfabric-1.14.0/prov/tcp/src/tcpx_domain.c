/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "tcpx.h"

extern struct fi_ops_msg tcpx_srx_msg_ops;
extern struct fi_ops_tagged tcpx_srx_tag_ops;


static int tcpx_srx_ctx_close(struct fid *fid)
{
	struct tcpx_rx_ctx *srx_ctx;
	struct slist_entry *entry;
	struct tcpx_xfer_entry *xfer_entry;

	srx_ctx = container_of(fid, struct tcpx_rx_ctx,
			       rx_fid.fid);

	while (!slist_empty(&srx_ctx->rx_queue)) {
		entry = slist_remove_head(&srx_ctx->rx_queue);
		xfer_entry = container_of(entry, struct tcpx_xfer_entry, entry);
		ofi_buf_free(xfer_entry);
	}

	while (!slist_empty(&srx_ctx->tag_queue)) {
		entry = slist_remove_head(&srx_ctx->tag_queue);
		xfer_entry = container_of(entry, struct tcpx_xfer_entry, entry);
		ofi_buf_free(xfer_entry);
	}

	ofi_bufpool_destroy(srx_ctx->buf_pool);
	fastlock_destroy(&srx_ctx->lock);
	free(srx_ctx);
	return FI_SUCCESS;
}

static struct fi_ops fi_ops_srx_ctx = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_srx_ctx_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int tcpx_srx_ctx(struct fid_domain *domain, struct fi_rx_attr *attr,
			struct fid_ep **rx_ep, void *context)
{
	struct tcpx_rx_ctx *srx_ctx;
	int ret = FI_SUCCESS;

	srx_ctx = calloc(1, sizeof(*srx_ctx));
	if (!srx_ctx)
		return -FI_ENOMEM;

	srx_ctx->rx_fid.fid.fclass = FI_CLASS_SRX_CTX;
	srx_ctx->rx_fid.fid.context = context;
	srx_ctx->rx_fid.fid.ops = &fi_ops_srx_ctx;

	srx_ctx->rx_fid.msg = &tcpx_srx_msg_ops;
	srx_ctx->rx_fid.tagged = &tcpx_srx_tag_ops;
	slist_init(&srx_ctx->rx_queue);
	slist_init(&srx_ctx->tag_queue);

	ret = fastlock_init(&srx_ctx->lock);
	if (ret)
		goto err1;

	ret = ofi_bufpool_create(&srx_ctx->buf_pool,
				 sizeof(struct tcpx_xfer_entry),
				 16, attr->size, 1024, 0);
	if (ret)
		goto err2;

	srx_ctx->match_tag_rx = (attr->caps & FI_DIRECTED_RECV) ?
				tcpx_match_tag_addr : tcpx_match_tag;
	srx_ctx->op_flags = attr->op_flags;
	*rx_ep = &srx_ctx->rx_fid;
	return FI_SUCCESS;
err2:
	fastlock_destroy(&srx_ctx->lock);
err1:
	free(srx_ctx);
	return ret;
}

static struct fi_ops_domain tcpx_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = ofi_ip_av_create,
	.cq_open = tcpx_cq_open,
	.endpoint = tcpx_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = fi_no_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = tcpx_srx_ctx,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

static int tcpx_set_ops(struct fid *fid, const char *name,
			uint64_t flags, void *ops, void *context)
{
	struct tcpx_domain *domain;

	domain = container_of(fid, struct tcpx_domain,
			      util_domain.domain_fid.fid);
	if (flags)
		return -FI_EBADFLAGS;

	if (!strcasecmp(name, OFI_OPS_DYNAMIC_RBUF)) {
		domain->dynamic_rbuf = ops;
		if (domain->dynamic_rbuf->size != sizeof(*domain->dynamic_rbuf)) {
			domain->dynamic_rbuf = NULL;
			return -FI_ENOSYS;
		}

		return 0;
	}

	return -FI_ENOSYS;
}

static int tcpx_domain_close(fid_t fid)
{
	struct tcpx_domain *domain;
	int ret;

	domain = container_of(fid, struct tcpx_domain,
			      util_domain.domain_fid.fid);

	ret = ofi_domain_close(&domain->util_domain);
	if (ret)
		return ret;

	free(domain);
	return FI_SUCCESS;
}

static struct fi_ops tcpx_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_domain_close,
	.bind = ofi_domain_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
	.tostr = NULL,
	.ops_set = tcpx_set_ops,
};

static struct fi_ops_mr tcpx_domain_fi_ops_mr = {
	.size = sizeof(struct fi_ops_mr),
	.reg = ofi_mr_reg,
	.regv = ofi_mr_regv,
	.regattr = ofi_mr_regattr,
};

int tcpx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		     struct fid_domain **domain_fid, void *context)
{
	struct tcpx_domain *domain;
	int ret;

	ret = ofi_prov_check_info(&tcpx_util_prov, fabric->api_version, info);
	if (ret)
		return ret;

	domain = calloc(1, sizeof(*domain));
	if (!domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(fabric, info, &domain->util_domain, context);
	if (ret)
		goto err;

	domain->util_domain.domain_fid.fid.ops = &tcpx_domain_fi_ops;
	domain->util_domain.domain_fid.ops = &tcpx_domain_ops;
	domain->util_domain.domain_fid.mr = &tcpx_domain_fi_ops_mr;
	*domain_fid = &domain->util_domain.domain_fid;

	return FI_SUCCESS;
err:
	free(domain);
	return ret;
}
