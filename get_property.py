#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import numpy
import sys
import time

f = open('props_multiline.txt','r')
outfile = open('props_test_new.txt','w')
nodeIDS = []
names = []
i=0
while True:
    i+=1
#    if (i%1000==0):
#        print i
    line = f.readline()
    if not line:
        break

    if len(line)>10:
        ID = (line.split(':')[0])
        name = line.split('"name":')
        if len(name)>1:
            name = name[1]
        else:
            continue

        name = name.split('"')[1]
        if len(name)<2:
            continue

        if name == 'Calcium':
            print "warning",ID,'Line:',i


        if not name in names:
            names.append(name)
    #    else: 
    #        name = name.split()
    #        if len(name)==1:
    #            print name
    #            time.sleep(0.2)
        nodeIDS.append(ID)

f.close()
outfile.close()
print len(names),len(nodeIDS)

sys.exit()

for i in range(len(names)):
    for j in range(i+1,len(names)):
        if names[i]==names[j]:
            print i,j,names[i],names[j]
            time.sleep(0.2)





'''
class gethtml:
    def __init__(self,elementname):
        x = wikipedia.WikipediaPage(elementname)
        x = x.html()
        x = x.encode('ascii', 'ignore').decode('ascii')
        x = x.replace('&#160;',' ')
        x = x.replace('&#x20',' ')
        x = x.replace('0b;',' ')
        self.html = x

    def getproperty(self,propertyname):
        propertyname = '>'+propertyname+'</a>'
        if (propertyname) in self.html:
            index = self.html.index(propertyname)
            newstring = self.html[index-300:index+300]
            newstring = newstring.split('</td></tr><tr><th')
            newstring = newstring[1].split('title')
            newstring = newstring[-1]
            newstring = newstring.split('>')[1]
            newstring = newstring.split('<')[0]
            return newstring
        else:
            print "Property not found"
            return None


element = sys.argv[1]

obj = gethtml(element)

print element, obj.getproperty("Phase"),\
                obj.getproperty("Element category"),\
                obj.getproperty("Magnetic ordering"),\
                obj.getproperty("Melting point")

'''




