
# Testing that the start function is called
class TestStart(object):
	
	def on_start(self):
		self._start = True
		
	def unittest_setup(self):
		self._start = False

	def unittest_result(self):
		return self._start == True

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = TestStart()