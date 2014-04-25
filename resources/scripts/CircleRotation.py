from epsilon import *

from epsilon.math import Vector3

import math

class CircleRotation(object):
	
	def __init__(self):
		self._active = True

	def on_start(self):
		pass
		
	def on_update(self, dt):
		if self._active:
			angle = -dt * math.pi / 6.0
			self.node.transform.rotate(Vector3.UP, angle)

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = CircleRotation()