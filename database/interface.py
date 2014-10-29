#!/usr/bin/env python
import sys
import os
import cgi
import re


# Path to the pipe for communication with the daemon
pipe_path = '/tmp/aim2014_graph_pipe'


# Get CGI fields
form = cgi.FieldStorage()

# Set content type:
print 'Content-type: text/html\r\n\n'

# Open the pipe and write what we get from the HTTP-GET request

def process_query():
    val = str(form)
# "ERROR: web form <node> not sent."
    try:
        val = form["node"].value
        val = re.sub(r'_','/',val)
    except:
        print "graph g { n [label=\"" + val +"\"]; }"
        return
        
     
# FOR DEBUGGING:
#    print "graph g { n [label=\"" + val +"\"]; }"
    f = open(pipe_path, 'w')
    f.write(val)
    f.close()
# Open the pipe again, get the result and print it.
    f = open(pipe_path, 'r')
    data = f.read()
    data = re.sub(r'\/','_',data)
    f.close()
    print data
    return

process_query()
