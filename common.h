#ifndef __COMMON_H
#define __COMMON_H

#define EXIT_USAGE              1
#define EXIT_MEMORY             2
#define EXIT_READ_INPUT         3
#define EXIT_EMPTY_INPUT        4
#define EXIT_INVALID_INPUT      5
#define EXIT_LARGE_INPUT        6
#define EXIT_COPY_INPUT         7
#define EXIT_WRITE_OUTPUT       8
#define EXIT_SOLVE              9

extern int opt_max;
extern int opt_solve;
extern int opt_debug;
extern const char *opt_input;
extern const char *opt_output;

/*
 * top of a building
 * top-left      @ (x,y)
 * top-right     @ (x+w,y)
 * bottome-left  @ (x, 0)
 * bottome-right @ (x+w, 0)
 */
typedef struct top {
    int x;
    int y;
    int w;
    struct top *n;
} top_t;

#define MAX_TOPS 2048
extern int max_tops;
extern int cnt_tops;
extern top_t *tops;
extern top_t *prof;
extern top_t *rest;

int solve_profile(top_t **pprof, top_t **prest);
int make_svg(const char *fname, const top_t *ptops, const top_t *pprof);

#endif/*__COMMON_H*/
/*
 * Local Variables:
 *   c-file-style: "stroustrup"
 *   indent-tabs-mode: nil
 * End:
 *
 * vim: set ai cindent et sta sw=4:
 */
