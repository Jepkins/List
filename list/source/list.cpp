#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

static const size_t default_cap = 256;

mylist::mylist(size_t base_cap)
{
    cap = base_cap? base_cap : default_cap;
    buff = (list_elm_t*) calloc(cap + 1, sizeof(*buff));
    next = (size_t*) calloc(cap + 1, sizeof(*next));
    prev = (size_t*) calloc(cap + 1, sizeof(*prev));
    if (!(buff && prev && next))
    {
        fprintf(stderr, "mylist: Allocation error!\n");
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
}

size_t mylist::getsize()
{
    return size;
}
size_t mylist::getcap()
{
    return cap;
}
size_t mylist::gethead()
{
    return head;
}
size_t mylist::gettail()
{
    return tail;
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
        // FUCK realloc
    }
    size_t ret = ifree;
    ifree = next[ifree];
    return ret;
}

int mylist::insert_after(list_elm_t elm, size_t ind)
{
    if (ind == 0)
    {
        fprintf(stderr, "mylist::insert_after(): ind == 0!!!\n");
        return -1;
    }
    size_t free_ind = request_free();

    size_t old_next = next[ind];
    buff[free_ind] = elm;
    next[ind] = free_ind;
    prev[free_ind] = ind;
    next[free_ind] = old_next;
    if (old_next != 0) prev[old_next] = free_ind;
    size++;
    return 0;
}
int mylist::insert_before(list_elm_t elm, size_t ind)
{
    if (ind == 0)
    {
        fprintf(stderr, "mylist::insert_before(): ind == 0!!!\n");
        return -1;
    }
    size_t free_ind = request_free();

    size_t old_prev = prev[ind];
    buff[free_ind] = elm;
    prev[ind] = free_ind;
    next[free_ind] = ind;
    prev[free_ind] = old_prev;
    if (old_prev != 0) next[old_prev] = free_ind;
    size++;
    return 0;
}
int mylist::insert_at(list_elm_t elm, size_t ind)
{
    insert_after(buff[ind], ind);
    buff[ind] = elm;
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
    if (old_next != 0) prev[old_next] = old_prev;
    if (old_prev != 0) next[old_prev] = old_next;

    buff[ind] = -1; prev[ind] = -1lu; // FUCK: unnessesary

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
