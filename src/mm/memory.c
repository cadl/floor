#include <memory.h>
#include <type.h>
#include <sys.h>


u32int placement_addr = (u32int)KMALLOC_START;

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

void init_frame()
{
    int i;
    memset(frames, 0xff, sizeof(u32int) * FRAMES_NUM);
    for (i=0; i<(u32int)KERNEL_END; i+=0x1000)
    {
        clear_frame(i/0x1000);
    }
}
