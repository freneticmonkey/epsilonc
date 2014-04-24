from epsilon.render import Colour
from epsilon import Gizmos 
from epsilon.math import Vector3

class PositionGizmo(object):
	
	def __init__(self):
		pass

	def on_start(self):
		pass
		
	def on_update(self, dt):
		Gizmos.colour(Colour.YELLOW)
		Gizmos.draw_sphere(self.node.transform.position, 0.25)

		self.node.transform.lookat(Vector3.ZERO);

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = PositionGizmo()