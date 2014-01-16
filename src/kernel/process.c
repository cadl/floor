#include <process.h>
#include <page.h>
#include <memory.h>


void init_process()
{
    u32int i;
    page_t *tmp_page;
    current_page_directory = clone_directory(kernel_page_directory);
    for (i=(u32int)USER_SPACE_START; i<(u32int)USER_SPACE_END; i++)
    {
        tmp_page = get_page(i, 1, current_page_directory);
        tmp_page->present = 1;
        tmp_page->user = 0;
        tmp_page->rw = 1;
        tmp_page->frame = i / 0x1000;
        mark_frame(i/0x1000);
    }
    switch_page_directory(current_page_directory);
}
