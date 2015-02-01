class TestClass(object):
	
	def __init__(self):
		pass

	def on_start(self):
		pass
		
	def on_update(self, dt):
		pass

	def on_destroy(self):
		pass

	def unittest_setup(self):
		pass

	def unittest_teardown(self):
		pass

	def unittest_result(self):
		return True

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = TestClass()