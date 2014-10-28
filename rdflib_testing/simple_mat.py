import rdflib

bname = 'simple_mat2'

g = rdflib.Graph()
result = g.parse("%s.nt" % bname, format='nt')

print "graph has %s statements." % len(g)

import rdflib.tools.rdf2dot
rdflib.tools.rdf2dot.rdf2dot(g, open("%s.dot" % bname,'w'))
