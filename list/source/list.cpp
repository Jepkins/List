#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

static const size_t list_starting_cap = 8;

static const size_t expansion_multiplier = 2;
static const size_t list_max_cap = 1e6;

#define ERRMSG_(msg) fprintf(stderr, "%s:" msg "\n", __func__)

int mylist::ctor()
{
    m_cap = list_starting_cap;
    m_buff = (list_elm_t*) calloc(m_cap + 1, sizeof(*m_buff));
    m_next = (size_t*) calloc(m_cap + 1, sizeof(*m_next));
    m_prev = (size_t*) calloc(m_cap + 1, sizeof(*m_prev));
    if (!(m_buff && m_prev && m_next))
    {
        free(m_buff); free(m_prev); free(m_next);
        ERRMSG_("Allocation error!");
        return 1;
    }
    m_next[0] = 0;
    m_prev[0] = 0;
    for (size_t i = 1; i <= m_cap; i++)
    {
        m_prev[i] = -1lu;
        m_next[i] = i + 1;
    }
    m_next[m_cap] = 0;

    inited = true;
    return 0;
}
void mylist::dtor()
{
    free(m_buff);
    free(m_next);
    free(m_prev);
    m_buff = nullptr;
    m_prev = nullptr;
    m_next = nullptr;
    m_free = 1;
    m_size = 0;
    m_cap = 0;
    inited = false;
}

list_errors_t mylist::verify()
{
    size_t phys_idx = 0;
    size_t log_idx  = 0;
    for (; log_idx <= m_size; log_idx++)
    {
        if (m_prev[m_next[phys_idx]] != phys_idx ||
            m_next[m_prev[phys_idx]] != phys_idx)
        {
            return LIST_BAD_CONNECT;
        }
        if (m_next[phys_idx] == 0)
            break;
        phys_idx = m_next[phys_idx];
    }
    if (log_idx != m_size)
        return LIST_SIZE_UNMATCH;
    return LIST_OK;
}

size_t mylist::size() const
{
    return m_size;
}
size_t mylist::cap() const
{
    return m_cap;
}
size_t mylist::next(size_t idx) const
{
    if (idx > m_cap)
    {
        ERRMSG_("Invalid idx!");
        return -1lu;
    }
    return m_next[idx];
}
size_t mylist::prev(size_t idx) const
{
    if (idx > m_cap)
    {
        ERRMSG_("Invalid idx!");
        return -1lu;
    }
    return m_prev[idx];
}

size_t mylist::find_by_logic(size_t log_idx) const
{
    if (log_idx > m_size)
    {
        ERRMSG_("log_idx is greater than list size!");
        return -1lu;
    }
    size_t phys_idx = 0;
    for (size_t i = 0; i < log_idx; i++)
    {
        phys_idx = m_next[phys_idx];
    }
    return phys_idx;
}

int mylist::linearize()
{
    return resize_w_linearization(m_cap);
}
int mylist::shrink_to_fit()
{
    return resize_w_linearization(m_size);
}
int mylist::resize_w_linearization(size_t new_cap)
{
    if (new_cap > list_max_cap)
    {
        ERRMSG_("Too big capacity to reallocate!");
        return -1;
    }
    list_elm_t* new_buff = (list_elm_t*) calloc((new_cap+1), sizeof(*m_buff));
    if (!new_buff)
    {
        ERRMSG_("Reallocation error!");
        return 1;
    }

    size_t* new_prev = (size_t*) calloc((new_cap+1), sizeof(*m_prev));
    if (!new_prev)
    {
        free(new_buff);
        ERRMSG_("Reallocation error!");
        return 1;
    }

    size_t* new_next = (size_t*) calloc((new_cap+1), sizeof(*m_next));
    if (!new_next)
    {
        free(new_buff); free(new_prev);
        ERRMSG_("Reallocation error!");
        return 1;
    }

    bool full = (new_cap == m_size);
    if (new_cap < m_size)
    {
        ERRMSG_("Warning: some elements were cut off during reallocation!");
        m_size = new_cap;
        full = true;
    }

    for (size_t log_idx = 0, phys_idx = 0; log_idx <= m_size; log_idx++)
    {
        new_buff[log_idx] = m_buff[phys_idx];
        new_prev[log_idx] = log_idx-1;
        new_next[log_idx] = log_idx+1;
        phys_idx = m_next[phys_idx];
    }
    new_prev[0] = m_size;
    new_next[m_size] = 0;
    free(m_buff);
    free(m_next);
    free(m_prev);
    m_buff = new_buff;
    m_next = new_next;
    m_prev = new_prev;

    m_free = full? 0 : m_size+1;
    for (size_t i = m_size+1; i <= new_cap; i++)
    {
        m_next[i] = i+1;
        m_prev[i] = -1lu;
    }
    m_next[new_cap] = 0;

    m_cap = new_cap;
    return 0;
}

size_t mylist::request_free()
{
    if (m_free == 0)
    {
        if (resize_w_linearization(m_cap * expansion_multiplier) != 0)
            return 0;
    }
    size_t ret = m_free;
    m_free = m_next[m_free];
    return ret;
}

int mylist::insert_after(list_elm_t elm, size_t idx)
{
    if (idx > m_cap || m_prev[idx] == -1lu)
    {
        ERRMSG_("invalid idx!");
        return -1;
    }

    size_t free_idx = request_free();

    if (free_idx == 0)
        return 1;

    size_t old_next = m_next[idx];
    m_buff[free_idx] = elm;
    m_next[idx] = free_idx;

    m_prev[free_idx] = idx;
    m_next[free_idx] = old_next;
    m_prev[old_next] = free_idx;

    m_size++;
    return 0;
}

int mylist::insert_before(list_elm_t elm, size_t idx)
{
    if (idx > m_cap || m_prev[idx] == -1lu)
    {
        ERRMSG_("Invalid idx!");
        return -1;
    }

    size_t free_idx = request_free();

    if (free_idx == 0)
        return 1;

    size_t old_prev = m_prev[idx];
    m_buff[free_idx] = elm;
    m_prev[idx] = free_idx;

    m_next[free_idx] = idx;
    m_prev[free_idx] = old_prev;
    m_next[old_prev] = free_idx;
    m_size++;
    return 0;
}
int mylist::insert_back  (list_elm_t elm)
{
    return insert_before(elm, 0);
}
int mylist::insert_front (list_elm_t elm)
{
    return insert_after(elm, 0);
}

int mylist::erase(size_t idx)
{
    if (idx > m_cap || m_prev[idx] == -1lu)
    {
        ERRMSG_("Invalid idx!");
        return -1;
    }

    if (idx == 0)
    {
        ERRMSG_("idx == 0!!!");
        return -1;
    }

    size_t old_next = m_next[idx];
    size_t old_prev = m_prev[idx];

    m_prev[old_next] = old_prev;
    m_next[old_prev] = old_next;

    m_prev[idx] = -1lu;

    m_next[idx] = m_free;

    m_free = idx;
    m_size--;
    return 0;
}
int mylist::erase_back  ()
{
    return erase(m_prev[0]);
}
int mylist::erase_front ()
{
    return erase(m_next[0]);
}


list_elm_t mylist::at(size_t idx) const
{
    if (idx > m_cap || m_prev[idx] == -1lu)
    {
        ERRMSG_("Invalid idx!");
        return -1;
    }
    return m_buff[idx];
}

#undef ERRMSG_
