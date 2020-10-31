#include <stdio.h>
#include "common.h"

int make_svg(const char *fname, const top_t *ptops, const top_t *pprof)
{
    int err = 0;
    FILE *fp = fopen(fname, "w");
    if (fp == NULL)
    {
        err = -1;
        goto error;
    }
    fprintf(fp, "%s",
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
"<svg\n"
"   xmlns=\"http://www.w3.org/2000/svg\"\n"
"   version=\"1.1\">\n"
"  <defs>\n"
"     <marker id=\"head\" orient=\"auto\" markerUnits=\"strokeWidth\"\n"
"        viewBox=\"0 0 10 10\" refX=\"5\" refY=\"5\"\n"
"        markerWidth=\"5\" markerHeight=\"5\">\n"
"       <circle cx=\"5\" cy=\"5\" r=\"3\" fill=\"red\"/>\n"
"     </marker>\n"
"  </defs>\n"
);

    int h = 0;
    for (const top_t *t = ptops; t; t = t->n)
    {
        if (t->y > h)
            h = t->y;
    }
    for (const top_t *p = pprof; p; p = p->n)
    {
        if (p->y > h)
            h = p->y;
    }

    for (const top_t *t = ptops; t; t = t->n)
    {
        fprintf(fp,
"  <rect\n"
"     fill=\"#000000\"\n"
"     x=\"%d\"\n"
"     y=\"%d\"\n"
"     width=\"%d\"\n"
"     height=\"%d\"\n"
"     stroke=\"#a000a0\"\n"
"     style=\"mix-blend-mode:difference;fill:#00a000;fill-opacity:1\" />\n",
        t->x, h - t->y, t->w, t->y);
    }

    if (pprof)
    {
        int x = 0, y = 0;
        fprintf(fp, "  <polyline\n     points=\"%d,%d", x, h - y);
        for (const top_t *p = pprof; p; p = p->n)
        {
            /* dive */
            if (x != p->x)
            {
                y = 0;
                fprintf(fp, " %d,%d", x, h - y);
                x = p->x;
                fprintf(fp, " %d,%d", x, h - y);
            }

            /* lift */
            if (y != p->y)
            {
                y = p->y;
                fprintf(fp, " %d,%d", x, h - y);
            }

            /* this top */
            x = p->x + p->w;
            y = p->y;
            fprintf(fp, " %d,%d", x, h - y);
        }
        if (y != 0)
        {
            y = 0;
            fprintf(fp, " %d,%d", x, h - y);
        }
        fprintf(fp, "%s",
"\"\n"
"     stroke=\"#f0f000\"\n"
"     style=\"fill:none\"\n"
"     marker-start=\"url(#head)\"\n"
"     marker-mid=\"url(#head)\"\n"
"     marker-end=\"url(#head)\"\n"
"     />\n"
        );
    }
    fprintf(fp, "%s", "</svg>\n");
    fprintf(stderr, "# generated %s\n", fname);
    err = 0;
error:
    if (fp != NULL) fclose(fp);
    return err;
}

/*
 * Local Variables:
 *   c-file-style: "stroustrup"
 *   indent-tabs-mode: nil
 * End:
 *
 * vim: set ai cindent et sta sw=4:
 */
