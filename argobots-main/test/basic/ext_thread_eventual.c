/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 * See COPYRIGHT in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "abt.h"
#include "abttest.h"

/* This test checks if ABT_eventual works with external threads or not.  This
 * test specifically focuses on whether ABT_eventual that internally uses
 * pthread_cond_t or futex works even if it spuriously wakes up because of
 * signals. */

#define DEFAULT_NUM_TOTAL_THREADS 4
#define DEFAULT_NUM_XSTREAMS 2
#define DEFAULT_NUM_ITER 500

typedef struct {
    ABT_eventual eventual;
    int counter;
    ABT_bool is_dynamic;
} eventual_set;
eventual_set g_eventual_sets[2];
int g_iter = DEFAULT_NUM_ITER;
ABT_barrier g_barrier;

#define NUM_EVENTUAL_SETS                                                      \
    ((int)(sizeof(g_eventual_sets) / sizeof(g_eventual_sets[0])))

typedef struct {
    int tid;
    int num_total_threads;
} thread_arg_t;

void thread_func(void *arg)
{
    int i;
    thread_arg_t *p_arg = (thread_arg_t *)arg;
    for (i = 0; i < g_iter; i++) {
        int j;
        for (j = 0; j < NUM_EVENTUAL_SETS; j++) {
            if (p_arg->tid == g_iter % p_arg->num_total_threads) {
                if (i == 0) {
                    g_eventual_sets[j].counter = 0;
                } else {
                    assert(g_eventual_sets[j].counter == i);
                }
                g_eventual_sets[j].counter += 1;
                ABT_eventual_set(g_eventual_sets[j].eventual, NULL, 0);
            } else {
                ABT_eventual_wait(g_eventual_sets[j].eventual, NULL);
            }
            assert(g_eventual_sets[j].counter == i + 1);
            ABT_barrier_wait(g_barrier);
            if (p_arg->tid == g_iter % p_arg->num_total_threads) {
                ABT_eventual_reset(g_eventual_sets[j].eventual);
            }
            ABT_barrier_wait(g_barrier);
        }
    }
}

void *pthread_func(void *arg)
{
    thread_func(arg);
    return NULL;
}

int main(int argc, char *argv[])
{
    int i, kind, ret;
    int num_total_threads = DEFAULT_NUM_TOTAL_THREADS;
    int num_xstreams = DEFAULT_NUM_XSTREAMS;
    ABT_eventual_memory eventual_mem = ABT_EVENTUAL_INITIALIZER;

    /* Read arguments. */
    ATS_read_args(argc, argv);
    if (argc >= 2) {
        num_total_threads = ATS_get_arg_val(ATS_ARG_N_ULT);
        num_xstreams = ATS_get_arg_val(ATS_ARG_N_ES);
        g_iter = ATS_get_arg_val(ATS_ARG_N_ITER);
    }
    assert(num_total_threads >= 1);
    assert(num_xstreams >= 1);

#ifndef ABT_ENABLE_VER_20_API
    ret = ABT_init(0, NULL);
    ATS_ERROR(ret, "ABT_init");
#endif
    ABT_bool support_external_thread;
    ret = ABT_info_query_config(ABT_INFO_QUERY_KIND_ENABLED_EXTERNAL_THREAD,
                                (void *)&support_external_thread);
    ATS_ERROR(ret, "ABT_info_query_config");
#ifndef ABT_ENABLE_VER_20_API
    ret = ABT_finalize();
    ATS_ERROR(ret, "ABT_finalize");
#endif
    if (!support_external_thread) {
        ATS_ERROR(ABT_ERR_FEATURE_NA, "ABT_info_query_config");
    }

    /* Initialize */
    ATS_init(argc, argv, num_xstreams);

    ATS_printf(2, "# of ESs : %d\n", num_xstreams);
    ATS_printf(1, "# of ULTs: %d\n", num_total_threads);
    ATS_printf(1, "# of iter: %d\n", g_iter);

    /* Set up eventual and g_barrier. */
    ret = ABT_eventual_create(0, &g_eventual_sets[0].eventual);
    ATS_ERROR(ret, "ABT_eventual_create");
    g_eventual_sets[0].is_dynamic = ABT_TRUE;
    g_eventual_sets[1].eventual = ABT_EVENTUAL_MEMORY_GET_HANDLE(&eventual_mem);
    g_eventual_sets[1].is_dynamic = ABT_FALSE;

    ret = ABT_barrier_create(num_total_threads, &g_barrier);
    ATS_ERROR(ret, "ABT_barrier_create");

    ABT_xstream *xstreams =
        (ABT_xstream *)malloc(sizeof(ABT_xstream) * num_xstreams);
    ABT_thread *threads =
        (ABT_thread *)malloc(sizeof(ABT_thread) * num_total_threads);
    pthread_t *pthreads =
        (pthread_t *)malloc(sizeof(pthread_t) * num_total_threads);
    thread_arg_t *thread_args =
        (thread_arg_t *)malloc(sizeof(thread_arg_t) * num_total_threads);
    ABT_pool *pools;
    pools = (ABT_pool *)malloc(sizeof(ABT_pool) * num_xstreams);

    for (i = 0; i < num_total_threads; i++) {
        thread_args[i].tid = i;
        thread_args[i].num_total_threads = num_total_threads;
    }

    /* Create Execution Streams */
    ret = ABT_xstream_self(&xstreams[0]);
    ATS_ERROR(ret, "ABT_xstream_self");
    for (i = 1; i < num_xstreams; i++) {
        ret = ABT_xstream_create(ABT_SCHED_NULL, &xstreams[i]);
        ATS_ERROR(ret, "ABT_xstream_create");
    }

    /* Get the pools attached to an execution stream */
    for (i = 0; i < num_xstreams; i++) {
        ret = ABT_xstream_get_main_pools(xstreams[i], 1, pools + i);
        ATS_ERROR(ret, "ABT_xstream_get_main_pools");
    }

    for (kind = 0; kind < ATS_TIMER_KIND_LAST_; kind++) {
        ATS_create_timer((ATS_timer_kind)kind);
        /* Create ULTs */
        for (i = 0; i < num_total_threads / 2; i++) {
            ret = ABT_thread_create(pools[i % num_xstreams], thread_func,
                                    &thread_args[i], ABT_THREAD_ATTR_NULL,
                                    &threads[i]);
            ATS_ERROR(ret, "ABT_thread_create");
        }
        /* Create Pthreads too. */
        for (i = num_total_threads / 2; i < num_total_threads; i++) {
            ret = pthread_create(&pthreads[i], NULL, pthread_func,
                                 &thread_args[i]);
            assert(ret == 0);
        }
        /* Join and free ULTs */
        for (i = 0; i < num_total_threads / 2; i++) {
            ret = ABT_thread_free(&threads[i]);
            ATS_ERROR(ret, "ABT_thread_free");
        }
        /* Join Pthreads too. */
        for (i = num_total_threads / 2; i < num_total_threads; i++) {
            ret = pthread_join(pthreads[i], NULL);
            assert(ret == 0);
        }
        ATS_destroy_timer();
    }

    /* Join Execution Streams */
    for (i = 1; i < num_xstreams; i++) {
        ret = ABT_xstream_join(xstreams[i]);
        ATS_ERROR(ret, "ABT_xstream_join");
    }

    /* Free Execution Streams */
    for (i = 1; i < num_xstreams; i++) {
        ret = ABT_xstream_free(&xstreams[i]);
        ATS_ERROR(ret, "ABT_xstream_free");
    }

    /* Free eventual and g_barrier. */
    for (i = 0; i < NUM_EVENTUAL_SETS; i++) {
        if (g_eventual_sets[i].is_dynamic) {
            ret = ABT_eventual_free(&g_eventual_sets[i].eventual);
            ATS_ERROR(ret, "ABT_eventual_free");
        }
    }
    ret = ABT_barrier_free(&g_barrier);
    ATS_ERROR(ret, "ABT_barrier_free");

    /* Finalize */
    ret = ATS_finalize(0);

    free(xstreams);
    free(threads);
    free(pthreads);
    free(thread_args);
    free(pools);

    return ret;
}
