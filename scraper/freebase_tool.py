#!/usr/bin/env python
import sys,json

from scrape_tools import *

def main(args):
    edge_file = args[0]
    prop_file = args[1]

    #comment out to skip vvvv
    recompute_nodes_without_props(edge_file, prop_file)
    quit()
    #open nodes without props
    with open('nodes_without_props.txt', 'r') as infile:
        nodes_without_props = json.load(infile)
        
    edges,props = find_and_add_props(nodes_without_props, edge_file, prop_file)

    for x in props.keys():
        print x

    json.dump(edges, open('edges_new.txt','w'))
    json.dump(props, open('props_new.txt','w'))

def find_and_add_props(nodes_without_props,edge_file, prop_file):
    #### Retrieving additional properties and back-linking edges
    #now that we have all the nodes that don't have properties, we can go through and get the data.
    edges = json.load(open(edge_file,'r'))
    props = json.load(open(prop_file,'r'))

    allnodes = []
    for x in edges:
        allnodes.append(x[0])
        allnodes.append(x[2])

    mynodes = set(allnodes)    
    allnodes = mynodes

    with PropertiesFile("additional_props.txt", True) as pf, EdgeFile("additional_edges.txt", True) as ef:
        for nodechunk in chunks(nodes_without_props,90):
            #print "getting for ", nodechunk[0],"...",nodechunk[-1]
            data = get_topic_for_id(nodechunk)
            if data is not None:
                data_to_files(data,pf,ef,target_node_inclusion=nodes_without_props)


    extra_edges = json.load(open("additional_edges.txt"))
    extra_props = json.load(open("additional_props.txt"))

    for i in extra_props.keys():
        if not i in props.keys():
            props[i] = extra_props[i]
    
    for nen in extra_edges:
        if nen[0] in allnodes and nen[2] in allnodes:
            edges.append(nen)

    return edges, props



def recompute_nodes_without_props(edge_file,prop_file):
    edges = json.load(open(edge_file))
    props = json.load(open(prop_file))

    nodes = []
    for x in edges:
        nodes.append(x[0])
        nodes.append(x[2])

    mynodes = set(nodes)    
    nodes = mynodes

    nodes_without_props = []


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

    with open('nodes_without_props.txt', 'w') as outfile:
        json.dump(nodes_without_props, outfile)

def chunks(l, n):
    """ Yield successive n-sized chunks from l.
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

main(sys.argv[1:])
