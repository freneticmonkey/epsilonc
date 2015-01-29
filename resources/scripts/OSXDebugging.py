from epsilon import *
import random

from epsilon.scene import TransformSpace

from epsilon.render import Colour
from epsilon.math import Vector3
from CycleValue import CycleValue

class OSXDebugging(object):
	
	def __init__(self):
		self._sphere_trans = None
		# Wow.  Setting a const value here (Vector3.ZERO) crashes on OSX.... prob related to trying to set a value
		# to a const object
		self._pos = Vector3(0)
		self._cycle = CycleValue()
		self._init_pos = 0

	def on_start(self):
		print "before"
		print self.node.id()
		# print "self"
		# print dir(self)
		print "renderer"
		print self.node.renderer
		# print "node"
		# print dir(self.node)
		print "transform"
		# util.debug_break()
		print self.node.transform
		# print self.node.transform

		print "after sad"
		self._init_pos = self.node.transform.position.x
		print "after"
		# colour = self.node.renderer.material.diffuse
		
		# if colour != Colour.YELLOW:
		# 	random.seed(self.node.id)
		# 	self.node.renderer.material.diffuse = Colour(random.random(), 
		# 												 random.random(), 
		# 												 random.random(), 1.0)

		# Randomly set the cycle direction
		if random.random() < 0.5:
			self._cycle.set_dir(False)

		print self._init_pos

	def on_update(self, dt):
		self._pos.x = self._cycle.get_value(dt) + self._init_pos
		self.node.transform.position = self._pos
		
		if Input.key_down(Input.Key.Q):
			print "Forward"
			print self.node.transform.forward
			print "Up"
			print self.node.transform.up
			print "Right"
			print self.node.transform.right

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = OSXDebugging()