/**
 * (C) Copyright 2016-2021 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */
/**
 * This file is part of daos
 *
 * include/daos/placement.h
 */

#ifndef __DAOS_PLACEMENT_H__
#define __DAOS_PLACEMENT_H__

#include <daos/common.h>
#include <daos/pool_map.h>
#include <daos/object.h>

/** default placement map when none are specified */
// 默认采用jump_hash算法算分布
#define DEFAULT_PL_TYPE PL_TYPE_JUMP_MAP

/** types of placement maps */
typedef enum {
	PL_TYPE_UNKNOWN,
	/** only support ring map for the time being */
	PL_TYPE_RING,
	/**Prototype placement map, jump_hash算法 */
	PL_TYPE_JUMP_MAP,
	/** reserved */
	PL_TYPE_PETALS,
} pl_map_type_t;

struct pl_map_init_attr {
	pl_map_type_t		ia_type;	// 分布算法, 默认使用jump hash: PL_TYPE_JUMP_MAP
	union {
		struct pl_ring_init_attr {
			pool_comp_type_t	domain;
			unsigned int		ring_nr;
		} ia_ring;
		struct pl_jump_map_init_attr {
			pool_comp_type_t	domain;
		} ia_jump_map;
	};
};

struct pl_target {
	uint32_t		pt_pos;
};

/** A group of targets */
struct pl_target_grp {
	/** pool map version to generate this layout */
	uint32_t		 	 tg_ver;
	/** number of targets */
	unsigned int		 tg_target_nr;
	/** array of targets */
	struct pl_target	*tg_targets;
};

struct pl_obj_shard {
	uint32_t	po_shard;	/* shard index */ 	// 分片序号, 从0开始，在obj内是唯一的；
	uint32_t	po_target;	/* target id */		// 这个分片关联的target的id号, target id应该是全局唯一的，所有target的编号是从0开始递增的
	uint32_t	po_fseq;	/* The latest failure sequence */	// 这个target上次被标记down时的poolmap的版本号
	uint32_t	po_rebuilding:1; /* rebuilding status */
};

struct pl_obj_layout {
	uint32_t		 ol_ver;		// 版本信息
	uint32_t		 ol_grp_size;	// group中target的数量(副本数)
	uint32_t		 ol_grp_nr;		// group的数量(类似ceph中的pg数)
	uint32_t		 ol_nr;			// target的总数, ol_grp_size * ol_grp_nr
	struct pl_obj_shard	*ol_shards;	// obj关联的所有分片的信息
};

/** common header of all placement map */
struct pl_map {
	/** correpsonding pool uuid */
	uuid_t			 pl_uuid;
	/** link chain on hash */
	d_list_t		 pl_link;
	/** protect refcount */
	pthread_spinlock_t	 pl_lock;
	/** refcount */
	int			 pl_ref;
	/** pool connections, protected by pl_rwlock */
	int			 pl_connects;
	/** type of placement map */
	pl_map_type_t		 pl_type;
	/** reference to pool map */
	struct pool_map		*pl_poolmap;
	/** placement map operations */
	struct pl_map_ops       *pl_ops;
};

/** attributes of the placement map */
struct pl_map_attr {
	int		pa_type;
	int		pa_domain;
	int		pa_domain_nr;
	int		pa_target_nr;	// 池内target的数量
};

int pl_init(void);
void pl_fini(void);

int pl_map_create(struct pool_map *pool_map, struct pl_map_init_attr *mia,
		  struct pl_map **pl_mapp);
void pl_map_destroy(struct pl_map *map);
void pl_map_print(struct pl_map *map);

struct pl_map *pl_map_find(uuid_t uuid, daos_obj_id_t oid);
int  pl_map_update(uuid_t uuid, struct pool_map *new_map, bool connect,
		pl_map_type_t default_type);
void pl_map_disconnect(uuid_t uuid);
int pl_map_query(uuid_t po_uuid, struct pl_map_attr *attr);
void pl_map_addref(struct pl_map *map);
void pl_map_decref(struct pl_map *map);
uint32_t pl_map_version(struct pl_map *map);

void pl_obj_layout_free(struct pl_obj_layout *layout);
int  pl_obj_layout_alloc(unsigned int grp_size, unsigned int grp_nr,
			 struct pl_obj_layout **layout_pp);
bool pl_obj_layout_contains(struct pool_map *map, struct pl_obj_layout *layout,
			    uint32_t rank, uint32_t target_index,
			    uint32_t shard);

int pl_obj_place(struct pl_map *map,
		 struct daos_obj_md *md,
		 struct daos_obj_shard_md *shard_md,
		 struct pl_obj_layout **layout_pp);

int pl_obj_find_rebuild(struct pl_map *map,
			struct daos_obj_md *md,
			struct daos_obj_shard_md *shard_md,
			uint32_t rebuild_ver, uint32_t *tgt_rank,
			uint32_t *shard_id, unsigned int array_size);

int pl_obj_find_drain(struct pl_map *map, struct daos_obj_md *md,
		      struct daos_obj_shard_md *shard_md,
		      uint32_t rebuild_ver, uint32_t *tgt_rank,
		      uint32_t *shard_id, unsigned int array_size);

int pl_obj_find_reint(struct pl_map *map,
			struct daos_obj_md *md,
			struct daos_obj_shard_md *shard_md,
			uint32_t rebuild_ver, uint32_t *tgt_rank,
			uint32_t *shard_id, unsigned int array_size);

int pl_obj_find_addition(struct pl_map *map,
			 struct daos_obj_md *md,
			struct daos_obj_shard_md *shard_md,
			uint32_t rebuild_ver, uint32_t *tgt_rank,
			uint32_t *shard_id, unsigned int array_size);

typedef struct pl_obj_shard *(*pl_get_shard_t)(void *data, int idx);

static inline struct pl_obj_shard *
pl_obj_get_shard(void *data, int idx)
{
	struct pl_obj_layout	*layout = data;

	return &layout->ol_shards[idx];
}

int pl_select_leader(daos_obj_id_t oid, uint32_t shard_idx, uint32_t grp_size,
		     int *tgt_id, pl_get_shard_t pl_get_shard, void *data);

void obj_layout_dump(daos_obj_id_t oid, struct pl_obj_layout *layout);

#endif /* __DAOS_PLACEMENT_H__ */
