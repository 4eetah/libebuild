#include "common.h"

int main(int argc, char **argv)
{
    const char *c1 = "hello/world-4.2.1a_beta100_pre200-r300";
    const char *c2 = "hello/world-4.2.1ab_beta100_pre200-r300";
    const char *c3 = "world-4.2.1a_-r300";
    const char *c4 = "hello/world-4.10";
    const char *c5 = "hello/world";
    const char *c7 = "hello/world-4.10a";
    const char *c8 = "hello/world-4";
    const char *c9 = "hello-world/";
    const char *c10 = "/hello-world";

    CPV *cpv1 = cpv_alloc(c1, 1);
    CPV *cpv2 = cpv_alloc(c2, 1);
    CPV *cpv3 = cpv_alloc(c3, 1);
    CPV *cpv4 = cpv_alloc(c4, 1);
    CPV *cpv5 = cpv_alloc(c5, 1);
    CPV *cpv6 = cpv_alloc(c5, 0);
    CPV *cpv7 = cpv_alloc(c7, 1);
    CPV *cpv8 = cpv_alloc(c8, 1);
    CPV *cpv9 = cpv_alloc(c9, 0);
    CPV *cpv10 = cpv_alloc(c10, 0);

    cpv_print(cpv1);
    printf("***\n");
    cpv_print(cpv2);
    printf("***\n");
    cpv_print(cpv3);
    printf("***\n");
    cpv_print(cpv4);
    printf("***\n");
    cpv_print(cpv5);
    printf("***\n");
    cpv_print(cpv6);
    printf("***\n");
    cpv_print(cpv7);
    printf("***\n");
    cpv_print(cpv8);
    printf("***\n");
    cpv_print(cpv9);
    cpv_print(cpv10);

    cpv_free(cpv1);
    cpv_free(cpv2);
    cpv_free(cpv3);
    cpv_free(cpv4);
    cpv_free(cpv5);
    cpv_free(cpv6);
    cpv_free(cpv7);
    cpv_free(cpv8);
    cpv_free(cpv9);
    cpv_free(cpv10);
}
