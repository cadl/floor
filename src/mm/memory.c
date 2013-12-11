#include <memory.h>
#include <type.h>


u32int kmalloc(u32int size)
{
    u32int m_start;
    if (placement_addr & (0xfff))
    {
        placement_addr &= 0xfffff000;
        placement_addr += 0x1000;
    }
    m_start = placement_addr;
    placement_addr += size;
    return m_start;
}
