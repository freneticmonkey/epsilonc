from epsilon import *
import random

from epsilon.scene import TransformSpace

from epsilon.render import Colour
from epsilon.math import Vector3
from CycleValue import CycleValue

class MyBehaviour(object):
	
	def __init__(self):
		self._sphere_trans = None
		self._pos = Vector3.ZERO
		self._cycle = CycleValue()
		self._init_pos = 0

	def on_start(self):
		#return
		# sm = SceneManager.get_instance()
		# scene = sm.current_scene()
		# root = scene.root()

		# self._sphere_trans = root.transform.find_child_with_name("sphere")
		#self._init_pos = self._sphere_trans.get_position().x
		self._init_pos = self.node.transform.position.x

		colour = self.node.renderer.material.diffuse
		print colour
		if colour != Colour.YELLOW:
			random.seed(self.node.id)
			self.node.renderer.material.diffuse = Colour(random.random(), 
														 random.random(), 
														 random.random(), 1.0)

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
_instance = MyBehaviour()