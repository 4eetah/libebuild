import os, sys
builddir = 'build/lib.linux-x86_64-2.7'
sep = os.path.sep
prev = sep.join(os.path.abspath(os.path.curdir).split(sep)[:-1])
sys.path.append(prev)
sys.path.append(sep.join([prev,builddir]))

from unittest import TestCase
from error import InvalidAtom
from atom import atom

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
]

class ParsingTest(TestCase):
    def test_random_batch(self):
        for atom_str in invalid_atoms:
            self.assertRaises(InvalidAtom, atom, atom_str)

        for atom_str in valid_atoms:
            self.assertIsInstance(atom(atom_str), atom)
