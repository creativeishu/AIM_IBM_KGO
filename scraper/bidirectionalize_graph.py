#!/usr/bin/env python
import sys,json

from scrape_tools import *

def main(args):
    edge_file = args[0]
    prop_file = args[1]


    #list here :        bad id      good id
    edges_to_bidirectionalize = {
            "": "" 
            }

    edges = json.load(open(edge_file))
    props = json.load(open(prop_file))
    assert (type(edges)==list)    
    assert (type(props)==dict)    


    for edge in edges:
        if edge[1] in edges_to_bidirectionalize.keys():
            if not ([edge[2],edges_to_bidirectionalize[edge[1]],edge[0]) in edges:
                edges.append([edge[2],edges_to_bidirectionalize[edge[1]],edge[0])

    json.dump(edges, open('bidir_edges.txt','w'),indent=2)
    json.dump(props, open('bidir_props.txt','w'),indent=2)

def chunks(l, n):
    """ Yield successive n-sized chunks from l.
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

main(sys.argv[1:])