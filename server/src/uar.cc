#include "uar.h"

void UAR::alloc_block(UAR_Page_Pool* upp) {
    UAR_Page_Block* upb = create_block(upp);

    for (l4_uint64_t i = 0; i < upp->block_size; i++) {
        UAR_Page* up = &upb->start[i];
        up->used = false;
        up->block = upb;
        up->data.uar.index = upp->data.base.index + i;/*TODO fix*/
        up->data.uar.addr = (Page*)((l4_uint8_t*)upp->data.base.addr + (i * HCA_PAGE_SIZE));
    }

    add_block_to_pool(upp, upb);
}

void UAR::free_block(UAR_Page_Pool* upp, UAR_Page_Block* upb) {
    remove_block_from_pool(upp, upb);

    destroy_block(upb);
}