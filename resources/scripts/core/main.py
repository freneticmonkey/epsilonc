import sys, os

from epsilon import RenderManager, Input, UIManager, util

from core.configurationmanager import ConfigurationManager

from resource.sceneloader import SceneLoader

from resource.scenesaver import SceneSaver

# This is an awkward name and can change, when I'm rested enough to think of something better
class ScriptCore(object):
	def __init__(self):
		self._cm = None
		self._rm = None

	def on_start(self):
		print "Starting Main. Initialising Python Managers"
		if util.is_debug():
			print "Running Debug Build"
		else:
			print "Running Release Build"
		self._cm = ConfigurationManager.get_instance()
		self._cm.load_configuration()		
		
		self._init_resources()

	def on_update(self, dt):
		
		# If escape is pressed, close the window
		if Input.key_down(Input.Key.Escape):
			RenderManager.close_window()
			print 'Closing'

		# If Tilde is pressed toggle the console window
		if Input.key_down(Input.Key.Tilde):
			print "NYI: Console Visible: %d" % self._console_window.visible

		if Input.key_down(Input.Key.Return) and False:			
			s = SceneSaver('test.xml')
			# Do the save
			s.save()


	def on_destroy(self):
		# self._cm.save_configuration()
		pass
		
	def _init_resources(self):
		self._sm = SceneLoader()
		self._sm.set_scene("model.xml")

# For Epsilon Access
_instance = ScriptCore()
