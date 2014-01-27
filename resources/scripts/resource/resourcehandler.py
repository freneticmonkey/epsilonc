'''
Created on Mar 4, 2012

@author: scottporter
'''
# Base class for all Resource Handlers
class ResourceHandlerBase(object):
    def __init__(self):
        
        self._resource_type = None
        self._filetypes = []
    
    @property
    def resource_type(self):
        return self._resource_type
    
    @property
    def filetypes(self):
        return self._filetypes
    
    def process_resource(self, filename, load):
        print "Errrpp you shouldn't be seeing this."
    
    def remove_resource(self, filename=""):
        pass