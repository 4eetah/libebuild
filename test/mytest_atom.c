#include "common.h"
#include <assert.h>

// portage test cases
const char *valid_atoms_str[] = {
"sys-apps/portage",
"=sys-apps/portage-2.1",
"=sys-apps/portage-2.1*",
">=sys-apps/portage-2.1",
"<=sys-apps/portage-2.1",
">sys-apps/portage-2.1",
"<sys-apps/portage-2.1",
"~sys-apps/portage-2.1",
"sys-apps/portage:foo",
"sys-apps/portage",

"null/portage",
">=null/portage-2.1",
"~null/portage-2.1",
"=null/portage-2.1*",

"=sys-apps/portage-2.2*:foo[bar?,!baz?,!doc=,build=]",
"=sys-apps/portage-2.2*:foo[doc?]",
"=sys-apps/portage-2.2*:foo[!doc?]",
"=sys-apps/portage-2.2*:foo[doc=]",
"=sys-apps/portage-2.2*:foo[!doc=]",
"=sys-apps/portage-2.2*:foo[bar,-baz,doc?,!build?]",
"=sys-apps/portage-2.2*:foo::repo[bar?,!baz?,!doc=,build=]",
"=sys-apps/portage-2.2*:foo::repo[doc?]",

"=foo/bar--baz-1-r1",
"=foo/bar-baz--1-r1",
"=foo/bar-baz---1-r1",
"=foo/bar-baz---1",
"games-strategy/ufo2000",
"~games-strategy/ufo2000-0.1",
"=media-libs/x264-20060810",
"foo/b",
"app-text/7plus",
"foo/666",
"=dev-libs/poppler-qt3-0.11*",

"sys-apps/portage::repo_123-name",
"=sys-apps/portage-2.1::repo",
"=sys-apps/portage-2.1*::repo",
"sys-apps/portage:foo::repo",
"null/portage::repo",

"virtual/ffmpeg:0/53",
"virtual/ffmpeg:0/53=",
"virtual/ffmpeg:=",
"virtual/ffmpeg:0=",
"virtual/ffmpeg:*",
};

const char *invalid_atoms_str[] = {
"sys-apps/portage-2.1:foo",
"sys-apps/portage-2.1:",
"sys-apps/portage-2.1:[foo]",

"=sys-apps/portage-2.2*:foo[!doc]",
"=sys-apps/portage-2.2*:foo[!-doc]",
"=sys-apps/portage-2.2*:foo[!-doc=]",
"=sys-apps/portage-2.2*:foo[!-doc?]",
"=sys-apps/portage-2.2*:foo[-doc?]",
"=sys-apps/portage-2.2*:foo[-doc=]",
"=sys-apps/portage-2.2*:foo[-doc!=]",
"=sys-apps/portage-2.2*:foo[-doc=]",
"=sys-apps/portage-2.2*:foo[bar][-baz][doc?][!build?]",
"=sys-apps/portage-2.2*:foo[bar,-baz,doc?,!build?,]",
"=sys-apps/portage-2.2*:foo[,bar,-baz,doc?,!build?]",
"=sys-apps/portage-2.2*:foo[bar,-baz][doc?,!build?]",
"=sys-apps/portage-2.2*:foo[bar][doc,build]",
"=sys-apps/portage-2.2*:foo::repo[!doc]",


">~cate-gory/foo-1.0",
">~category/foo-1.0",
"<~category/foo-1.0",
"###cat/foo-1.0",
"###cat/foo-1.0::repo",
"~sys-apps/portage",
"~sys-apps/portage::repo",
"portage::repo",
"=portage::repo",

"portage",
"=portage",
">=portage-2.1",
"~portage-2.1",
"=portage-2.1*",

"null/portage*:0",
">=null/portage",
">null/portage",
"=null/portage*",
"=null/portage",
"~null/portage",
"<=null/portage",
"<null/portage",
"null/portage-2.1*",

"app-doc/php-docs-20071125",
"app-doc/php-docs-20071125-r2",
"=foo/bar-1-r1-1-r1",
"foo/-z-1",
"=foo/bar-1-r1-1-r1",
"=foo/bar-123-1",
"=foo/bar-123-1*",
"foo/bar-123",
"=foo/bar-123-1-r1",
"=foo/bar-123-1-r1*",
"foo/bar-123-r1",
"foo/bar-1",
"=foo/bar-baz-1--r1",

"sys-apps/portage-2.1:foo::repo",
"sys-apps/portage-2.1:::repo",
"sys-apps/portage-2.1:::repo[foo]",
"app-doc/php-docs-20071125::repo",
"=foo/bar-1-r1-1-r1::repo",

"virtual/ffmpeg:0/53*",
"virtual/ffmpeg:0*",
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

int main(int argc, char **argv)
{
    ATOM *atom;

    if (argc == 2) {
        printf("%s ", argv[1]);
        atom = atom_alloc(argv[1]);
        if (atom)
            printf("<--- valid\n");
        else
            printf("<--- invalid\n");
        atom_print(atom);
        atom_free(atom);
        return 0;
    }
    
    int i, len, width;
    len = sizeof(valid_atoms_str) / sizeof(valid_atoms_str[0]);
    width = max_len(valid_atoms_str, len);
    for (i = 0; i < len; ++i) {
        atom = atom_alloc(valid_atoms_str[i]);
        printf("%-*s ", width, valid_atoms_str[i]);
        assert(atom != NULL && "invalid atom! should be valid");
        printf("<--- valid\n");
        atom_free(atom);
    }

    len = sizeof(invalid_atoms_str) / sizeof(invalid_atoms_str[0]);
    width = max_len(invalid_atoms_str, len);
    for (i = 0; i < len; ++i) {
        atom = atom_alloc(invalid_atoms_str[i]);
        printf("%-*s ", width, invalid_atoms_str[i]);
        assert(atom == NULL && "valid atom! should be invalid");
        printf("<--- invalid\n");
        atom_free(atom);
    }
}
