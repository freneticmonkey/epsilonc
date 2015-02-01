from epsilon import Input
from epsilon import Gizmos
from epsilon.render import Colour

class BoundsTest(object):
	
	def __init__(self):
		pass

	def on_start(self):
		pass
		
	def on_update(self, dt):
		if Input.key(Input.Key.G):
			bounds = self.node.transform.bounds
			Gizmos.colour(Colour.RED)
			Gizmos.draw_cube(bounds.centre, bounds.size*2.0)



	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = BoundsTest()