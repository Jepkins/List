#ifndef LIST_H
#define LIST_H
#define DO_LIST_DUMPS // FUCK: remove

#include <stdlib.h>
#include <stdio.h>

#ifndef CODE_POSITION
#define CODE_POSITION
typedef struct {
    const char* file;
    int line;
    const char* func;
} code_position_t;
#define POS__ {__FILE__, __LINE__, __func__}
#endif // CODE_POSITION

typedef int list_elm_t;

enum list_errors_t {
    LIST_OK = 0,
    LIST_BAD_CONNECT  = 1,
    LIST_SIZE_UNMATCH = 2
};

#ifdef DO_LIST_DUMPS
typedef class mylist_dumper mylist_dumper;
#endif // DO_LIST_DUMPS

class mylist {
public:
    int  ctor();
    void dtor();
    list_errors_t verify();

    size_t size()                        const;
    size_t cap()                         const;
    size_t next(size_t ind)              const;
    size_t prev(size_t ind)              const;
    size_t find_by_logic(size_t log_ind) const;
    list_elm_t at(size_t ind)            const;

    int linearize();
    int shrink_to_fit();

    int insert_after (list_elm_t elm, size_t ind);
    int insert_before(list_elm_t elm, size_t ind);
    int insert_back  (list_elm_t elm);
    int insert_front (list_elm_t elm);

    int erase(size_t ind);
    int erase_back ();
    int erase_front();

// private:
    size_t request_free();
    int resize_w_linearization(size_t new_cap);

    list_elm_t* m_buff{nullptr};
    size_t* m_prev{nullptr};
    size_t* m_next{nullptr};
    size_t m_free {1};
    size_t m_size {0};
    size_t m_cap  {0};
    bool m_inited   {false};

#ifdef DO_LIST_DUMPS
    friend mylist_dumper;
#endif // DO_LIST_DUMPS
};

#ifdef DO_LIST_DUMPS

#define MAX_NAME_LEN 100
class mylist_dumper {
public:
    int start (const char* dump_dir);
    int new_dump (mylist* ls, const char reason[] = "unknown", code_position_t from = {});
    void end();
private:
    int new_htm(mylist* ls, const char reason[], code_position_t from);
    int new_dot(mylist* ls);
    char m_root_dir[MAX_NAME_LEN]{};
    char m_dot_dir [MAX_NAME_LEN]{};
    char m_png_dir [MAX_NAME_LEN]{};
    FILE* m_html_fp   {nullptr};
    size_t m_dump_num   {0};
    bool m_started      {false};
};

#endif // DO_LIST_DUMPS

#endif // LIST_H
