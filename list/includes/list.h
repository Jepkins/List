#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef int list_elm_t;

class mylist {
private:
    list_elm_t* buff = nullptr;
    size_t* prev = nullptr;
    size_t* next = nullptr;
    size_t ifree = 1;
    size_t size = 0;
    size_t cap = 0;
    bool inited = false;

    mylist(const mylist&) = delete;             // FUCK: make the copy ctor
    mylist& operator=(const mylist&) = delete;  // FUCK: make the assignment
    // FUCK: make moves

    size_t request_free();
    int expand();
public:
    mylist();
    ~mylist();
    size_t getsize();
    size_t getcap();
    size_t getnext(size_t ind);
    size_t getprev(size_t ind);
    size_t getfree();
    int insert_after(list_elm_t elm, size_t ind);
    int insert_before(list_elm_t elm, size_t ind);
    int erase(size_t ind);
    list_elm_t at(size_t ind);
};

#define DO_LIST_DUMPS // FUCK: remove
#ifdef DO_LIST_DUMPS
#define MAX_NAME_LEN 100
class mylist_dumper {
private:
    char root_dir[MAX_NAME_LEN] = {};
    char dot_dir[MAX_NAME_LEN] = {};
    char png_dir[MAX_NAME_LEN] = {};
    FILE* html_fp = nullptr;
    size_t dump_num = 0;
    bool started = false;

    int new_htm(mylist* ls);
    int new_dot(mylist* ls);
public:
    int start (const char* dump_dir);
    int new_dump (mylist* ls);
    void end();
};
#endif // DO_LIST_DUMPS

#endif // LIST_H
