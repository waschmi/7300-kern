/*
 * VDIN vframe support
 *
 * Author: Bobby Yang <bo.yang@amlogic.com>
 *
 * Copyright (C) 2010 Amlogic Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */


#ifndef __VDIN_VF_H
#define __VDIN_VF_H

/* Standard Linux Headers */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/list.h>

/* Amlogic Linux Headers */
#include <linux/amports/vframe.h>

#define VF_LOG_EN

#ifdef VF_LOG_EN

#define VF_LOG_LEN       200
/* only log frontend opertations */
#define VF_LOG_FE
/* only log backend opertations */
#define VF_LOG_BE

typedef enum vf_operation_e {
    VF_OPERATION_INIT = 0,
    VF_OPERATION_FPEEK,
    VF_OPERATION_FGET,
    VF_OPERATION_FPUT,
    VF_OPERATION_BPEEK,
    VF_OPERATION_BGET,
    VF_OPERATION_BPUT,
    VF_OPERATION_FREE,
} vf_operation_t;

typedef enum vf_status_e {
    VF_STATUS_WL = 0, // In write list
    VF_STATUS_WM,     // In write mode
    VF_STATUS_RL,     // In read  list
    VF_STATUS_RM,     // In read  mode
    VF_STATUS_WT,     // In wait  list
    VF_STATUS_SL,
} vf_status_t;

typedef struct vf_log_s {
    /*
     * master
     */
    // [ 0][  n] 1: buf[n] is in write list
    // [ 1][  n] 1: buf[n] is in write mode
    // [ 2][  n] 1: buf[n] is in read  list
    // [ 3][  n] 1: buf[n] is in read  mode
    // [ 4][  n] 1: buf[n] is in wait  list
    /*
     * slave
     */
    // [ 5][  n] 1: buf[n] is in write list
    // [ 6][  n] 1: buf[n] is in write mode
    // [ 7][  n] 1: buf[n] is in read  list
    // [ 8][  n] 1: buf[n] is in read  mode
    // [ 9][  n] 1: buf[n] is in wait  list
    // [10][  7] 1: operation failure
    //     [6:3]    reserved
    //     [2:0]    operation ID
    unsigned char  log_buf[VF_LOG_LEN][11];
    unsigned int   log_cur;
    struct timeval log_time[VF_LOG_LEN];
} vf_log_t;

#endif

#define VF_FLAG_NORMAL_FRAME         0x00000001


typedef struct vf_entry {
    struct vframe_s vf;
    enum vf_status_e status;
    struct list_head list;
    unsigned int flag;
} vf_entry_t;

typedef struct vf_pool {
    unsigned int max_size, size;
    struct vf_entry *master;
    struct vf_entry *slave;
    struct list_head wr_list; /* vf_entry */
    unsigned int     wr_list_size;
    struct list_head *wr_next;
    struct list_head rd_list; /* vf_entry */
    unsigned int     rd_list_size;
    struct list_head wt_list; /* vframe_s */
    spinlock_t lock;
#ifdef VF_LOG_EN
    struct vf_log_s log;
#endif
} vf_pool_t;

extern void vf_log_init(struct vf_pool *p);
extern void vf_log_print(struct vf_pool *p);

extern struct vf_entry *vf_get_master(struct vf_pool *p, int index);
extern struct vf_entry *vf_get_slave(struct vf_pool *p, int index);

extern struct vf_pool *vf_pool_alloc(int size);
extern int vf_pool_init(struct vf_pool *p, int size);
extern void vf_pool_free(struct vf_pool *p);

extern struct vf_entry *provider_vf_peek(struct vf_pool *p);
extern struct vf_entry *provider_vf_get(struct vf_pool *p);
extern void provider_vf_put(struct vf_entry *vf, struct vf_pool *p);

extern struct vf_entry *receiver_vf_peek(struct vf_pool *p);
extern struct vf_entry *receiver_vf_get(struct vf_pool *p);
extern void receiver_vf_put(struct vframe_s *vf, struct vf_pool *p);

extern struct vframe_s *vdin_vf_peek(void* op_arg);
extern struct vframe_s *vdin_vf_get (void* op_arg);
extern void vdin_vf_put(struct vframe_s *vf, void* op_arg);

#endif

