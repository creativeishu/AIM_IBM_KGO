# vim: set ts=4 sw=4

import json

class EdgeFile:
    """Write an edge file (simplified N-Triples in JSON)"""

    def __init__(self, filename, overwrite = False):
        if not overwrite:
            import os.path
            if os.path.isfile(filename):
                raise RuntimeError("file already exists: %s" % filename)

        self._file = open(filename, 'w')
        self._file.write('[\n')

        self._separator = ''

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self._file.write('\n]')
        self._file.close()

    def add_entry(self, subject, predicate, value):
        self._file.write(self._separator)
        json.dump([subject, predicate, value], self._file)
        self._separator = ',\n'

class PropertiesFile:
    """Write a properties file (JSON)"""

    def __init__(self, filename, overwrite = False):
        if not overwrite:
            import os.path
            if os.path.isfile(filename):
                raise RuntimeError("file already exists: %s" % filename)

        self._file = open(filename, 'w')
        self._file.write('{\n')

        self._separator = ''

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self._file.write('\n}')
        self._file.close()

    def add_entry(self, nodename, properties):
        """ Add a new entry to the file.

        The properties are expected to be a python hash:
            { "melting point": 1234, "boiling point": 1500 }

        Implementation note: since we are doing a "streaming output"
        we have to workaround the fact that the fragments the JSON library
        writes must be valid JSON itself. Meaning that we can't make the JSON
        library to output only the following:
            "Iron": { "melting point": 1234 }
        """
        self._file.write(self._separator)
        self._file.write("\"%s\": " % nodename)
        json.dump(properties, self._file)
        self._separator = ',\n'
