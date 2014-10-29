#!/usr/bin/env python
import sys,json

from scrape_tools import *

def main(args):
    node_file = args[0]
    prop_file = args[1]
    
    n_data = open(node_file)
    p_data = open(prop_file)

    nodes_without_props = []

    nodes = json.load(n_data)
    props = json.load(p_data)
    noname = 0
    noprop = 0
    total = 0
    for n in nodes:
        total +=1
        if n in props.keys():
            print "Node {0:12s} has property data ".format(n),
            if 'name' in props[n].keys():
                print "[name = {0:20s}]".format(props[n]['name'].encode("utf8"))
            else:
                print " but NO name property".format(props[n]['name'].encode("utf8"))
                noname +=1
        else:
            noprop += 1
            print "!!!! Node {0:12s} has no property data [number {1:5d}/{2:-5d}]".format(n,noprop,total)
            nodes_without_props.append(n)


    with PropertiesFile("additional_props.txt", True) as pf, EdgeFile("additional_edges.txt", True) as ef:
        for node_id in nodes_without_props:
            data = get_topic_for_id(node_id)
            if data is not None:
                data_to_files(data,pf,ef,target_node_inclusion=nodes_without_props)


    n_data.close()
    p_data.close()


main(sys.argv[1:])
