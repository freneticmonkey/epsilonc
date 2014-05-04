class CollisionBehaviour(object):
	
	def __init__(self):
		pass

	def on_start(self):
		pass
		
	def on_update(self, dt):
		pass

	def on_destroy(self):
		pass

	def on_collision(self, collision):
		# print "collision"
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = CollisionBehaviour()