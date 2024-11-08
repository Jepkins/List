#include <stdio.h>
#include "list.h"

int main()
{
    const int test_size = 8;
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
    dmp.new_dump(&ls, "initial", POS__);
    for (size_t i = 1; i <= test_size; i++)
    {
        if (i % 3 == 0)
            ls.erase(i);
    }
    dmp.new_dump(&ls, "checking (each 3rd deleted)", POS__);

    ls.shrink_to_fit();
    dmp.new_dump(&ls, "checking (shrink_to_fit)", POS__);

    int err_code = 0;
    if ((err_code = ls.verify()) != LIST_OK)
    {
        dmp.new_dump(&ls, "verification err", POS__);
        fprintf(stderr, "Verification error (code = %d), list dumped\n", err_code);
    }
    dmp.end();
    ls.dtor();
    return 0;
}
