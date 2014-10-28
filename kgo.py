#	KNowledge Graph. 


class KGraph:
	def __init__(self):
		self.nodes = []
		self.nodes2 = []
		self.edges = []
		self.allAttributes = set()
		self.node_attributes = {}
		self.node2_attributes = {}

	def add_node(self, element):
		self.nodes.append(element[0])
		attribute_info = element[1]
		self.node_attributes[element[0]] = attribute_info
		self.allAttributes.update(attribute_info.keys())
		att_dict = {(x,attribute_info[x]):element[0] \
						 for x in attribute_info.keys()}
		self.node2_attributes.update(att_dict)
		self.nodes2.extend(att_dict.keys())
		return ''

	
	def compareElements(self, e1, e2):
		att1 = self.node_attributes[e1]
		att2 = self.node_attributes[e2]
		compareDict = {}
		for y in att1.keys():
			if not(y in att2.keys()):
				continue
			alpha = float(abs(att1[y] - att2[y]))/att1[y]
			compareDict[alpha]=y
		return compareDict

	def add_edge(self, x1, x2, threshold):
		compareList = self.compareElements(x1, x2).keys()
		results = [x < threshold for x in compareList]
		if (sum(results) > 0):
			edge = (x1,x2)
			self.edges.append(edge)
		return ''

	def neighbors(self, node):
		nbrs = [node]
		for edge in self.edges:
			x,y = edge
			if (x == node):
				nbrs.append(y)
		return nbrs			

		
	# def searchByNode(self, node):
	# 	if (node in self.nodes):
	# 		return self.node_attributes[node]
	# 	else:
	# 		return None	


	def searchByAttribute(self, attribute):
		if (attribute in self.node2_attributes.keys()):
			return self.node2_attributes[attribute]
		else:
			return None	

	def searchbyKeyword(self, keyword):
		if (keyword in self.nodes):
			return keyword

		keyword = keyword.split()
		attr = keyword[0], keyword[1]
		if (keyword[0] in self.nodes):
			return keyword[0]
		if (keyword[1] in self.nodes):
			return keyword[1]	

		try:
			attribute = (keyword[0], float(keyword[1]))
			return self.searchByAttribute(attribute)
		except:
			return "There are no results matching the query."


	def search(self, query):
		result = self.searchbyKeyword(query)
		relatedStuff = self.neighbors(result)
		for x in relatedStuff:
			print x, self.node_attributes[x]
		return ''	



G = KGraph()

node1 = ['Iron',{'boil':100, 'melt':80, 'density':3, 'atomic_weight':14}]
node2 = ['Copper',{'boil':90, 'melt':60, 'density':1.5, 'atomic_weight':32}]
node3 = ['Copper1',{'boil':92, 'melt':63, 'density':1.7, 'atomic_weight':32}]
node4 = ['Copper2',{'boil':94, 'melt':66, 'density':1, 'atomic_weight':35}]
node5 = ['Copper3',{'boil':95, 'melt':67, 'density':2.5, 'atomic_weight':36}]
node6 = ['Copper4',{'boil':97, 'melt':76, 'density':3.5, 'atomic_weight':37}]
node7 = ['Copper5',{'boil':98, 'melt':86, 'density':2.0, 'atomic_weight':42}]
node8 = ['Copper6',{'boil':89, 'melt':72, 'density':1.9, 'atomic_weight':52}]
node9 = ['Copper7',{'boil':96, 'melt':90, 'density':3, 'atomic_weight':26}]
node10 = ['Copper8',{'boil':103, 'melt':93, 'density':1.8, 'atomic_weight':12, 'atomic_number':63}]


G.add_node(node1)
G.add_node(node2)
G.add_node(node3)
G.add_node(node4)
G.add_node(node5)
G.add_node(node6)
G.add_node(node7)
G.add_node(node8)
G.add_node(node9)
G.add_node(node10)
for x1 in G.nodes:
	for x2 in G.nodes:
		if (x1 != x2):
			G.add_edge(x1, x2, 0.05)

			
#print len(G.edges)
# print G.node_attributes['Iron']
#print G.allAttributes
#print G.neighbors('Iron')
#print G.searchByNode('Copper')
#print G.node2_attributes
#print G.searchByAttribute(('boil', 92))
#print G.searchbyKeyword('Iron')
print G.search(('boil 100'))















