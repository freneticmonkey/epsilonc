from epsilon import Input
from epsilon.math import Vector3, Quaternion
from epsilon import util

class KeyboardControl(object):
	
	def __init__(self):
		self._speed = 10
		self._angle_speed = 100.0
		self._mouse_speed_x = 0.02
		self._mouse_speed_y = 0.02

		self._on = False

	def on_start(self):
		pass
		
	def on_update(self, dt):
		if Input.key_up(Input.Key.RAlt):
			self._on = not self._on
			if self._on:
				print "kb on"
			else:
				print "kb off"
		if self._on:
			
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

			if Input.key(Input.Key.E):
				self.node.transform.translate(Vector3.UP * applied_speed * dt )

			if Input.key(Input.Key.C):
				self.node.transform.translate(-Vector3.UP * applied_speed * dt )

	def on_destroy(self):
		pass

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = KeyboardControl()