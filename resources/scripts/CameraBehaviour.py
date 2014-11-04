from epsilon import Input
from epsilon.logging import Logger
from epsilon.math import Vector3, Quaternion, Vector2, Matrix4
from epsilon.render import Camera, Colour
from epsilon.render.const import WIDTH, HEIGHT
from epsilon.scene import TransformSpace
from epsilon import util

from epsilon import Gizmos

from random import randint
import math

class CameraBehaviour(object):
	
	def __init__(self):
		self._orig_pos = self.node.transform.position

		self._was_down = False
		self._middle = Vector2(WIDTH/2,HEIGHT/2)

		self._reset()

	def _reset(self):
		self._speed = 10
		self._angle_speed = 1#100.0
		self._mouse_speed_x = 1 #10
		self._mouse_speed_y = 1 #10

		self.node.transform.orientation = Quaternion()
		self.node.transform.position = self._orig_pos
		self._v = 0
		self._h = math.pi		

	def on_start(self):
		# Convert to radians
		self._angle_speed *= (math.pi / 180.0)

	def on_update(self, dt):

		if Input.key_down(Input.Key.R):
			self._reset()

		if Input.key_down(Input.Key.I):

			print "Cam: info"
			print "---------"
			print "right: \t" + str(self.node.transform.right)
			print "up:  \t" 	+ str(self.node.transform.up)
			print "forward:"+ str(self.node.transform.forward)
			print "pos:  \t" 	+ str(self.node.transform.position)
			print "ori:  \t" 	+ str(self.node.transform.orientation)

		# if the right mouse button is down, active cam controls
		if Input.mouse_button(Input.Button.Right) or Input.key(Input.Key.Space):
			Input.mouse_visible(False)
			self._was_down = True

			if Input.mouse_button_down(Input.Button.Right):
				Input.set_mouse_position(self._middle)

			self.movement(dt)
			self.rotation(dt)

			Input.set_mouse_position(self._middle)
		else:
			if self._was_down:
				Input.mouse_visible(True)
				self._was_down = False

	def rotation(self, dt):
		angle = self._angle_speed * dt

		# convert the mouse input into rotation
		# mouse_pos = Input.mouse_position_relative()
		# 
		mouse_pos = Input.mouse_position()

		self._h += self._mouse_speed_x * angle * ( (WIDTH  / 2) - mouse_pos.x )
		self._v += self._mouse_speed_y * angle * ( (HEIGHT / 2) - mouse_pos.y )

		self.node.transform.fps(self.node.transform.position, self._v, self._h)
	
	def movement(self, dt):
		applied_speed = self._speed

		right 	= self.node.transform.right
		forward = self.node.transform.forward
		up 		= self.node.transform.up

		if Input.key(Input.Key.LShift):
				applied_speed *= 10.0

		if Input.key(Input.Key.A):
			self.node.transform.translate(right * applied_speed * dt)
			
		if Input.key(Input.Key.D):
			self.node.transform.translate(-right * applied_speed * dt )
			
		if Input.key(Input.Key.W):
			self.node.transform.translate(forward * applied_speed * dt )

		if Input.key(Input.Key.S):
			self.node.transform.translate(-forward * applied_speed * dt )

		if Input.key(Input.Key.E):
			self.node.transform.translate(-up * applied_speed * dt )

		if Input.key(Input.Key.C):
			self.node.transform.translate(up * applied_speed * dt )

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = CameraBehaviour()
