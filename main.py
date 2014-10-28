#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import wikipedia as wp


def main(args):
    print "#getting periodic table"
    
    output_to_file("test.txt",[["s","p","o"], ["x","y","z"]])

    periodic_table = wp.page("Category:Chemical element")

    print periodic_table.url

    

def output_to_file(filename, datalist):
    outfile = open(filename, 'w')

    outfile.write("[\n")
    for line in datalist:
        outfile.write("[ \"{0:s}\" , \"{1:s}\" , \"{2:s}\" ]\n".format(line[0],line[1],line[2]))

    outfile.write("]\n")

    outfile.close()

main(sys.argv[1:])
