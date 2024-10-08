#pragma once

#include <l4/re/env>
#include "mem.h"
#include "queue.h"
#include "cmd.h"
#include "uar.h"

namespace Driver {

cu32 INIT_TIMEOUT_MS = 5000;
cu32 IMB_MAX_PAGE_PAYLOAD = 2 + CMD::IMB_MAX_DATA;

#pragma pack(4)
struct Init_Seg {
    reg32 fw_rev;
    reg32 cmdif_rev_fw_sub;
    reg32 rsvd0[2];
    reg32 cmdq_addr_msb;
    reg32 cmdq_addr_lsb_info;
    reg32 dbv;
    reg32 rsvd1[120];
    reg32 initializing;
};
#pragma pack()

struct HCA_CAP {};

/* main context for all operations on the HCA */
struct MLX5_Context : CMD::CMD_Args {
    Init_Seg* init_seg;
    HCA_CAP hca_cap;
    UAR::UAR_Page_Pool uar_page_pool;
    MEM::MEM_Page_Pool mem_page_pool;
    Q::Queue_Obj_Pool event_queue_pool;
    Q::Queue_Obj_Pool compl_queue_pool;
    Q::Queue_Obj_Pool work_queue_pool;
};

/* Page Request Handler Options */
struct PRH_OPT {
    MLX5_Context* ctx;
    l4_uint32_t eq;
    volatile bool active;
};

void debug_cmd(MLX5_Context& ctx, l4_uint32_t slot);

void init_wait(reg32* initializing);

l4_uint32_t get_issi_support(MLX5_Context& ctx, l4_uint32_t slot);

l4_int32_t get_number_of_pages(MLX5_Context& ctx, l4_uint32_t slot);

void set_driver_version(MLX5_Context& ctx);

l4_uint32_t configure_issi(MLX5_Context& ctx);

bool configure_hca_cap(MLX5_Context& ctx);

void provide_pages(MLX5_Context& ctx, l4_uint32_t init_page_count);

l4_int32_t provide_boot_pages(MLX5_Context& ctx);

l4_int32_t provide_init_pages(MLX5_Context& ctx);

l4_uint32_t reclaim_pages(MLX5_Context& ctx, l4_int32_t page_count);

l4_uint32_t reclaim_all_pages(MLX5_Context& ctx);

void init_queue_obj_pool(MLX5_Context& ctx);

void init_hca(MLX5_Context& ctx);

void teardown_hca(MLX5_Context& ctx);

void alloc_uar(MLX5_Context& ctx, l4_uint8_t* bar0);

void dealloc_uar(MLX5_Context& ctx);

void* page_request_handler(void* arg);

pthread_t setup_event_queue(MLX5_Context& ctx, l4_uint64_t icu_src, reg32* msix_table, L4::Cap<L4::Icu>& icu, PRH_OPT& opt);

l4_uint32_t alloc_pd(MLX5_Context& ctx);

void dealloc_pd(MLX5_Context& ctx, l4_uint32_t pd);

void setup_work_queue(MLX5_Context& ctx);

}
