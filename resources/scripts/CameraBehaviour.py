from epsilon import Input
from epsilon.logging import Logger
from epsilon.math import Vector3, Quaternion, Vector2
from epsilon.render import Camera, Colour
from epsilon.render.const import WIDTH, HEIGHT
from epsilon.scene import TransformSpace
from epsilon import util

from epsilon import Gizmos

from random import randint
import math

class CameraBehaviour(object):
	
	def __init__(self):
		self._speed = 10
		self._angle_speed = 100.0
		self._mouse_speed_x = 10
		self._mouse_speed_y = 10

		self._was_down = False
		self._middle = Vector2(WIDTH/2,HEIGHT/2)

		self._v = -math.pi
		self._h = 0
		
	def on_start(self):
		self._pos = self.node.transform.position
		self._control = self.node.transform.parent_transform.parent
		self._control.transform.position = Vector3(0,1,-18)

		# Convert to radians
		self._angle_speed *= (math.pi / 180.0)

	def on_update(self, dt):

		Gizmos.colour(Colour.BLUE)
		for x in range(10):
			for y in range(10):
				# x = randint(0, x)
				# y = randint(0, y)
				# z = randint(0, 10)
				Gizmos.draw_cube(Vector3(x,y,y), Vector3.ONE)

		Gizmos.colour(Colour.CYAN)
		Gizmos.draw_sphere(Vector3(0,-1,0), 1)

		# Gizmos.colour(Colour.GREEN)
		# mp = Input.mouse_position()
		# pos = self.node.screen_to_world(mp)
		# Gizmos.draw_sphere(pos, 0.1)

		# if the right mouse button is down, active cam controls
		if Input.mouse_button(Input.Button.Left):
			Input.mouse_visible(False)
			self._was_down = True

			if Input.mouse_button_down(Input.Button.Left):
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
		mouse_pos = Input.mouse_position_relative()

		self._h += (mouse_pos.x / WIDTH) * angle * self._mouse_speed_x
		self._v += (mouse_pos.y / HEIGHT) * angle * self._mouse_speed_y

		self.node.transform.fps(self._control.transform.position, self._v, self._h)
		# self.node.transform.fps(self._control.transform.position, self._v, self._h)
		# print self.node.transform.forward
		# self._control.transform.local_orientation = self.node.transform.orientation
	
	def movement(self, dt):
		applied_speed = self._speed

		viewMat = self.node.camera.get_view_matrix()
		# right 	= Vector3(viewMat[0], -viewMat[4], viewMat[8])
		# up 		= Vector3(viewMat[1], -viewMat[5], viewMat[9])
		# forward = Vector3(viewMat[2], -viewMat[6], viewMat[10])
		right = self.node.transform.right
		forward = self.node.transform.forward
		up = self.node.transform.up

		if Input.key_down(Input.Key.I):

			print "info"
			print "right: " + str(right)
			print "up: " + str(up)
			print "forward: " + str(forward)

		if Input.key(Input.Key.LShift):
				applied_speed *= 10.0

		if Input.key(Input.Key.A):
			self._control.transform.translate(right * applied_speed * dt)
			# self._control.transform.translate(-Vector3.RIGHT * applied_speed * dt)
			
		if Input.key(Input.Key.D):
			self._control.transform.translate(-right * applied_speed * dt )
			# self._control.transform.translate(Vector3.RIGHT * applied_speed * dt )
			
		if Input.key(Input.Key.W):
			self._control.transform.translate(-forward * applied_speed * dt )
			# self._control.transform.translate(Vector3.FORWARD * applied_speed * dt )

		if Input.key(Input.Key.S):
			self._control.transform.translate(forward * applied_speed * dt )
			# self._control.transform.translate(-Vector3.FORWARD * applied_speed * dt )

		if Input.key(Input.Key.E):
			self._control.transform.translate(up * applied_speed * dt )
			# self._control.transform.translate(Vector3.UP * applied_speed * dt )

		if Input.key(Input.Key.C):
			self._control.transform.translate(-up * applied_speed * dt )
			# self._control.transform.translate(-Vector3.UP * applied_speed * dt )

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = CameraBehaviour()
