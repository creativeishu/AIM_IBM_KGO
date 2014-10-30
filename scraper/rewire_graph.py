#!/usr/bin/env python
import sys,json

from scrape_tools import *

def main(args):
    edge_file = args[0]
    prop_file = args[1]


    #list here :        bad id      good id
    ids_to_replace = [("/m/02kctdj" ,"/m/025rsfk"),
                      ("/m/0bnvbd4",  "/m/025rw19" ),
                      ("/m/02kcv_c",  "/m/025rw19" ),
                      ("/m/0bnv6p5",  "/m/025svlc" ),
                      ("/m/02kcq04",  "/m/025s7j4" ),
                      ("/m/02kcqln",  "/m/025sf9q" ),
                      ("/m/063z0gk",  "/m/05_f5y5" ),
                      ("/m/0640s9v",  "/m/05_f5zh" ),
                      ("/m/08rbl0" ,  "/m/05_f5wn" ),
                      ("/m/0ggk182",  "/m/0838f"   ),
                      ("/m/02kcq1_",  "/m/025sf0_" ),
                      ("/m/0ggk3fn",  "/m/06x4c"   ),
                      ("/m/02kcr3f",  "/m/025tkrf" ),
                      ("/m/02kcjvf",  "/m/025tkqy" )]
    ids_to_replace = []

    edges = json.load(open(edge_file))
    props = json.load(open(prop_file))
    assert (type(edges)==list)    
    assert (type(props)==dict)    

    for x in props:
        if len( props[x]['name']) > 25:
            props[x]['name'] = props[x]['name'][0:10]+"..."
        print props[x]['name']

    all_names = [props[x]['name'] for x in props]
    all_names_unique = set(all_names)
    for name in all_names_unique:
        dupl_names = []
        for k,prop in props.items():
            if prop['name']==name:
                dupl_names.append(k)
        if len(dupl_names)>1:
            root_prop = dupl_names[0]
            print len(props[root_prop].keys()),props[root_prop].keys()
            for x in dupl_names[1:]:
                z = dict(props[root_prop].items() + props[x].items())
                props[root_prop] = z
                ids_to_replace.append((x,root_prop))
            print len(props[root_prop].keys()),props[root_prop].keys()
            print "===="

    bad_ids = [x[0] for x in ids_to_replace]

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
