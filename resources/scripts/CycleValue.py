import math

class CycleValue(object):
	def __init__(self):
		self._time_value = 0
		self._freq = 100
		self._deg = 0
		self._dir = True

	def set_dir(self, direction):
		self._dir = direction

	def set_freq(self, freq):
		self._freq = freq

	def get_value(self, dt):

		self._time_value += dt
		if self._time_value > ( 1.0 / self._freq ):
			self._time_value = 0
			self._deg += 1

		if self._deg >= 360:
			self._deg = 0

		v = math.cos( (self._deg * math.pi) / 180.0 )
		# If the direction is flipped, flip the result
		if not self._dir:
			v = -v
		return v