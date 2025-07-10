/*
* Copyright (c) 2008-2016 Allwinner Technology Co. Ltd.
* All rights reserved.
*
* File : aw_ion.h
* Description :
* History :
*   Author  :
*   Date    : 2016/08/31
*   Comment :
*
*
*/

#ifndef CDC_UTIL_H__
#define CDC_UTIL_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include "cdc_ion_5.4.h"

//no.1 define usr handle type
//note: modify in cedarc_v1.3 maybe problem
#if (defined CONF_KERNEL_VERSION_3_4)
    typedef void* aw_ion_user_handle_t;
    #define ION_USER_HANDLE_INIT_VALUE (NULL)
#else
    typedef int aw_ion_user_handle_t;
    #define ION_USER_HANDLE_INIT_VALUE (-1)
#endif

//no.2 define ion heap type
enum aw_ion_heap_type {
    AW_ION_SYSTEM_HEAP_TYPE,
    AW_ION_SYSTEM_CONTIG_HEAP_TYPE,
    AW_ION_CARVEOUT_HEAP_TYPE,
    AW_ION_TYPE_HEAP_CHUNK,
    AW_ION_TYPE_HEAP_DMA,
    AW_ION_TYPE_HEAP_CUSTOM,
    //allwinner add
    AW_ION_TYPE_HEAP_SECURE,
 
    AW_ION_NUM_HEAPS = 16,/* must be last so device specific heaps always
                              are at the end of this enum */
};

#define AW_ION_SYSTEM_HEAP_MASK           (1 << AW_ION_SYSTEM_HEAP_TYPE)
#define AW_ION_SYSTEM_CONTIG_HEAP_MASK    (1 << AW_ION_SYSTEM_CONTIG_HEAP_TYPE)
#define AW_ION_CARVEOUT_HEAP_MASK         (1 << AW_ION_CARVEOUT_HEAP_TYPE)
#define AW_ION_DMA_HEAP_MASK              (1 << AW_ION_TYPE_HEAP_DMA)
//allwinner add
#define AW_ION_SECURE_HEAP_MASK           (1 << AW_ION_TYPE_HEAP_SECURE)

#if ((defined CONF_KERNEL_VERSION_3_4) ||  \
	 (defined CONF_KERNEL_VERSION_3_10) || \
	 (defined CONF_KERNEL_VERSION_4_9))

#define	AW_SYSTEM_HEAP_MASK     AW_ION_SYSTEM_HEAP_MASK
#define AW_DMA_HEAP_MASK        AW_ION_DMA_HEAP_MASK
#define AW_SYSTEM_CONFIG_MASK   AW_ION_SYSTEM_CONTIG_HEAP_MASK
#define AW_CARVEROUT_HEAP_MASK  AW_ION_CARVEOUT_HEAP_MASK
//allwinner add
#define AW_SECURE_HEAP_MASK     AW_ION_SECURE_HEAP_MASK

#else

#define	AW_SYSTEM_HEAP_MASK     AW_ION_NEW_SYSTEM_HEAP_MASK
#define AW_DMA_HEAP_MASK        AW_ION_NEW_DMA_HEAP_MASK
#define AW_SYSTEM_CONFIG_MASK   0
#define AW_CARVEROUT_HEAP_MASK  0
//allwinner add
#define AW_SECURE_HEAP_MASK     0

#endif

//no.3 define flags
#define AW_ION_CACHED_FLAG 1
/* mappings of this buffer should be cached,
ion will do cache maintenance when the buffer is mapped for dma */
#define AW_ION_CACHED_NEEDS_SYNC_FLAG 2
/* mappings of this buffer will created at mmap time,
if this is set caches must be managed manually */

//no.4 define some struct
typedef struct aw_ion_allocation_data
{
    size_t aw_len;
    size_t aw_align;
    unsigned int aw_heap_id_mask;
    unsigned int flags;
    aw_ion_user_handle_t handle;
} aw_ion_allocation_data_t;

typedef struct aw_dma_heap_allocation_data {
    __u64 len;
    __u32 fd;
    __u32 fd_flags;
    __u64 heap_flags;
}aw_dma_heap_allocation_data_t;

typedef struct aw_ion_handle_data
{
    aw_ion_user_handle_t handle;
} aw_ion_handle_data_t;

typedef struct aw_ion_fd_data
{
    aw_ion_user_handle_t handle;
    int aw_fd;
} aw_ion_fd_data_t;

typedef struct aw_ion_custom_info
{
    unsigned int aw_cmd;
    unsigned long aw_arg;
} aw_ion_custom_data_t;

//allwinner add
typedef struct aw_sunxi_phys_data
{
    aw_ion_user_handle_t handle;
    unsigned int  phys_addr;
    unsigned int  size;
} aw_sunxi_phys_data_t;

//allwinner add
typedef struct aw_cache_range
{
    long long    start;
    long long    end;
} aw_cache_range_t;

typedef struct sunxi_pool_info {
    unsigned int total;     //unit kb
    unsigned int free_kb;  // size kb
    unsigned int free_mb;  // size mb
} sunxi_pool_info_t;

//no.5 define align
#define SZ_64M       0x04000000
#define SZ_4M        0x00400000
#define SZ_1M        0x00100000
#define SZ_64K       0x00010000
#define SZ_4k        0x00001000
#define SZ_1k        0x00000400

//no.6 define ioctl cmd
#define AW_MEM_ION_IOC_MAGIC        'I'
#define AW_MEM_ION_IOC_ALLOC        _IOWR(AW_MEM_ION_IOC_MAGIC, 0, struct aw_ion_allocation_data)
#define AW_MEM_ION_IOC_FREE         _IOWR(AW_MEM_ION_IOC_MAGIC, 1, struct aw_ion_handle_data)
#define AW_MEM_ION_IOC_MAP          _IOWR(AW_MEM_ION_IOC_MAGIC, 2, struct aw_ion_fd_data)
#define AW_MEM_ION_IOC_SHARE        _IOWR(AW_MEM_ION_IOC_MAGIC, 4, struct aw_ion_fd_data)
#define AW_MEM_ION_IOC_IMPORT       _IOWR(AW_MEM_ION_IOC_MAGIC, 5, struct aw_ion_fd_data)
#define AW_MEM_ION_IOC_SYNC         _IOWR(AW_MEM_ION_IOC_MAGIC, 7, struct aw_ion_fd_data)
#define AW_MEM_ION_IOC_CUSTOM       _IOWR(AW_MEM_ION_IOC_MAGIC, 6, struct aw_ion_custom_info)

//allwinner add costom cmd
#define ION_IOC_SUNXI_FLUSH_RANGE           5
#define ION_IOC_SUNXI_FLUSH_ALL             6
#define ION_IOC_SUNXI_PHYS_ADDR             7
#define ION_IOC_SUNXI_DMA_COPY              8
#define ION_IOC_SUNXI_POOL_INFO             10
#define ION_IOC_SUNXI_TEE_ADDR              17

#define AW_DMA_HEAP_IOC_MAGIC      'H'

/**
 * DOC: DMA_HEAP_IOCTL_ALLOC - allocate memory from pool
 *
 * Takes a dma_heap_allocation_data struct and returns it with the fd field
 * populated with the dmabuf handle of the allocation.
 */
#define AW_DMA_HEAP_IOCTL_ALLOC    _IOWR(AW_DMA_HEAP_IOC_MAGIC, 0x0,\
                       struct aw_dma_heap_allocation_data)


/* begin/end dma-buf functions used for userspace mmap. */
struct aw_dma_buf_sync {
    __u64 flags;
};

#define AW_DMA_BUF_SYNC_READ      (1 << 0)
#define AW_DMA_BUF_SYNC_WRITE     (2 << 0)
#define AW_DMA_BUF_SYNC_RW        (AW_DMA_BUF_SYNC_READ | AW_DMA_BUF_SYNC_WRITE)
#define AW_DMA_BUF_SYNC_START     (0 << 2)
#define AW_DMA_BUF_SYNC_END       (1 << 2)
#define AW_DMA_BUF_SYNC_VALID_FLAGS_MASK \
    (AW_DMA_BUF_SYNC_RW | AW_DMA_BUF_SYNC_END)

#define AW_DMA_BUF_NAME_LEN    32

#define AW_DMA_BUF_BASE      'b'
#define AW_DMA_BUF_IOCTL_SYNC    _IOW(AW_DMA_BUF_BASE, 0, struct aw_dma_buf_sync)

#ifdef __cplusplus
}
#endif

#endif
