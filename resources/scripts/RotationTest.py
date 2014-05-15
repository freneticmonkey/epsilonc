from epsilon import *

from epsilon.math import Vector3
from CycleValue import CycleValue

import math

class RotationTest(object):
	
	def __init__(self):
		self._cycle = CycleValue()
		self._active = True
		self.angle = 0

	def on_start(self):
		pass
		
	def on_update(self, dt):
		if self._active:
			self.angle = dt * 1#(self._cycle.get_value(dt) * math.pi) / 200.0
			self.node.transform.rotate(Vector3.UP, self.angle)

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = RotationTest()