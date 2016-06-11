#include "common.h"
#include <assert.h>
#include <getopt.h>

const char *valid_cpv_vers_str[] = {
"hello/world-4.2.1a_beta100_pre200-r300",
"hello/world-4.10",
"hello/world-4.10a",
"hello/world-4",
};

const char *invalid_cpv_vers_str[] = {
"hello/world-4.2.1ab_beta100_pre200-r300",
"world-4.2.1a_-r300",
"hello/world",
"hello-world/",
"/hello-world",
};

size_t max_len(const char **s, size_t size)
{
    int i;
    size_t max = 0;
    for (i = 0; i < size; ++i)
        if (max < strlen(s[i]))
            max = strlen(s[i]);
    return max;
}

static int optret;
static struct option cpv_long_opts[] = {
    {"versioned", no_argument, &optret, 'v'},
    {"unversioned", no_argument, &optret, 'u'},
    {"help", no_argument, &optret, 'h'},
    {0, 0, 0, 0}
};

int main(int argc, char **argv)
{
    CPV *cpv;
    int versioned = 1;
    int c, index;
    c = getopt_long(argc, argv, "", cpv_long_opts, &index);

    if (!c || argc == 2) {
        switch (optret) {
        case 'v':
            versioned = 1;
            break;
        case 'u':
            versioned = 0;
            break;
        case 'h':
        case '?':
            printf("Usage: ./mytest_cpv [--versioned | --unversioned] [cpv]\n");
            exit(1);
        }
        printf("%s ", argv[optind]);
        cpv = cpv_alloc(argv[optind], versioned);
        if (cpv)
            printf("<--- valid\n");
        else
            printf("<--- invalid\n");
        cpv_print(cpv);
        cpv_free(cpv);
        return 0;
    }
    
    int i, len, width;
    len = sizeof(valid_cpv_vers_str) / sizeof(valid_cpv_vers_str[0]);
    width = max_len(valid_cpv_vers_str, len);
    for (i = 0; i < len; ++i) {
        cpv = cpv_alloc(valid_cpv_vers_str[i], versioned);
        printf("%-*s ", width, valid_cpv_vers_str[i]);
        assert(cpv != NULL && "invalid cpv! should be valid");
        printf("<--- valid\n");
        cpv_free(cpv);
    }

    len = sizeof(invalid_cpv_vers_str) / sizeof(invalid_cpv_vers_str[0]);
    width = max_len(invalid_cpv_vers_str, len);
    for (i = 0; i < len; ++i) {
        cpv = cpv_alloc(invalid_cpv_vers_str[i], versioned);
        printf("%-*s ", width, invalid_cpv_vers_str[i]);
        assert(cpv == NULL && "valid atom! should be invalid");
        printf("<--- invalid\n");
        cpv_free(cpv);
    }
}
