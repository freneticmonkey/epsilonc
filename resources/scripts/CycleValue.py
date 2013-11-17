import math

class CycleValue(object):
	def __init__(self):
		self._time_value = 0
		self._freq = 100
		self._deg = 0

	def set_freq(self, freq):
		self._freq = freq

	def get_value(self, dt):
		self._time_value += dt
		if self._time_value > ( 1.0 / self._freq ):
			self._time_value = 0
			self._deg += 1

		if self._deg >= 360:
			self._deg = 0

		return math.cos( (self._deg * math.pi) / 180.0 )