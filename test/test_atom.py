import os, sys
builddir = 'build/lib.linux-x86_64-2.7'
sep = os.path.sep
prev = sep.join(os.path.abspath(os.path.curdir).split(sep)[:-1])
sys.path.append(prev)
sys.path.append(sep.join([prev,builddir]))

from unittest import TestCase
from error import InvalidAtom
from atom import atom
from functools import partial

valid_atoms = [
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

# blockers for versioned/unversioned atoms
"!!=hello/world-4.10",
"!=hello/world-4.10",
"!hello/world",
"!!hello/world",
]

invalid_atoms = [
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

# blockers for versioned/unversioned atoms
"!!hello/world-4.10",
"!hello/world-4.10",
"!=hello/world",
"!!=hello/world",
]

class TestAtom(TestCase):

    def test_random_batch(self):
        for atom_str in invalid_atoms:
            print "%50s <-- invalid" % atom_str
            self.assertRaises(InvalidAtom, atom, atom_str)

        for atom_str in valid_atoms:
            print "%50s < -- valid" % atom_str
            self.assertIsInstance(atom(atom_str), atom)

    def test_eapi0(self):
        atom_base = "=cat/pkg-4.1"
        for postfix in (':1', ':asdf', '[x]', '[x,y]',
                        ':1[x,y]', '[x,y]:1', ':1::repo'):
            atom_str = atom_base + postfix
            self.assertRaises(InvalidAtom, atom, atom_str, eapi=0)
        self.check_use(0)

    def test_eapi1(self):
        atom_base = "=cat/pkg-4.1"
        for postfix in ('[x]', '[x,y]', ':1[x,y]', '[x,y]:1', ':1:repo'):
            atom_str = atom_base + postfix
            self.assertRaises(InvalidAtom, atom, atom_str, eapi=1)
        self.check_use(1)

    def test_eapi2(self):
        self.check_use(2)

    def test_eapi3(self):
        self.check_use(3)
        atom("dev-util/foon:1", eapi=3)
        atom("dev-util/foon:2", eapi=3)
        atom("!dev-util/foon:1", eapi=3)
        atom("dev-util/foon:1[x]", eapi=3)
        atom("dev-util/foon:1[x?]", eapi=3)

    def test_eapi4(self):
        self.check_use(4)
    def test_eapi5(self):
        self.check_use(5)
    def test_eapi6(self):
        self.check_use(6)


    def check_use(self, eapi):
        atom_str = "dev-util/bsdiff"
        kls = partial(atom, eapi=eapi)
        if eapi >= 2:
            # Valid chars: [a-zA-Z0-9_@+-]
            kls('%s[zZaA09]' % atom_str)
            kls('%s[x@y]' % atom_str)
            kls('%s[x+y]' % atom_str)
            kls('%s[x-y]' % atom_str)
            kls('%s[x_y]' % atom_str)
            kls('%s[-x_y]' % atom_str)
            kls('%s[x?]' % atom_str)
            kls('%s[!x?]' % atom_str)
            kls('%s[x=]' % atom_str)
            kls('%s[!x=]' % atom_str)

            # '.' not a valid char in use deps
            self.assertRaises(InvalidAtom, kls, "%s[x.y]" % atom_str)

            # Use deps start with an alphanumeric char (non-transitive)
            self.assertRaises(InvalidAtom, kls, "%s[@x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[_x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[+x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[-@x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[-_x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[-+x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[--x]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[]" % atom_str)
            self.assertRaises(InvalidAtom, kls, "%s[-]" % atom_str)

            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[foon")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[[fo]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x][y]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x]:1")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x]a")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[--]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x??]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x=?]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x?=]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x==]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[x??]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[!=]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[!?]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[!!x?]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[!-x?]")
        else:
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[foo]")
            self.assertRaises(InvalidAtom, kls, "dev-util/diffball[bar]")

        if eapi >= 4:
            kls('%s[x(+)]' % atom_str)
            kls('%s[x(-)]' % atom_str)
            kls("%s[debug(+)]" % atom_str)
            kls("%s[debug(-)]" % atom_str)
            kls("%s[missing(+)]" % atom_str)
            kls("%s[missing(-)]" % atom_str)
            kls("%s[missing(+)]" % atom_str)
            kls("%s[-missing(-)]" % atom_str)
            kls("%s[-missing(+)]" % atom_str)
            kls("%s[-missing(-),debug]" % atom_str)
            kls("%s[-missing(+),debug(+)]" % atom_str)
            kls("%s[missing(+),debug(+)]" % atom_str)
            self.assertRaises(InvalidAtom, kls, '%s[x(+-)]' % atom_str)
            self.assertRaises(InvalidAtom, kls, '%s[x(@)]' % atom_str)
        else:
            self.assertRaises(InvalidAtom, kls, '%s[x(+)]' % atom_str)
            self.assertRaises(InvalidAtom, kls, '%s[x(-)]' % atom_str)
