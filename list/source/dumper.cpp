#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "list.h"
#include "colors.h"
#ifdef DO_LIST_DUMPS // FUCK: move up

#define LIST_ELM_T_FORMAT "d"
static char html_filename[] = "/dumps.html";

int mylist_dumper::start (const char* dump_dir)
{
    if (strnlen(dump_dir, MAX_NAME_LEN) == MAX_NAME_LEN)
    {
        fprintf(stderr, "start_dumps(): Too long dump_dir name\n");
        return -1;
    }
    strcpy(root_dir, dump_dir);

    strcpy(dot_dir, root_dir);
    strcat(dot_dir, "/imgs/dots");

    strcpy(png_dir, root_dir);
    strcat(png_dir, "/imgs/pngs");

    char system_call[MAX_NAME_LEN+20] = {};
    sprintf(system_call, "%s%s", "mkdir -p ", root_dir);
    system(system_call);
    sprintf(system_call, "%s%s", "mkdir -p ", png_dir);
    system(system_call);
    sprintf(system_call, "%s%s", "mkdir -p ", dot_dir);
    system(system_call);

    char html_filename_w_path[MAX_NAME_LEN + 20];
    strcpy(html_filename_w_path, root_dir);
    strcat(html_filename_w_path, html_filename);
    html_fp = fopen(html_filename_w_path, "w");
    if (!html_fp)
    {
        fprintf(stderr, "mylist_dumper::start(): Could not open html file\n");
        return 1;
    }
    fprintf(html_fp,
            "<body style = \"background-color: #ffffffff; margin: 0px;\">\n"
            "<div style = \"margin: 0px; "
            "font-weight: bold; "
            "color: #000000; "
            "font-size: 16; "
            "font-family: Verdana, sans-serif; "
            "width: 100%%;\">\n"
            );

    started = true;
    return 0;
}

int mylist_dumper::new_dump(mylist* ls, const char reason[], code_position_t from)
{
    if (ls == nullptr)
    {
        fprintf(stderr, "mylist_dumper::new_dump(): ls == nullptr!\n");
        return -1;
    }
    if (started == false)
    {
        fprintf(stderr, "mylist_dumper::new_dump(): duming not started (try calling mylist_dumper::start())\n");
        return -2;
    }

    new_dot(ls);
    new_htm(ls, reason, from);

    fflush(html_fp);
    dump_num++;
    return 0;
}

void mylist_dumper::end()
{
    if (started)
    {
        fprintf(html_fp, "</div>\n</body>");
        fclose(html_fp);
    }
    started = false;
}

int mylist_dumper::new_dot(mylist* ls)
{
    char dot_name[MAX_NAME_LEN+20] = {};
    sprintf(dot_name, "%s%4s%lu%4s", dot_dir, "/img", dump_num, ".dot");
    FILE* dot_fp = fopen(dot_name, "w");
    if (!dot_fp)
    {
        fprintf(stderr, "mylist_dumper::new_dot: Failed to open .dot file\n");
        return 1;
    }
    char png_name[MAX_NAME_LEN+20] = {};
    sprintf(png_name, "%s%4s%lu%4s", png_dir, "/img", dump_num, ".png");

    fprintf(dot_fp, "digraph\n{\nrankdir = LR; bgcolor = \"#ffffffff\"; ranksep = 0.6;\n");

    size_t cap = ls->cap();
    for(size_t i = 0; i <= cap; i++)
    {
        size_t color = CL_LIGHT_GREEN;
        if (ls->prev(i) == -1lu)
            color = CL_LIGHT_RED;
        if (i == 0)
            color = CL_LIGHT_YELLOW;
        fprintf(dot_fp, "node%lu [shape = Mrecord, fillcolor = \"#%.8lX\", style=filled; label =\""
                        "{<ind>ind\\n%lu | {<next> next\\n%lu | <val> value\\n%" LIST_ELM_T_FORMAT " | <prev> prev\\n%ld }}\"];\n",
                        i, color, i, ls->next(i), ls->at(i), ls->prev(i));
    }
    fprintf(dot_fp, "edge [constraint = true; style = \"invis\";]\n");
    for(size_t i = 0; i < cap; i++)
    {
        fprintf(dot_fp, "node%lu -> node%lu;\n", i, i+1);
    }
    fprintf(dot_fp, "edge [constraint = false; style = \"\"; color = \"#%.8X\";]\n", CL_SATURATED_GREEN);
    for(size_t i = 0; i <= cap; i++)
    {
        size_t next = ls->next(i);
        fprintf(dot_fp, "node%lu -> node%lu;\n", i, next);
    }
    fprintf(dot_fp, "edge [constraint = false; style = \"\"; color = \"#%.8X\";]\n", CL_SATURATED_RED);
    for(size_t i = 0; i <= cap; i++)
    {
        size_t prev = ls->prev(i);
        if (prev == -1lu)
            continue;
        else
            fprintf(dot_fp, "node%lu -> node%lu;\n", i, prev);
    }
    fprintf(dot_fp, "free [shape = circle, color = \"#%.8X\", label = \"free\\n%lu\"];\n", CL_LIGHT_BROWN, ls->m_free);
    if (ls->m_free != 0)
    {
        fprintf(dot_fp, "free -> node%lu [constraint = false; style = \"\"; color = \"#%.8X\";];\n", ls->m_free, CL_LIGHT_BROWN);
    }
    fprintf(dot_fp, "}\n");
    fclose(dot_fp);
    char syscall[2 * MAX_NAME_LEN + 60] = {};
    sprintf(syscall, "dot %s -Tpng -o %s", dot_name, png_name);
    system(syscall);
    return 0;
}

int mylist_dumper::new_htm(mylist* ls, const char reason[], code_position_t from)
{
    fprintf(html_fp, "<pre style = \"margin-left: 10px;\">\n");
    fprintf(html_fp, "LIST_DUMP (called from %s:%d in function %s, reason: %s, num = %lu)\n",
                      from.file, from.line, from.func, reason, dump_num);
    fprintf(html_fp, "cap = %lu\n", ls->cap());
    fprintf(html_fp, "size = %lu\n", ls->size());
    fprintf(html_fp, "</pre>\n");

    char png_name[MAX_NAME_LEN] = {};
    sprintf(png_name, "%s%lu%s", "imgs/pngs/img", dump_num, ".png");

    fprintf(html_fp, "<div style = \"overflow-x:scroll; width: 100%%; height: 45%%\">\n<img src = %s style = \"height: 100%%;\">\n</div>\n", png_name);
    fprintf(html_fp, "<hr style = \"border: 0px; width: 100%%; height: 6px; color: black; background-color: black;\">\n");
    return 0;
}

#endif // DO_LIST_DUMPS
