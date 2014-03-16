class Smooth(object):
	def __init__(self, size):
		self._size 		= size
		self._values 	= [0] * size
		self._pos 		= 0

	def smooth(self, value):
		self._values[self._pos] = value
		if self._pos == (self._size - 1):
			self._pos = 0

		return reduce(lambda x, y: x + y, self._values) / self._size