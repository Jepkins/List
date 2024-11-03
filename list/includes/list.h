#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef int list_elm_t;

class mylist {
public:
    // WORK: push_back pop_back ...
    int ctor();
    void dtor();
    size_t size()                        const;
    size_t cap()                         const;
    size_t next(size_t ind)              const;
    size_t prev(size_t ind)              const;
    size_t find_by_logic(size_t log_ind) const;
    list_elm_t at(size_t ind)            const;
    int linearize();
    int shrink_to_fit();
    int insert_after(list_elm_t elm, size_t ind);
    int insert_before(list_elm_t elm, size_t ind);
    int erase(size_t ind);
private:
    size_t request_free();
    int resize_w_linearization(size_t new_cap);
    list_elm_t* m_buff{nullptr};
    size_t* m_prev{nullptr};
    size_t* m_next{nullptr};
    size_t m_free {1};
    size_t m_size {0};
    size_t m_cap  {0};
    bool inited   {false};
};

#define DO_LIST_DUMPS // FUCK: remove
#ifdef DO_LIST_DUMPS

#define MAX_NAME_LEN 100
class mylist_dumper {
public:
    int start (const char* dump_dir);
    int new_dump (mylist* ls);
    void end();
private:
    int new_htm(mylist* ls);
    int new_dot(mylist* ls);
    char root_dir[MAX_NAME_LEN]{};
    char dot_dir [MAX_NAME_LEN]{};
    char png_dir [MAX_NAME_LEN]{};
    FILE* html_fp   {nullptr};
    size_t dump_num {0};
    bool started    {false};
};
#endif // DO_LIST_DUMPS

#endif // LIST_H
