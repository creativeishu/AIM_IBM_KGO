#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import requests
import time
import json
from json import JSONDecoder
from datetime import datetime, timedelta
import wikipedia as wp


def main(args):
    print "#getting periodic table"
    
    #output_to_file("test.txt",[["s","p","o"], ["x","y","z"]])

    periodic_table = wp.page("Category:Chemical elements")

    pages =  get_pages_in_category("Chemical elements")
    for x in pages :
        print x




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
    RATE_LIMIT = False
    RATE_LIMIT_MIN_WAIT = None
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
