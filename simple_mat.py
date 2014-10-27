import rdflib
g = rdflib.Graph()
result = g.parse("simple_mat.nt", format='nt')

print "graph has %s statements." % len(g)

import rdflib.tools.rdf2dot
rdflib.tools.rdf2dot.rdf2dot(g, open('simple_mat.dot','w'))
