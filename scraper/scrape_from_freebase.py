#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import urllib
from serialization_helpers import *
from scrape_tools import *

def main(args):

    stopping_count = 400
    runtime = 0


    with PropertiesFile("props.txt", True) as pf, EdgeFile("edges.txt", True) as ef:

        cursor = ""
        while(runtime<stopping_count):
            #get all the id's
            cursor, data = get_result_bunch(cursor)
            topic_ids = [x['id'][0] for x in data]

            print "getting data for topics", runtime*150
            data_per_topic = get_topic_for_id(topic_ids)
            if data_per_topic is not None:
                data_to_files(data_per_topic,pf,ef)
            runtime +=1

main(sys.argv[1:])
