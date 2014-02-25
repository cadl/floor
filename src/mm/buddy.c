#include <type.h>
#include <buddy.h>
#include <sys.h>
#include <memory.h>
#ifdef BUDDY_TEST
#define ALLOC malloc
#include <stdlib.h>
#include <stdio.h>
#else
#define ALLOC kmalloc
#endif

struct buddy_alloctor page_alloctor;

static inline int _is_power_of_two(u32int x)
{
    return !(x & (x - 1));
}

static inline u32int _log2(u32int x)
{
    if (x < 2)
        return 0;
    return 1 + _log2(x/2);
}

static inline s8int _int_max(s8int a, s8int b)
{
    return a > b? a: b;
}

static inline void _make_parent(struct buddy_alloctor *alloctor, u32int node_idx)
{
    u32int parent_idx, buddy_idx;

    if (node_idx == 0)
        return;

    buddy_idx = node_idx - 1 + (node_idx & 1) * 2;
    parent_idx = ((node_idx + 1) >> 1) - 1;

    alloctor->powers[parent_idx] = _int_max(alloctor->powers[node_idx],
                                             alloctor->powers[buddy_idx]);
    _make_parent(alloctor, parent_idx);
}

static inline u32int _index_offset(u32int node_idx, s8int node_pow, s8int root_pow)
{
    return (node_idx - (1 << (root_pow - node_pow)) + 1) * (1 << node_pow);
}

static inline void _combine(struct buddy_alloctor *alloctor, u32int node_idx)
{
    u32int buddy_idx, parent_idx;
    s8int node_pow;

    if (node_idx == 0)
        return;

    buddy_idx = node_idx - 1 + (node_idx & 1) * 2;
    parent_idx = (node_idx+1)/2 - 1;

    node_pow = alloctor->powers[node_idx];
    if (alloctor->powers[buddy_idx] == node_pow)
    {
        alloctor->powers[parent_idx] = node_pow + 1;
        _combine(alloctor, parent_idx);
    }
    else
    {
        _make_parent(alloctor, node_idx);
    }
}

struct buddy_alloctor* buddy_new(u32int size, u32int block_size)
{
    struct buddy_alloctor *alloctor;
    u32int node_num, length, i;
    s8int power;

    length = size / block_size;
    if (!_is_power_of_two(length))
    {
        length |= length >> 1;
        length |= length >> 2;
        length |= length >> 4;
        length |= length >> 8;
        length |= length >> 16;
        length += 1;
    }

    node_num = 2 * length - 1;
    alloctor = (struct buddy_alloctor *)ALLOC(sizeof(struct buddy_alloctor));
    alloctor->size = block_size * length;
    alloctor->block_size = block_size;

    alloctor->powers = (s8int *)ALLOC(sizeof(s8int)*(node_num));
    power = _log2(length) + 1;

    for (i=0; i<node_num; i++)
    {
        if (_is_power_of_two(i+1))
        {
            power--;;
        }
        alloctor->powers[i] = power;
    }
    return alloctor;
}

u32int buddy_malloc(struct buddy_alloctor* alloctor, u32int size)
{
    u32int i, length, node_num;
    s8int power, root_power;

    i = 0;
    size = size / alloctor->block_size;
    if (!_is_power_of_two(size))
    {
        size |= size>>1;
        size |= size>>2;
        size |= size>>4;
        size |= size>>8;
        size |= size>>16;
        size += 1;
    }

    length = alloctor->size / alloctor->block_size;
    root_power = _log2(length);
    power = _log2(size);
    node_num = 2 *length -1;

    while (i<node_num)
    {
        if (alloctor->powers[i] < power)
            return -1;
        if ((i<<1)+1 < node_num && alloctor->powers[(i<<1)+1] >= power)
        {
            i = (i<<1) + 1;
        }
        else if ((i<<1)+2 < node_num && alloctor->powers[(i<<1)+2] >= power)
        {
            i = (i<<1) + 2;
        }
        else
        {
            alloctor->powers[i] = -1;
            _make_parent(alloctor, i);
            return _index_offset(i, power, root_power) * alloctor->block_size;
        }
    }
    return -1;
}

void buddy_free(struct buddy_alloctor *alloctor, u32int offset)
{
    u32int length, node_num, left_length, power, i;

    length = alloctor->size / alloctor->block_size;
    offset = offset / alloctor->block_size;
    node_num = 2 * length - 1;
    i = left_length = 0;

    while (i < node_num)
    {
        length = length / 2;
        if (left_length == offset)
        {
            if (alloctor->powers[i] == -1)
            {
                power = _log2(length*2);
                alloctor->powers[i] = power;
                _combine(alloctor, i);
                return;
            }
        }
        if(left_length + length <= offset)
        {
            left_length = left_length + length;
            i = i * 2 + 2;
        }
        else
        {
            i = i * 2 + 1;
        }
    }
}
