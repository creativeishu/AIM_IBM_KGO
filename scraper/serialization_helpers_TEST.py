#!/usr/bin/env python2
# vim: set ts=4 sw=4

import unittest
import tempfile
import os

from serialization_helpers import *

class TestEdgeFile(unittest.TestCase):

    def test_no_entries(self):
        (fileno, filename) = tempfile.mkstemp()
        os.close(fileno)
        
        with EdgeFile(filename, True) as ef:
            pass
        d = json.load(open(filename, 'r'))
        os.unlink(filename)

    def test_simple_entries(self):
        (fileno, filename) = tempfile.mkstemp()
        os.close(fileno)
        
        with EdgeFile(filename, True) as ef:
            ef.add_entry("Iron", "is similar to", "Copper")
            ef.add_entry("Copper", "is similar to", "Iron")

        d = json.load(open(filename, 'r'))
        
        self.assertEqual(d[0][0], "Iron")
        self.assertEqual(d[0][1], "is similar to")
        self.assertEqual(d[0][2], "Copper")
        self.assertEqual(d[1][0], "Copper")
        self.assertEqual(d[1][1], "is similar to")
        self.assertEqual(d[1][2], "Iron")

        os.unlink(filename)

class TestPropertiesFile(unittest.TestCase):

    def test_no_entries(self):
        (fileno, filename) = tempfile.mkstemp()
        os.close(fileno)
        
        with PropertiesFile(filename, True) as pf:
            pass
        d = json.load(open(filename, 'r'))
        os.unlink(filename)

    def test_simple_entries(self):
        (fileno, filename) = tempfile.mkstemp()
        os.close(fileno)
        
        with PropertiesFile(filename, True) as pf:
            pf.add_entry("Iron", { "Melting temperature": 1234 })
            pf.add_entry("Copper", { "Density": 12 })

        d = json.load(open(filename, 'r'))
        
        self.assertEqual(d["Iron"]["Melting temperature"], 1234)
        self.assertEqual(d["Copper"]["Density"], 12)

        os.unlink(filename)

if __name__ == '__main__':
    unittest.main()
