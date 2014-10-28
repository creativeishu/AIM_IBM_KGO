#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import requests
import time
import json
from json import JSONDecoder
from datetime import datetime, timedelta
import wikipedia as wp
from wikipedia.exceptions import *
from serialization_helpers import *


def main(args):
    exclusionlist = ["rticle", "ikiped"     , "language", 
                     "dmy"   , "ansfermium" , "errors"  , 
                     "stub"  , "versit"     , "NPOV"    ,
                     "Dream" , ]
    inclusionlist = ["chem", "Chem", "ompound", "onductor", "oxide", "metal", "Metal", "sotope", 
                     "aterial"]

    print "#getting periodic table"
    
    #output_to_file("test.txt",[["s","p","o"], ["x","y","z"]])
    edge_file_name = "edges.txt"

    #first get the periodic table of elements
    with EdgeFile(edge_file_name, True) as ef:
        periodic_table =  get_contents_for_cat("Chemical elements",ef,inclusionlist=inclusionlist)

#   with EdgeFile(edge_file_name, True) as ef:
#       excluded=False 
#       for el in periodic_table[5:]:
#           element_page = wp.page(el)
#           this_elements_categories = element_page.categories
#           for cat in this_elements_categories:
#               for excl in exclusionlist:
#                   if excl in cat: 
#                       excluded=True
#               if not (excluded or el==cat):
#                   ef.add_entry(el,"is member of", cat)
#                   ef.add_entry(cat,"contains", el)
#               excluded=False


        #now our node-edge-node is
        # element   memberof  this_elements_categories[i]

def get_contents_for_cat(category,ef,rec_level=0,inclusionlist=[],visited_cat=[],visited_pages=[]):
    rec_level +=1
    cat_pages =  get_pages_in_category(category)
    print "ENTERED: ", rec_level, category, cat_pages
    print "ENTERED: ", rec_level, "visited: ", visited_cat
    for el in cat_pages:
        if not el in visited_pages:
            try:
                element_page = wp.page(el)
                visited_pages.append(el)
            except DisambiguationError:
                continue 
            except PageError:
                continue
        else:
            print "! skipped: ", el
            continue

        this_elements_categories = element_page.categories
        print el,
        included=False
        for cat in this_elements_categories:
            for incl in inclusionlist:
                if incl in cat: 
                    included=True
                    continue
            if included==True and not (el==cat or cat in visited_cat):
                ef.add_entry(el,"is member of", cat)
                ef.add_entry(cat,"contains", el)
                visited_cat.append(cat)
                print cat,
                if (rec_level<=3):
                    visited_cat,visited_pages = get_contents_for_cat(cat,ef,rec_level,inclusionlist,visited_cat,visited_pages)
            included=False
    print ""
    return visited_cat,visited_pages


def get_pages_in_category(cat_name):
    query_params = {
      'action': 'query',
      'list': 'categorymembers',
      'cmlimit': '500',
      'cmtitle': "Category:"+cat_name
    }
    res = wiki_request(query_params)
    return [x['title'] for x in res['query']['categorymembers'] if x['ns']==0]


def wiki_request(params):
    API_URL = 'http://en.wikipedia.org/w/api.php'
    RATE_LIMIT = True
    RATE_LIMIT_MIN_WAIT = 1 
    RATE_LIMIT_LAST_CALL = None
    USER_AGENT = 'wikipedia (https://github.com/goldsmith/Wikipedia/)'

    params['format'] = 'json'

    headers = {
        'User-Agent': USER_AGENT
    }

    if RATE_LIMIT and RATE_LIMIT_LAST_CALL and \
        RATE_LIMIT_LAST_CALL + RATE_LIMIT_MIN_WAIT > datetime.now():
    # it hasn't been long enough since the last API call
    # so wait until we're in the clear to make the request
        wait_time = (RATE_LIMIT_LAST_CALL + RATE_LIMIT_MIN_WAIT) - datetime.now()
        time.sleep(int(wait_time.total_seconds()))

    r = requests.get(API_URL, params=params, headers=headers)

    if RATE_LIMIT:
       RATE_LIMIT_LAST_CALL = datetime.now()

    return r.json()

main(sys.argv[1:])
