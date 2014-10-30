#!/usr/bin/env python
import sys
import json

def main(args):
    graph = json.load(open(args[0]))

    print "graph G{"
    for line in graph:
        print "{0:s} -- {1:s};".format(line[0].replace('/','__'), line[2].replace('/','__'))
    
    print "}"

main(sys.argv[1:])
