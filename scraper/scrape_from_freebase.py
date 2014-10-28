#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import json
import urllib

def main(args):
    service_url = 'https://www.googleapis.com/freebase/v1/search'
    params = {
       "domain": 'chemistry',
        "output" : "(all)",
     }
    url = service_url + '?' + urllib.urlencode(params)
    response = json.loads(urllib.urlopen(url).read())
    if not 'result' in response:
        print response
    print len(response['result'])
    for p in response['result']:
        if 'output' in p and 'all' in p['output']:
            print '=========================='
            for k,v in p['output']['all'].items():
                for vi in v:
                    if type(vi) == dict and 'mid' in vi:
                        print p['mid'], '<%s>'%k, vi['mid'] # links
                    else:
                        print p['mid'], '<%s>'%k, vi # node properties


main(sys.argv[1:])
