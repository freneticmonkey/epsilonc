from resource.resourcemanager import ResourceManager
import sys

# This is an awkward name and can change, when I'm rested enough to think of something better
class ScriptCore(object):
	def __init__(self):
		pass

	def on_start(self):
		print "Starting Main"
		print "Python Paths: " + ",".join(sys.path)
		rm = ResourceManager()

	def on_update(self, dt):
		pass

	def on_destroy(self, dt):
		pass

# For Epsilon Access
_instance = ScriptCore()
