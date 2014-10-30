#!/usr/bin/env python
import sys
import os
import cgi
import re

USE_DUMMY=False

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
    search_name=True
    try:
        val = form["node"].value
        if 'id:' in val:
            search_name = False
            val = val.replace('id:', '')
            val = re.sub(r'___','/',val)
    except:
        print "graph g { n [label=\"" + val +"\"]; }"
        return
    
    query = '%d %s' % (search_name, val)
    
    f = open(pipe_path, 'w')
    f.write(query)
    f.close()
# Open the pipe again, get the result and print it.
    f = open(pipe_path, 'r')
    data = f.read()
    data = re.sub(r'\/','___',data)
    f.close()
    print data
    return

if USE_DUMMY:
    print """graph G {
_m_04t7l [label="Metal" title="test1"];
_m_04t7l -- _m_025rw19 [label="superclass_of._engineering_material_subclass" title="test1"];
_m_04t7l -- _m_025sqz8 [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_025rsfk [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_027vj2v [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_06qqb [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_01brf [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_01504 [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_02w4cxr [label="superclass_of._engineering_material_subclass"];
_m_04t7l -- _m_025d79z [label="object._freebase_valuenotation_is_reviewed"];
_m_04t7l -- _m_0bn_73x [label="broader_than._base_skosbase_vocabulary_equivalent_topic_narrower_concept"];
_m_04t7l -- _m_02_y2k8 [label="object._dataworld_gardening_hint_replaces"];
_m_04t7l -- _m_044mcwg [label="property._common_topic_image"];
_m_04t7l -- _m_05ppmvg [label="property._common_topic_image"];
_m_04t7l -- _m_0cmqxm9 [label="property._common_topic_image"];
_m_04t7l -- _m_02_7ycf [label="object._type_object_attribution"];
_m_025rw19 [label="Metopirone"];
_m_025sqz8 [label="ZN"];
_m_025rsfk [label="Copper"];
_m_027vj2v [label="Aluminum"];
_m_06qqb [label="nolabel"];
_m_01brf [label="nolabel"];
_m_01504 [label="nolabel"];
_m_02w4cxr [label="nolabel"];
_m_025d79z [label="nolabel"];
_m_0bn_73x [label="nolabel"];
_m_02_y2k8 [label="nolabel"];
_m_044mcwg [label="nolabel"];
_m_05ppmvg [label="nolabel"];
_m_0cmqxm9 [label="nolabel"];
_m_02_7ycf [label="nolabel"];
}
    """
else:
    process_query()
