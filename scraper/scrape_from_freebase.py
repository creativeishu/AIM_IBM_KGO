#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import urllib
from serialization_helpers import *

def main(args):

    stopping_count = 400
    runtime = 0


    pf=PropertiesFile("props.txt", True)
    ef=EdgeFile("edges.txt", True)

    cursor = ""
    while(runtime<stopping_count):
        #get all the id's
        cursor, data = get_result_bunch(cursor)
        topic_ids = [x['id'][0] for x in data]

        print "getting data for topics", runtime
        data_per_topic = get_topic_for_id(topic_ids)
        if data_per_topic is not None:
            data_to_files(data_per_topic,pf,ef)
        runtime +=1



def get_result_bunch(cursor):
    """execute a query with the specified cursor [=kindof offset in the database]"""

    service_url = 'https://www.googleapis.com/freebase/v1/mqlread'
    query = [{
        "id": [],
        "mid": [],
       #"name": "Malaria",
       #  "*": [{}],
         #"topic": [{}],
         #"/common/topic/notable_for": [{}],
         "type": {
           "key": { "namespace": "/chemistry", "limit": 1 },
           "*":[],
         "limit": 1
         },
         "limit": 100 
        }]

    params = {
        "query" :     json.dumps(query),
        "key" : "AIzaSyCgJBUnif7NrPqwY_loGQXo9OGaPPHTjA0",
        "cursor" : str(cursor),
        "limit": "10"
        }
    #print "#", params 
    url = service_url + '?' + urllib.urlencode(params)
    #print "#", url
    response = json.loads(urllib.urlopen(url).read())
    #assert ('result' in response)
    if ('result' in response):
        return  response['cursor'],response['result']
    else:
        print response
        quit()
   #elif 'error' in response:
   #    print response['error']['message']
   #    return  cursor,[]



def data_to_files(data, pf, ef):
    """take the json result and print it to the two files"""
    key_exclusion_list = [
            "url./common/topic/topic_equivalent_webpage" ,   #just crap translations of the webpage
                        ]
    for p in data:
        if 'name' in p:
            nodeprops={'name':p['name']}
        else:
            nodeprops={}
        if 'output' in p and 'all' in p['output']:
            for k,v in p['output']['all'].items():
                if k in key_exclusion_list: continue  #skip excluded keys
                for vi in v:
                    if type(vi) == dict and 'mid' in vi:
                        ef.add_entry(p['mid'],k,vi['mid'])
                   #elif type(vi) == list:
                   #    for sublist in vi:
                   #        if type(sublist) == dict and 'mid' in vi:
                   #            ef.add_entry(p['mid'],k,vi['mid'])
                    else:
                        nodeprops[k] = vi
        pf.add_entry(p['mid'],nodeprops)

def get_topic_for_id(concept_id):
    """execute a query with the specified cursor [=kindof offset in the database]"""

    service_url = 'https://www.googleapis.com/freebase/v1/search'
    params = {
        #"id": concept_id,
        "filter" : "(any " + " ".join(["id:" + x for x in concept_id]) + ")",
        "output": "(all)",
        "key" : "AIzaSyCgJBUnif7NrPqwY_loGQXo9OGaPPHTjA0",
        "limit" : 100,
     }
    url = service_url + '?' + urllib.urlencode(params)
    response = json.loads(urllib.urlopen(url).read())

    if ('result' in response):
        return  response['result']
    elif 'error' in response:
        print response['error']['message']
        return  None
    else:
        print response
        quit()
main(sys.argv[1:])
