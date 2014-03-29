class TransformDebug(object):
	
	def __init__(self):
		pass

	def on_start(self):
		print "%s: %s" % (self.node.name, str(self.node.transform.position))

		#This does appear to fix the problem.  So is it an initialisation problem?
		#
		#self.node.transform.position.y = 0
		
	def on_update(self, dt):
		pass

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = TransformDebug()