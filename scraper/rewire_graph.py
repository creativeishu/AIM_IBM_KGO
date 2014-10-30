#!/usr/bin/env python
import sys,json

from scrape_tools import *

def main(args):
    edge_file = args[0]
    prop_file = args[1]

    #list here :        bad id      good id
    ids_to_replace = [('/m/02kcdj',           ),
                                               ]
    bad_ids = [x[0] for x in ids_to_replace]

    edges = json.load(open(edge_file))
    props = json.load(open(props_file))

    for edge in edges:
        for f,r in ids_to_replace:
            if edge[0]==f:
                edge[0]=r
            if edge[2]==f:
                edge[2]=r

    for my_id in bad_ids:
        props.pop(my_id,None)

    json.dump(props, open('fixed_props.txt','w'))
    json.dump(edges, open('fixed_edges.txt','w'))

def chunks(l, n):
    """ Yield successive n-sized chunks from l.
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

main(sys.argv[1:])
