#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "common.h"

int opt_max = MAX_TOPS;
int opt_solve = 1;
int opt_debug = 1;
const char *opt_input = NULL;
const char *opt_output = "output.svg";

int max_tops = 0;
int cnt_tops = 0;
top_t *tops = NULL;
top_t *prof = NULL;
top_t *rest = NULL;

/**
 * file_read_tops - read tops from file into tops
 * @fname:      file name
 * Return:      count
 */
static int file_read_tops(const char *fname)
{
    FILE *fp;
    top_t *top = &tops[0];
    top_t *end = &tops[max_tops];
    int x, y, w;

    if (fname == NULL)
        fp = stdin;
    else
        fp = fopen(fname, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "# failed to open %s\n", fname);
        exit(EXIT_READ_INPUT);
    }

    while (top < end)
    {
        int cnt = fscanf(fp, "%d %d %d", &x, &y, &w);
        if (cnt == EOF)
            break;
        if (cnt != 3)
        {
            fprintf(stderr, "# failed to read %s\n", fname ?: "stdin");
            exit(EXIT_READ_INPUT);
        }
        if (x <= 0 || y <= 0 || w <= 0)
        {
            if (fname) fclose(fp);
            return -1;
        }
        top->x = x;
        top->y = y;
        top->w = w;
        ++top;
    }

    if (fname) fclose(fp);
    return top - &tops[0];
}

/**
 * compare_tops - compare tops on x
 * @a:          top a
 * @b:          top b
 * Return:      a-b
 */
static int compare_tops(const void *a, const void *b)
{
    return ((const top_t*)a)->x - ((const top_t*)b)->x;
}

/**
 * sort_tops - sort tops by x
 * @ntops:      number of tops
 * @ptops:      array of tops
 */
static void sort_tops(int ntops, top_t ptops[])
{
    qsort(ptops, ntops, sizeof(top_t), compare_tops);
    fprintf(stderr, "# %d tops sorted\n", cnt_tops);

    /* also create link list */
    top_t *n = NULL;
    for (int i = ntops-1; i >= 0; --i)
    {
        top_t *t = &ptops[i];
        t->n = n;
        n = t;
    }
}

/**
 * copy_tops_to_prof - copy ptops to prof using rest
 * @ptops:      list of tops
 */
static int copy_tops_to_prof(top_t *ptops)
{
    int cnt = 0;
    top_t **p = &prof;
    for (prof = NULL; ptops != NULL; ptops = ptops->n)
    {
        top_t *t = rest;
        if (rest == NULL)
            return -1;
        rest = rest->n;
        t->x = ptops->x;
        t->y = ptops->y;
        t->w = ptops->w;
        t->n = NULL;
        *p = t;
        p = &t->n;
        ++cnt;
    }
    fprintf(stderr, "# %d tops copied\n", cnt);
    return cnt;
}

/**
 * usage - print usage and exit on error
 * @err:        error
 * @msg:        message
 * Return:      err
 */
static int usage(int err, const char *msg)
{
    if (msg) printf("%s", msg);

    printf("%s",
           "\n"
           "city-profile [options...]\n"
           "\n"
           "options:\n"
           "--help\n"
           "-h          this help\n"
           "--solve=0/1\n"
           "-s0/1       solve (default 1)\n"
           "--debug=0/1\n"
           "-d0/1       debug (default 1)\n"
           "--max=int\n"
           "-m int      max tops to alloc\n"
           "--input=input\n"
           "-i input    input tops (x y w)\n"
           "--output=output\n"
           "-o output   output svg file\n");

    if (err) exit(err);
    return err;
}

/**
 * parse_args - parse cmdline args
 * @argc:       argc
 * @argv:       argv
 * Return:      error
 */
static int parse_args(int argc, char * const argv[])
{
    static const char *shortopts = "hs:d:m:i:o:";
    static struct option longopts[] =
    {
        { "help",   no_argument,       0, 'h' },
        { "solve",  required_argument, 0, 's' },
        { "debug",  required_argument, 0, 'd' },
        { "max",    required_argument, 0, 'm' },
        { "input",  required_argument, 0, 'i' },
        { "output", required_argument, 0, 'o' },
        { NULL,     0,                 0,  0  },
    };
    int c;
    int index;

    while ((c = getopt_long(argc, argv, shortopts, longopts, &index)) >= 0)
    {
        switch (c)
        {
            case 'h':
                usage(0, NULL);
                exit(0);
                break;
            case 's':
                opt_solve = atoi(optarg);
                break;
            case 'd':
                opt_debug = atoi(optarg);
                break;
            case 'm':
                opt_max = atoi(optarg);
                break;
            case 'i':
                opt_input = optarg;
                break;
            case 'o':
                opt_output = optarg;
                break;
            case '?':
                usage(1, NULL);
                break;
        }
    }
    return 0;
}

int main(int argc, char * const argv[])
{
    if (parse_args(argc, argv) < 0)
    {
        return EXIT_USAGE;
    }

    /* allocate max tops */
    max_tops = (opt_max <= 0 ? MAX_TOPS : opt_max);
    tops = (top_t*)calloc(max_tops, sizeof(top_t));
    if (tops == NULL)
    {
        perror("failed to allocate max tops");
        return EXIT_MEMORY;
    }

    cnt_tops = file_read_tops(opt_input);
    if (cnt_tops == 0)
    {
        fprintf(stderr, "no input\n");
        return EXIT_EMPTY_INPUT;
    }
    if (cnt_tops < 0)
    {
        fprintf(stderr, "invalid input\n");
        return EXIT_INVALID_INPUT;
    }
    if (cnt_tops + cnt_tops >= max_tops)
    {
        fprintf(stderr, "too many tops\n");
        return EXIT_LARGE_INPUT;
    }

    fprintf(stderr, "# %d tops read\n", cnt_tops);
    sort_tops(cnt_tops, tops);

    /* create rest list */
    for (int i = max_tops-1; i >= cnt_tops; --i)
    {
        top_t *t = &tops[i];
        t->n = rest;
        rest = t;
    }
    fprintf(stderr, "# %d tops left\n", max_tops-cnt_tops);

    if (opt_solve)
    {
        if (copy_tops_to_prof(tops) < 0)
        {
            fprintf(stderr, "failed to copy prof\n");
            return EXIT_COPY_INPUT;
        }
        int cnt = solve_profile(&prof, &rest);
        if (cnt < 0)
        {
            fprintf(stderr, "failed to solve\n");
            return EXIT_SOLVE;
        }
        fprintf(stderr, "# %d tops solved\n", cnt);
        for (top_t *p = prof; p != NULL; p = p->n)
        {
            fprintf(stderr, "# %d %d %d\n", p->x, p->y, p->w);
        }
    }

    if (make_svg(opt_output, tops, prof) < 0)
    {
        fprintf(stderr, "failed to make svg\n");
        return EXIT_WRITE_OUTPUT;
    }

    free(tops);
    return 0;
}

/*
 * Local Variables:
 *   c-file-style: "stroustrup"
 *   indent-tabs-mode: nil
 * End:
 *
 * vim: set ai cindent et sta sw=4:
 */
