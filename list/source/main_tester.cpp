#include <stdio.h>
#include "list.h"

const int test_size = 20;

int main()
{
    mylist ls;
    mylist_dumper dmp;
    dmp.start("dumps");
    list_elm_t val = 0;
    for (size_t i = 0; i < test_size; i++, val+=10)
        ls.insert_before(val, 0);
    dmp.new_dump(&ls);
    for (size_t i = 1; i <= test_size; i++)
    {
    dmp.new_dump(&ls);
        if (i % 2)
        ls.erase(i);
    }
    dmp.new_dump(&ls);

//     size_t ind = ls.getprev(0);
//     while (ls.getprev(ind) != 0)
//     {
//         printf("[%lu] = %d\n", ind, ls.at(ind));
//         ind = ls.getprev(ind);
//     }
//     printf("[%lu] = %d\n", ind, ls.at(ind));
//
//     printf("cap = %lu, [2] = %d, next[2] = %lu, prev[2] = %ld\n", ls.getcap(), ls.at(2), ls.getnext(2), ls.getprev(2));
    dmp.end();
    return 0;
}
