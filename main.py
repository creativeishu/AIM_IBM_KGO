#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import pywikibot as pwb
from   pywikibot import pagegenerators


def main(args):
    print "#getting periodic table"
    
    site = pwb.Site()
    pt_category = pwb.Category(site,'Category:Chemical elements')

    elements_gen = pwb.pagegenerators.CategorizedPageGenerator(pt_category)

    for el in  elements_gen:
        print el.title()

main(sys.argv[1:])
