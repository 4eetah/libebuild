import os, sys
builddir = 'build/lib.linux-x86_64-2.7'
sep = os.path.sep
prev = sep.join(os.path.abspath(os.path.curdir).split(sep)[:-1])
sys.path.append(prev)
sys.path.append(sep.join([prev,builddir]))

from unittest import TestCase
from error import InvalidCPV
from cpv import cpv

class ParsingTest(TestCase):
    def test_random_batch(self):
        self.assertRaises(InvalidCPV, cpv, "hello/world-4-10")
        self.assertRaises(InvalidCPV, cpv, "hello/world-4.")
        self.assertRaises(InvalidCPV, cpv, "hello/world-r10")
        self.assertRaises(InvalidCPV, cpv, "hello/world-4.10_gamma10")
        self.assertRaises(InvalidCPV, cpv, "hello/world-4.0-4.0")
        self.assertRaises(InvalidCPV, cpv, "hello/world-4ab")
        self.assertRaises(InvalidCPV, cpv, "hello/world-4.10", versioned=False)
        self.assertRaises(InvalidCPV, cpv, "hello/world", versioned=True)
        self.assertRaises(TypeError, cpv, "hello/world-4.10", keyword=True)
        self.assertIsInstance(cpv("sys-apps/portage-2.1a_alpha100_pre_p10-r100"), cpv)
