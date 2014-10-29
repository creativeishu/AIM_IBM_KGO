import sys
import os
import cgi


# Path to the pipe for communication with the daemon
pipe_path = '/tmp/aim2014_graph_pipe'


# Get CGI fields
form = cgi.FieldStorage()

# Set content type:
print 'Content-type: text/html\r\n\n'

# Open the pipe and write what we get from the HTTP-GET request
f = open(pipe_path, 'w')
val = form["node"].value
f.write(val)
f.close()

# Open the pipe again, get the result and print it.
f = open(pipe_path, 'r')
data = f.read()
f.close()
print data

