
# Testing that the update function is called (once only)
class TestStart(object):
	
	def __init__(self):
		self._update = 0

	def on_update(self):
		self._update += 1
		
	def unittest_setup(self):
		self._update = 0

	def unittest_result(self):
		return self._update == 1

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = TestStart()