#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

static const size_t list_starting_cap = 16;

static const size_t expansion_multiplier = 2;
static const size_t list_max_cap = 1e6;

mylist::mylist()
{
    cap = list_starting_cap;
    buff = (list_elm_t*) calloc(cap + 1, sizeof(*buff));
    next = (size_t*) calloc(cap + 1, sizeof(*next));
    prev = (size_t*) calloc(cap + 1, sizeof(*prev));
    if (!(buff && prev && next))
    {
        free(buff); free(prev); free(next);
        fprintf(stderr, "mylist: Allocation error!\n");
        return;
    }
    next[0] = 0;
    prev[0] = 0;
    for (size_t i = 1; i <= cap; i++)
    {
        prev[i] = -1lu; // FUCK: unnessesary
        next[i] = i + 1;
    }
    next[cap] = 0;

    inited = true;
}
mylist::~mylist()
{
    free(buff);
    free(next);
    free(prev);
    buff = nullptr;
    prev = nullptr;
    next = nullptr;
    ifree = 1;
    size = 0;
    cap = 0;
    inited = false;
}

size_t mylist::getsize()
{
    return size;
}
size_t mylist::getcap()
{
    return cap;
}
size_t mylist::getfree()
{
    return ifree;
}
size_t mylist::getnext(size_t ind)
{
    return next[ind];
}
size_t mylist::getprev(size_t ind)
{
    return prev[ind];
}

size_t mylist::request_free()
{
    if (ifree == 0)
    {
        expand();
    }
    size_t ret = ifree;
    ifree = next[ifree];
    return ret;
}
int mylist::expand()
{
    size_t new_cap = cap * expansion_multiplier;

    if (new_cap > list_max_cap)
    {
        fprintf(stderr, "mylist::expand(): Too big capacity to reallocate\n");
        return 2;
    }
    list_elm_t* new_buff = (list_elm_t*) realloc(buff, (new_cap+1) * sizeof(*buff));
    if (!new_buff)
    {
        fprintf(stderr, "mylist::expand(): Reallocation error!\n");
        return 1;
    }
    else
        buff = new_buff;

    size_t* new_prev = (size_t*) realloc(prev, (new_cap+1) * sizeof(*prev));
    if (!new_prev)
    {
        fprintf(stderr, "mylist::expand(): Reallocation error!\n");
        return 1;
    }
    else
        prev = new_prev;

    size_t* new_next = (size_t*) realloc(next, (new_cap+1) * sizeof(*next));
    if (!new_next)
    {
        fprintf(stderr, "mylist::expand(): Reallocation error!\n");
        return 1;
    }
    else
        next = new_next;

    ifree = cap+1;
    for (size_t i = cap+1; i <= new_cap; i++)
    {
        next[i] = i+1;
        prev[i] = -1lu; // FUCK: unnessesary
    }
    next[new_cap] = 0;

    cap = new_cap;
    return 0;
}

int mylist::insert_after(list_elm_t elm, size_t ind)
{
    size_t free_ind = request_free();

    size_t old_next = next[ind];
    buff[free_ind] = elm;
    next[ind] = free_ind;
    prev[free_ind] = ind;
    next[free_ind] = old_next;
    prev[old_next] = free_ind;
    size++;
    return 0;
}
int mylist::insert_before(list_elm_t elm, size_t ind)
{
    size_t free_ind = request_free();

    size_t old_prev = prev[ind];
    buff[free_ind] = elm;
    prev[ind] = free_ind;
    next[free_ind] = ind;
    prev[free_ind] = old_prev;
    next[old_prev] = free_ind;
    size++;
    return 0;
}
int mylist::erase(size_t ind)
{
    if (ind == 0)
    {
        fprintf(stderr, "mylist::erase(): ind == 0!!!\n");
        return -1;
    }
    size_t old_next = next[ind];
    size_t old_prev = prev[ind];
    prev[old_next] = old_prev;
    next[old_prev] = old_next;

    prev[ind] = -1lu; // FUCK: unnessesary

    next[ind] = ifree;
    ifree = ind;
    size--;
    return 0;
}

list_elm_t mylist::at(size_t ind)
{
// FUCK: errors
    return buff[ind];
}

