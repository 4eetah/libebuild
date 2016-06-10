#include "common.h"
#include <assert.h>

// portage test cases for vercmp

typedef struct {
    const char *v1;
    const char *v2;
} vcomp;

vcomp less_test_cases[] = {
    {"4.0", "5.0"},
    {"5", "5.0"},
    {"1.0_pre2", "1.0_p2"},
    {"1.0_alpha2", "1.0_p2"},
    {"1.0_alpha1", "1.0_beta1"},
    {"1.0_beta3", "1.0_rc3"},
    {"1.001000000000001", "1.001000000000002"},
    {"1.00100000000", "1.0010000000000000001"},
    {"999999999999999998", "999999999999999999"},
    {"1.01", "1.1"},
    {"1.0-r0", "1.0-r1"},
    {"1.0", "1.0-r1"},
    {"1.0", "1.0.0"},
    {"1.0b", "1.0.0"},
    {"1_p1", "1b_p1"},
    {"1", "1b"},
    {"1.1", "1.1b"},
    {"12.2b", "12.2.5"},
    {"1", "2"},
    {"1.0_alpha", "1.0_pre"},
    {"0", "0.0"},
    {"1.0-r0", "1.0-r1"},
    {"1.0", "1.0-r1"},
    {"1.0", "1.0.0"},
    {"1_p1", "1b_p1"},
};

vcomp greater_test_cases[] = {
    {"6.0", "5.0"},
    {"5.0", "5"},
    {"1.0-r1", "1.0-r0"},
    {"1.0-r1", "1.0"},
    {"999999999999999999", "999999999999999998"},
    {"1.0.0", "1.0"},
    {"1.0.0", "1.0b"},
    {"1b", "1"},
    {"1b_p1", "1_p1"},
    {"1.1b", "1.1"},
    {"12.2.5", "12.2b"},
    {"1.0_beta", "1.0_alpha"},
    {"1.0-r1", "1.0-r0"},
    {"1.0-r1", "1.0"},
    {"1b", "1"},
    {"1.1b", "1.1"},
    {"12.2b", "12.2"},
    {"001.01", "001.001"},
};

vcomp equal_test_cases[] = {
    {"4.0", "4.0"},
    {"1.0", "1.0"},
    {"1.0-r0", "1.0"},
    {"1.0", "1.0-r0"},
    {"1.0-r0", "1.0-r0"},
    {"1.0-r1", "1.0-r1"},
    {"000001", "01"},
    {"001", "1"},
    {"001.001", "1.001"},
};

int main(int argc, char **argv)
{
    if (argc == 3) {
        assert(isvalid_version(argv[1]) && "got invalid version");
        assert(isvalid_version(argv[2]) && "got invalid version");
        int ret = version_cmp(argv[1], argv[2]);
        printf("%s ", argv[1]);
        if (ret < 0)
            printf("<");
        else if (ret > 0)
            printf(">");
        else
            printf("==");
        printf(" %s\n", argv[2]);
        return 0;
    }
    
    int len, i;
    len = sizeof(less_test_cases) / sizeof(*less_test_cases);
    for (i = 0; i < len; ++i) {
        printf("%s < %s\n", less_test_cases[i].v1, less_test_cases[i].v2);
        assert(isvalid_version(less_test_cases[i].v1) && "got invalid version");
        assert(isvalid_version(less_test_cases[i].v2) && "got invalid version");
        assert((version_cmp(less_test_cases[i].v1, less_test_cases[i].v2) < 0) &&
                "should be less than");
    }

    len = sizeof(greater_test_cases) / sizeof(*greater_test_cases);
    for (i = 0; i < len; ++i) {
        printf("%s > %s\n", greater_test_cases[i].v1, greater_test_cases[i].v2);
        assert(isvalid_version(less_test_cases[i].v1) && "got invalid version");
        assert(isvalid_version(less_test_cases[i].v2) && "got invalid version");
        assert((version_cmp(greater_test_cases[i].v1, greater_test_cases[i].v2) > 0) &&
                "should be greater than");
    }

    len = sizeof(equal_test_cases) / sizeof(*equal_test_cases);
    for (i = 0; i < len; ++i) {
        printf("%s == %s\n", equal_test_cases[i].v1, equal_test_cases[i].v2);
        assert(isvalid_version(less_test_cases[i].v1) && "got invalid version");
        assert(isvalid_version(less_test_cases[i].v2) && "got invalid version");
        assert((version_cmp(equal_test_cases[i].v1, equal_test_cases[i].v2) == 0) &&
                "should be equal");
    }
}
