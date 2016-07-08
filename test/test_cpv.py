import os, sys
sep = os.path.sep
prev = sep.join(os.path.abspath(os.path.curdir).split(sep)[:-1])
sys.path.append(prev)
from unittest import TestCase
from libebuild.cpv import cpv
from pkgcore.ebuild.errors import InvalidCPV

class MiscTest(TestCase):
    def test_parse_batch(self):
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

    def test_compare(self):
        self.assertEqual(cpv("hello/world-4.10"), cpv("hello/world-4.10"))
        self.assertEqual(cpv("hello/world-4.0010"), cpv("hello/world-4.001"))
        self.assertNotEqual(cpv("allo/world-4.10"), cpv("hello/world-4.10"))
        self.assertLess(cpv("hello/world-4.8"), cpv("hello/world-4.10"))
        self.assertNotEqual(cpv("hello/xorld-4.8"), cpv("hello/world-4.10"))
        self.assertGreater(cpv("hello/world-4.10"), cpv("hello/world-4.8"))
