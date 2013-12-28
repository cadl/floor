#include <string.h>
#include <type.h>

void memset(void *s, u8int c, u32int n) // c value, n byte
{
    int i;
    for (i=0; i<n; i++)
    {
        *((u8int *)s + i) = c;
    }
}

void memcpy(void *dst, const void *src, u32int n) // n byte
{
    int i;
    for (i=0; i<n; i++)
    {
        *((u8int *)dst + i) = *((u8int *)src + i);
    }
}
