from epsilon import Input
from epsilon.logging import Logger
from epsilon.math import Vector3, Quaternion, Vector2
from epsilon.render import Camera
from epsilon.render.const import *
from epsilon.scene import TransformSpace
from epsilon import util

import math

class CameraBehaviour(object):
	
	def __init__(self):
		self._speed = 10
		self._angle_speed = 100.0
		self._mouse_speed_x = 0.2
		self._mouse_speed_y = 0.2

		self._was_down = False
		self._middle = Vector2(WIDTH/2,HEIGHT/2)

		self._tla = False

	def on_start(self):

		print "Camera Name: " + self.node.name
		self._control = self.node.transform.parent_transform.parent

	def on_update(self, dt):
		
		self._tla = False

		if Input.key_down(Input.Key.L):
			self.node.lookat(Vector3(1,1,0))

		# if the right mouse button is down, active cam controls
		if Input.mouse_button(Input.Button.Right):
			Input.mouse_visible(False)
			self._was_down = True

			# if Input.mouse_button_down(Input.Button.Right):
			# 	Input.set_mouse_position(self._middle)

			self.movement(dt)
			self.rotation(dt)
			self.look_at()

			# Input.set_mouse_position(self._middle)
		else:
			if self._was_down:
				Input.mouse_visible(True)
				self._was_down = False

	def look_at(self):
		target = self._control.transform.forward * self._speed
		self.node.lookat(self._control.transform.position, target)

	def rotation(self, dt):
		angle = self._angle_speed * dt

		# # Convert to radians
		angle *= (math.pi / 180.0)

		# convert the mouse input into rotation
		mouse_pos = Input.mouse_position_relative()
	
		h_angle = (self._mouse_speed_x * -mouse_pos.x * angle) #angle
		v_angle = (self._mouse_speed_y * -mouse_pos.y * angle) #0.0

		self._control.transform.rotate(self.node.transform.up, h_angle)
		self._control.transform.rotate(self.node.transform.right, v_angle)
	
	def movement(self, dt):
		applied_speed = self._speed

		if Input.key(Input.Key.LShift):
				applied_speed *= 10.0

		if Input.key(Input.Key.A):
			self._control.transform.translate(-Vector3.RIGHT * applied_speed * dt)

		if Input.key(Input.Key.D):
			self._control.transform.translate(Vector3.RIGHT * applied_speed * dt )

		if Input.key(Input.Key.W):
			self._control.transform.translate(Vector3.FORWARD * applied_speed * dt )

		if Input.key(Input.Key.S):
			self._control.transform.translate(-Vector3.FORWARD * applied_speed * dt )

		if Input.key(Input.Key.E):
			self._control.transform.translate(Vector3.UP * applied_speed * dt )

		if Input.key(Input.Key.C):
			self._control.transform.translate(-Vector3.UP * applied_speed * dt )

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = CameraBehaviour()