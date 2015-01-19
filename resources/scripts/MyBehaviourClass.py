from epsilon import *
import random

from epsilon.scene import TransformSpace

from epsilon.render import Colour
from epsilon.math import Vector3, Vector2
from CycleValue import CycleValue

from epsilon.ui import IMGUI, WindowOptions

class MyBehaviour(object):
	
	def __init__(self):
		self._sphere_trans = None
		# Wow.  Setting a const value here (Vector3.ZERO) crashes on OSX.... prob related to trying to set a value
		# to a const object
		self._pos = Vector3(0)
		self._cycle = CycleValue()
		self._init_pos = 0

		self._options = WindowOptions()
		self._options.is_open = True
		self._options.position = Vector2(10,10)

		self._value = 0.5

		self._colour = Colour.RED

	def on_start(self):
		self._init_pos = self.node.transform.position.x
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

	def on_gui(self):
		if IMGUI.begin("Behaviour: " + self.node.name, self._options):
			IMGUI.text("Hello World! (From Python) which can change.")

			if IMGUI.small_button("Click Me"):
				IMGUI.text("Button Clicked!")

			IMGUI.spacing()

			self._value = IMGUI.slider_float("Value", self._value, 0.0, 1.0)

			self._value = IMGUI.input_float("Value Input", self._value, 0.1, 0.3, 3)

			IMGUI.text_wrapped("This is some very long text that will hopefully wrap on the window")

			IMGUI.spacing()
			self.node.transform.position.y = self._value
			self.node.transform.position = IMGUI.input_float3("Pos:", self.node.transform.position)

			self._colour = IMGUI.colour_edit4("Colour: ", self._colour)
		IMGUI.end()


# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = MyBehaviour()