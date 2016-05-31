#include "common.h"

const char * const my_suffix[] = {"alpha", "beta", "pre", "rc", "/*bogus*/", "p"};
int main(int argc, char **argv)
{
    int i;
    int len = 6;
    printf("len = %d\n", len);
    for (i = 0; i < len; ++i)
        printf("getsuffix(%s) = %d\n", my_suffix[i], getsuffix(my_suffix[i]));

    const char *v1 = "4.2.1_beta100_pre200-r300";
    const char *v2 = "4.2.1a_beta100_pre200-r300";
    const char *v3 = "4.2..1a_beta100_pre200-r300";
    const char *v4 = ".4.2.1a_beta_pre200-r300";
    const char *v5 = ".4.2.1a_beta0_hey";
    const char *v6 = "4.2.1a";
    const char *v7 = "4.0";
    const char *v8 = "4.0-r100d";
    const char *v9 = "4.0_";
    printf("isversion(%s) = %d\n", v1, isversion(v1));
    printf("isversion(%s) = %d\n", v2, isversion(v2));
    printf("isversion(%s) = %d\n", v3, isversion(v3));
    printf("isversion(%s) = %d\n", v4, isversion(v4));
    printf("isversion(%s) = %d\n", v5, isversion(v5));
    printf("isversion(%s) = %d\n", v6, isversion(v6));
    printf("isversion(%s) = %d\n", v7, isversion(v7));
    printf("isversion(%s) = %d\n", v8, isversion(v8));
    printf("isversion(%s) = %d\n", v9, isversion(v8));

    const char *c1 = "hello/world-4.2.1a_beta100_pre200-r300";
    const char *c2 = "hello/world-4.2.1ab_beta100_pre200-r300";
    const char *c3 = "world-4.2.1a_-r300";
    const char *c4 = "hello/world-4.10";
    const char *c5 = "hello/world";
    const char *c7 = "hello/world-4.10a";
    const char *c8 = "hello/world-4";

    CPV *cpv1 = cpv_alloc(c1, 1);
    CPV *cpv2 = cpv_alloc(c2, 1);
    CPV *cpv3 = cpv_alloc(c3, 1);
    CPV *cpv4 = cpv_alloc(c4, 1);
    CPV *cpv5 = cpv_alloc(c5, 1);
    CPV *cpv6 = cpv_alloc(c5, 0);
    CPV *cpv7 = cpv_alloc(c7, 1);
    CPV *cpv8 = cpv_alloc(c8, 1);

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

    cpv_free(cpv1);
    cpv_free(cpv2);
    cpv_free(cpv3);
    cpv_free(cpv4);
    cpv_free(cpv5);
    cpv_free(cpv6);
    cpv_free(cpv7);
    cpv_free(cpv8);

}
