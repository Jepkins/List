#include <stdio.h>
#include "list.h"

int main()
{
    const int test_size = 20;
    mylist ls;
    ls.ctor();
    mylist_dumper dmp;
    dmp.start("dumps");
    list_elm_t val = 0;
    for (size_t i = 0; i < test_size; i++, val+=10)
    {
        if (i == 15)
            ls.erase(5);
        ls.insert_before(val, 0);
    }
    dmp.new_dump(&ls);
    for (size_t i = 1; i <= test_size; i++)
    {
        if (i % 2)
            ls.erase(i);
    }
    dmp.new_dump(&ls);
    ls.linearize();
    dmp.new_dump(&ls);
    ls.shrink_to_fit();
    dmp.new_dump(&ls);

    dmp.end();
    ls.dtor();
    return 0;
}
//     size_t ind = ls.getprev(0);
//     while (ls.prev(ind) != 0)
//     {
//         printf("[%lu] = %d\n", ind, ls.at(ind));
//         ind = ls.getprev(ind);
//     }
//     printf("[%lu] = %d\n", ind, ls.at(ind));
