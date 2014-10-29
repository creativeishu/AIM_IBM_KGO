#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import urllib
from serialization_helpers import *

def main(args):

    stopping_count = 19000


    pf=PropertiesFile("props.txt", True)
    ef=EdgeFile("edges.txt", True)

    cursor = ""
    #while(cursor<stopping_count):
    cursor, data = get_result_bunch(cursor)
    for x in data[0].keys():
        print data[0][x]
#    print data[0].keys()
    data_to_files(data,pf,ef)



def get_result_bunch(cursor):
    """execute a query with the specified cursor [=kindof offset in the database]"""

    print "   #getting bunch of results {0:8s}".format(cursor)
    service_url = 'https://www.googleapis.com/freebase/v1/mqlread'
    query = [{
       #"type": {'key': {"namespace": "/chemistry",'limit':'1'},'limit':'1'},
       #"output" : "(all:/measurement)",
       # "limit" : "8",
       # "id": None,
       #"name": "Malaria",
         "*": [{}],
         "type": {
           "key": {
           "namespace": "/chemistry",
           "limit": 1
           },
         "limit": 1
         },
         "limit": 3
        }]

    params = {
        "query" :     json.dumps(query),
        "key" : "AIzaSyCgJBUnif7NrPqwY_loGQXo9OGaPPHTjA0",
        "cursor" : str(cursor),
        "limit": "10"
        }
    print "#", params 
    url = service_url + '?' + urllib.urlencode(params)
    print "#", url
    response = json.loads(urllib.urlopen(url).read())
    assert ('result' in response)
    #if ('result' in response):
    return  response['cursor'],response['result']
   #elif 'error' in response:
   #    print response['error']['message']
   #    return  cursor,[]



def data_to_files(data, pf, ef):
    """take the json result and print it to the two files"""
    key_exclusion_list = [
            "url./common/topic/topic_equivalent_webpage" ,   #just crap translations of the webpage
                        ]
    print len(data)
    for p in data:
        nodeprops={}
        if 'output' in p and 'all' in p['output']:
            for k,v in p['output']['all'].items():
                if k in key_exclusion_list: continue  #skip excluded keys
                for vi in v:
                    if type(vi) == dict and 'mid' in vi:
                        ef.add_entry(p['mid'],k,vi['mid'])
                        #print p['mid'], '<%s>'%k, vi['mid'] # links
                    else:
                        #print p['mid'], '<%s>'%k, vi # node properties
 #                      print k
                        nodeprops[k] = vi
        pf.add_entry(p['mid'],nodeprops)

main(sys.argv[1:])
