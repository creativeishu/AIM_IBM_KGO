#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import wikipedia as wp


def main(args):
    print "#getting periodic table"
    
    periodic_table = wp.page("List of Elements")
    print periodic_table.title
    print periodic_table.links

main(sys.argv[1:])
