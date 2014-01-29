from epsilon import Input
from epsilon.math import Vector3, Quaternion
from epsilon.render import Camera
from epsilon.scene import TransformSpace

import math

class CameraBehaviour(object):
	
	def __init__(self):
		self._speed = 10
		self._angle_speed = 10
		self._mouse_speed_x = 0.02
		self._mouse_speed_y = 0.02


	def on_start(self):
		print "Camera Name: " + self.node.name

	def on_update(self, dt):

		# if the right mouse button is down, active cam controls
		if Input.mouse_button(Input.Button.Right):

			applied_speed = self._speed

			if Input.key(Input.Key.LShift):
				applied_speed *= 10.0

			# print "Speed: %s" % str(s)
			# print self.node.transform.position

			pos = self.node.transform.position

			if Input.key(Input.Key.A):
				self.node.transform.translate(-Vector3.RIGHT * applied_speed * dt)

			if Input.key(Input.Key.D):
				self.node.transform.translate(Vector3.RIGHT * applied_speed * dt )

			if Input.key(Input.Key.W):
				self.node.transform.translate(Vector3.FORWARD * applied_speed * dt )

			if Input.key(Input.Key.S):
				self.node.transform.translate(-Vector3.FORWARD * applied_speed * dt )

			angle = self._angle_speed * dt

			# Convert to radians
			angle *= (math.pi / 180.0)

			# convert the mouse input into rotation
			mouse_pos = Input.mouse_position_relative()

			h_angle = self._mouse_speed_x * -mouse_pos.y
			v_angle = self._mouse_speed_y * -mouse_pos.x

			self.node.transform.rotate(self.node.transform.up, h_angle)
			self.node.transform.rotate(self.node.transform.right, v_angle)

			# if h_angle > 0.0:
			# 	print self.node.transform.forward

			# Set the camera look at position to a point self._speed units in front
			self.node.lookat(self.node.transform.position, self.node.transform.forward * self._speed)

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = CameraBehaviour()