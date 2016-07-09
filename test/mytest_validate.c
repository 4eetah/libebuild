#include "elib.h"

const char * const my_suffix[] = {"alpha", "beta", "pre", "rc", "nonexistent", "", "p"};

int main(int argc, char **argv)
{
    int i;
    int len = 7;
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

    printf("isvalid_version(%s) = %d\n", v1, isvalid_version(v1));
    printf("isvalid_version(%s) = %d\n", v2, isvalid_version(v2));
    printf("isvalid_version(%s) = %d\n", v3, isvalid_version(v3));
    printf("isvalid_version(%s) = %d\n", v4, isvalid_version(v4));
    printf("isvalid_version(%s) = %d\n", v5, isvalid_version(v5));
    printf("isvalid_version(%s) = %d\n", v6, isvalid_version(v6));
    printf("isvalid_version(%s) = %d\n", v7, isvalid_version(v7));
    printf("isvalid_version(%s) = %d\n", v8, isvalid_version(v8));
    printf("isvalid_version(%s) = %d\n", v9, isvalid_version(v8));
}
