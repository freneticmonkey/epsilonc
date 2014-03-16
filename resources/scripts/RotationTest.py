from epsilon import *

from epsilon.math import Vector3
from CycleValue import CycleValue

import math

class RotationTest(object):
	
	def __init__(self):
		self._cycle = CycleValue()

	def on_start(self):
		pass
		
	def on_update(self, dt):
		angle = (self._cycle.get_value(dt) * math.pi) / 10.0
		self.node.transform.rotate(Vector3.RIGHT, angle)

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = RotationTest()