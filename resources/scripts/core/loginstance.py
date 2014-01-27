from epsilon.logging import Logger

# Helper to aid setting up Log Instances for custom logs

class LogInstance(object):
	def __init__(self, logname):
		self._logname = logname
    
	def __call__(self, msg):
		self.log(msg)

	def log(self, msg):
		Logger.log(self._logname, msg)