#include <memory.h>
#include <type.h>


u32int placement_addr = 0x600000;

void* kmalloc(u32int size)
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

void memset(void *ptr, u8int val, u32int size)
{
    int i;
    for (i=0; i<size; i++)
    {
        *((u8int *)ptr + i) = val;
    }
}

void init_frame()
{
    u32int total_mem = (u32int)TOTAL_MEM;
    nframes = total_mem/0x1000/32;
    frames = (u32int*)kmalloc(sizeof(u32int)*nframes);
    memset(frames, 0, nframes);
}
