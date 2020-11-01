#include <stdio.h>
#include <assert.h>
#include "common.h"

/*
 * always assuming a <= b: * a->x <= b->x
 */

static inline int a_alignes_b(const top_t *a, const top_t *b)
{
    return a->x == b->x;
}
/*
 * a_strictly_before_b => a_loosely_before_b
 * !a_strictly_before_b => a_intersects_b
 */
static inline int a_strictly_before_b(const top_t *a, const top_t *b)
{
    return a->x + a->w < b->x;
}
/*
 * !a_strictly_before_b => a_intersects_b
 */
static inline int a_loosely_before_b(const top_t *a, const top_t *b)
{
    return a->x + a->w <= b->x;
}
static inline int a_touches_b(const top_t *a, const top_t *b)
{
    return a->x + a->w == b->x;
}
static inline int a_intersects_b(const top_t *a, const top_t *b)
{
    return /* a->x <= b->x && */ a->x + a->w >= b->x;
}

/* pop from rest list */
static top_t *pop_rest(top_t **prest)
{
    assert(prest);
    top_t *t = *prest;
    if (*prest)
    {
        *prest = t->n;
        t->n = NULL;
    }
    return t;
}

///* push into rest list */
//static void push_rest(top_t **prest, top_t *t)
//{
//    assert(prest);
//    t->n = *prest;
//    *prest = t;
//}

/* insert into list */
static void insert_top(top_t **pnext, top_t *t)
{
    assert(pnext && t);
    while (*pnext != NULL && (*pnext)->x < t->x)
        pnext = &(*pnext)->n;
    t->n = *pnext;
    *pnext = t;
}

#define debug(format, ...) (opt_debug ? fprintf(stderr, "%s:%d\t"format, __FILE__, __LINE__, ## __VA_ARGS__) : 0)

/**
 * solve_profile - solve profile
 * @pprof:	pointer to copied tops list to return
 * @prest:	pointer to free tops list
 * Return:	cnt or error if < 0
 */
int solve_profile(top_t **pprof, top_t **prest)
{
    int cnt = 0;
    top_t **pprev = NULL, *q; /* prev top */
    top_t **pnext = pprof, *p; /* next top */

    while ((p = *pnext) != NULL)
    {
        debug("p: x=%d y=%d w=%d\n", p->x, p->y, p->w);
        if (pprev == NULL)
        {
            pprev = pnext;
            ++cnt;
            goto next;
        }

        q = *pprev;
        debug("q: x=%d y=%d w=%d\n", q->x, q->y, q->w);

        if (a_touches_b(q, p) && q->y == p->y)
        {
            q->w += p->w;
            goto next;
        }
        if (a_loosely_before_b(q, p))
        {
            q->n = p;
            pprev = &q->n;
            ++cnt;
            goto next;
        }

        /*
         * w = width of intersection
         * a_intersects_b(q, p) + w > 0
         */
        int w = q->x + q->w - p->x; /* > 0 */

        /* extra before insection */
        if (q->x < p->x)
        {
            if (q->y < p->y)
            {
                top_t *r = pop_rest(prest);
                if (r == NULL)
                    return -EXIT_SOLVE;

                q->w = p->x - q->x;
                debug("q: x=%d y=%d w=%d\n", q->x, q->y, q->w);

                r->x = p->x;
                r->y = q->y;
                r->w = w;
                debug("r: x=%d y=%d w=%d\n", r->x, r->y, r->w);
                insert_top(&p->n, r);

                /* a_loosely_before_b(q, p) */
                q->n = p;
                pprev = &q->n;
                ++cnt;
                goto next;
            }
            else if (q->y == p->y)
            {
                /* q absorbs p */
                if (q->x + q->w < p->x + p->w)
                {
                    q->w = p->x + p->w - q->x;
                    debug("q: x=%d y=%d w=%d\n", q->x, q->y, q->w);
                }
                pnext = &p->n;
                goto next;
            }
            else  /* q->y > p->y */
            {
                /* p included in q */
                if (p->w <= w)
                {
                    pnext = &p->n;
                    goto next;
                }

                top_t *r = pop_rest(prest);
                if (r == NULL)
                    return -EXIT_SOLVE;

                r->x = q->x + q->w;
                r->y = p->y;
                r->w = p->w - w;
                debug("r: x=%d y=%d w=%d\n", r->x, r->y, r->w);
                insert_top(&p->n, r);
                goto next;
            }
        }
        else /* q->x == p->x */
        {
            if (q->y < p->y)
            {
                /* q included in p */
                if (q->w <= p->w)
                {
                    *pprev = p;
                    goto next;
                }

                top_t *r = pop_rest(prest);
                if (r == NULL)
                    return -EXIT_SOLVE;

                r->x = p->x + p->w;
                r->y = q->y;
                r->w = q->w - p->w;
                debug("r: x=%d y=%d w=%d\n", r->x, r->y, r->w);
                insert_top(&p->n, r);

                /* q takes over p */
                q->y = p->y;
                q->w = p->w;
                debug("q: x=%d y=%d w=%d\n", q->x, q->y, q->w);
                goto next;
            }
            else if (q->y == p->y)
            {
                /* q absorbs p */
                if (q->x + q->w < p->x + p->w)
                {
                    q->w = p->x + p->w - q->x;
                    debug("q: x=%d y=%d w=%d\n", q->x, q->y, q->w);
                }
                pnext = &p->n;
                goto next;
            }
            else  /* q->y > p->y */
            {
                /* p included in q */
                if (p->w <= w)
                {
                    pnext = &p->n;
                    goto next;
                }

                top_t *r = pop_rest(prest);
                if (r == NULL)
                    return -EXIT_SOLVE;

                r->x = q->x + q->w;
                r->y = p->y;
                r->w = p->w - w;
                debug("r: x=%d y=%d w=%d\n", r->x, r->y, r->w);
                insert_top(&p->n, r);
                goto next;
            }
        }
next:
        pnext = &p->n;
    }
    if (pprev)
    {
        q = *pprev;
        q->n = NULL;
    }

    return cnt;
}

/*
 * Local Variables:
 *   c-file-style: "stroustrup"
 *   indent-tabs-mode: nil
 * End:
 *
 * vim: set ai cindent et sta sw=4:
 */
