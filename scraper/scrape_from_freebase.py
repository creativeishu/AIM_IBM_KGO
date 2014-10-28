#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import urllib
from serialization_helpers import *

def main(args):

    stopping_count = 100


    pf=PropertiesFile("props.txt", True)
    ef=EdgeFile("edges.txt", True)

    cursor = 1
    while(cursor<stopping_count):
        hits,cursor, data = get_result_bunch(cursor)
        if (cursor==1):
            print "Found {0:6d} hits".format(hits)
        data_to_files(data,pf,ef)



def get_result_bunch(cursor):
    """execute a query with the specified cursor [=kindof offset in the database]"""

    print "   #getting bunch of results {0:8d}".format(cursor)
    service_url = 'https://www.googleapis.com/freebase/v1/search'
    params = {
       "domain": 'chemistry',
        "output" : "(all)",
        "limit" : "10",
        "key" : "AIzaSyCgJBUnif7NrPqwY_loGQXo9OGaPPHTjA0",
        "cursor" : cursor
     }
    url = service_url + '?' + urllib.urlencode(params)
    response = json.loads(urllib.urlopen(url).read())
    assert ('result' in response)
    return  response['hits'], response['cursor'],response['result']


def data_to_files(data, pf, ef):
    """take the json result and print it to the two files"""

    for p in data:
        if 'output' in p and 'all' in p['output']:
            for k,v in p['output']['all'].items():
                nodeprops={}
                for vi in v:
                    if type(vi) == dict and 'mid' in vi:
                        ef.add_entry(p['mid'],k,vi['mid'])
                        #print p['mid'], '<%s>'%k, vi['mid'] # links
                    else:
                        #print p['mid'], '<%s>'%k, vi # node properties
                        nodeprops['k'] = vi
                pf.add_entry(p['mid'],nodeprops)

main(sys.argv[1:])
