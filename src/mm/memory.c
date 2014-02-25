#include <memory.h>
#include <type.h>
#include <sys.h>
#include <string.h>
#include <monitor.h>
#include <asm/system.h>
#include <page.h>
#include <buddy.h>

static u32int placement_addr = (u32int)KERNEL_HEAP_START;
static struct buddy_alloctor *frame_alloctor;

void *kmalloc(u32int size)
{
    void *m_start;
    if (placement_addr & (0xfff))
    {
        placement_addr &= 0xfffff000;
        placement_addr += 0x1000;
    }
    m_start = (void *)placement_addr;
    placement_addr += size;
    return m_start;
}

void *kmalloc_f(u32int size, u32int *frame_idx)
{
    void *m_start;
    if (placement_addr & 0xfff)
    {
        placement_addr &= 0xfffff000;
        placement_addr += 0x1000;
    }
    *frame_idx = placement_addr / 0x1000;
    m_start = (void *)placement_addr;
    placement_addr += size;
    return m_start;
}
void init_frame()
{
    frame_alloctor = buddy_new(TOTAL_MEM - USER_SPACE_START, 0x1000);
    monitor_put_hex(frame_alloctor->size);
    monitor_put_hex(frame_alloctor->block_size);
    monitor_put_hex(frame_alloctor->size/frame_alloctor->block_size);
}

void *frame2pointer(u32int frame_idx)
{
    u32int addr;
    addr = frame_idx * 0x1000;
    return (void *)addr;
}

s32int get_free_frames(u32int n)
{
    s32int idx;
    idx = buddy_malloc(frame_alloctor, n * 0x1000);
    if (idx == -1)
        panic("no free frame\n");
    return idx / 0x1000 + USER_SPACE_START / 0x1000;
}
