#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import wikipedia


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

obj = gethtml("Iron")

print obj.getproperty("Phase")
print obj.getproperty("Element category")
print obj.getproperty("Magnetic ordering")
print obj.getproperty("Melting point")





