import os
from os.path import join, realpath, dirname, isabs, splitext, exists

from core.settings import Settings
from core.loginstance import LogInstance

from ConfigParser import ConfigParser

class ResourceManager(object):

	def __init__(self):
		# Setup a custom Log for this class
		self._log = LogInstance("ResourceManager")

		# Set Default Resources folder
		self._current_path = os.path.join(os.getcwd(), Settings.get("ResourceSettings", "path"))
		self._load_config = {}

		self._handlers = []
		self._resources = {}

	def init(self):
		self._load_resources_cfg()

		try:
			self._load_resources()
		except Exception, e:
			print "Error loading scene: " + str(e)
		
	@property
	def current_path(self):
		return self._current_path
	
	@current_path.setter
	def current_path(self, new_current_path):
		self._current_path = new_current_path
		
	# Add a handler for loading specific Resources
	def add_handler(self, new_handler):
		
		# Disabling Class check... and instead checking for raw attributes due to circular import issue with ResourceHandler
#		if isinstance(new_handler, ResourceHandler):
		
		if hasattr(new_handler, "filetypes") and  hasattr(new_handler, "process_resource"):
			self._handlers.append(new_handler)
		else:
			self._log("The Resource Handler class: %s is not derived from ResourceHandler." % (new_handler.__class__.__name__))
	
	
	def _load_resources_cfg(self):
		# Parse resources file
		self._config = ConfigParser()

		# Resources file is resources.cfg in the resources folder
		file_name = "resources.cfg"
		file_path = os.path.join(self._current_path, file_name)

		if os.path.exists(file_path):
			result = self._config.read(file_path)

			if len(result) > 0 and os.path.basename(result[0]) == file_name:

				# Populate the classes with the appropriate values
				for section in self._config.sections():
					self._load_config[section] = {}
					
					for option in self._config.options(section):
						value = self._config.get(section, option)
						if value.find(',') != -1:
							value = value.replace(' ','').split(',')
						self._load_config[section][option] = value
		else:
			self._log("Unable to find Resources file: %s" % file_path)

	# load all resources from the resources folder, 
	# filtered by the resources.cfg file
	def _load_resources(self):

		# Build file list
		file_list = {}

		# Build the ignore lists
		ignore_folders = []
		ignore_formats = []

		# If there is an ignore section, and it contains a 'folder' option
		if "ignore" in self._load_config:
			if "folders" in self._load_config["ignore"]:
				ignore_folders = self._load_config["ignore"]["folders"]

			if "formats" in self._load_config["ignore"]:
				ignore_formats = self._load_config["ignore"]["formats"]				

		for path, dirs, files in os.walk(self._current_path):
			if os.path.basename(path) not in ignore_folders:
				for f in files:
					
					# get the file extension
					ext = os.path.splitext(f)[1][1:].lower()

					# firstly check the ignore
					if ext in ignore_formats:
						# ignore this file
						self._log("Ignoring file: %s" % f)
					else:						

						# Determine what type of file it belongs to
						for section in self._load_config.keys():

							# If formats have been specified for this resource type
							if "formats" in self._load_config[section]:
								# if the extension of the file is being tracked by a resource type
								if ext in self._load_config[section]["formats"]:

									# If necessary create the filetype list
									if section not in file_list.keys():
										file_list[section] = []

									# Store the path to the file
									file_abs = os.path.join(path, f)
									file_list[section].append(file_abs)
									#self._log("[%s] Found resource: %s" % (section, file_abs) )

							# TODO: Handle Resource file lists here

		# Now we have a list of the resources

		# The load is ordered by known dependencies
		
		# Textures
		# NYI

		# Shaders		
		# NYI

		# Meshes		
		# NYI

		# Scene
		if "scene" in file_list:

			main_scene = None
			if "start" in self._load_config["scene"]:
				main_scene = self._load_config["scene"]["start"]
				
			for scene_file in file_list["scene"]:
				load = False

				# if the scene is the main scene, as specified in the resources.cfg
				if os.path.split(scene_file)[1] == main_scene:
					# Load it during processing
					load = True
					self._log("Loading Scene file: %s" % os.path.split(scene_file)[1])

				# Process the scene resource
				self.process_resource(scene_file, load)
	
	# External function that can in turn be called by the ResourceHandlers allowing for recursive loading
	def process_resource(self, filename, load):
		
		# check if this resource has already been loaded
		new_resource = self.get_resource_by_filename(filename)
		
		# If the resource hasn't already been loaded, load it
		if new_resource is None:
			# Check file extension to see if this type is handled by this ResourceHandler
			parts = splitext(filename)
			# trim the period and ensure lowercase
			extension = parts[1][1:].lower()
			
			the_handler = None
			for handler in self._handlers:
				if extension in handler.filetypes:
					the_handler = handler
					break
				
			# If a handler for the filetype is found
			if not the_handler is None:
				# Get the current path set in the ResourceManager
				current_resource_path = self._current_path
				
				# get the path of the new Resource
				abs_filename = filename
				# if the path is relative
				if not isabs(filename):
					# Make it absolute
					abs_filename = realpath(join(current_resource_path, filename))
				print filename
				if not exists(filename):
					self._log("Resource could not be loaded: %s" % filename)
					self._log("Current path: %s" % os.getcwd())
				else:
					# Set the new path as the path in the ResourceManager
					self._current_path = dirname(abs_filename)
					
					# Process the Resource
					new_resource = the_handler.process_resource(abs_filename, load)
					if not new_resource is None:
						self._add_resource(new_resource)
						self._log("Added Resource: file: %s loading: %d" % (filename, load))
					
					# Rollback the current relative path set in the ResourceManager
					self._current_path = current_resource_path
			else:
				self._log("There is no handler registered to handle the specified filetype: %s for file: %s" % (extension.upper(), filename))
		
		return new_resource
	
	def _add_resource(self, new_resource):
		if not new_resource.rid in self._resources:
			self._resources[new_resource.rid] = new_resource
		else:
			self._log("Resource already exists with id: %d filename: %s" % (new_resource.rid, new_resource.filename))
	
	def remove_resource(self, filename="", rid=-1):
		# Find the resource and remove it using the appropriate handler
		# Will the resource have to notify its manager owners?  or is this a cleanup
		pass
	
	def get_resource_by_id(self, rid):
		if rid in self._resources:
			return self._resources[rid]
		else:
			return None
	
	def get_resource_by_filename(self, filename):
		found_resource = None
		for rid in self._resources:
			if self._resources[rid].filename == filename:
				found_resource = self._resources[rid]
				break
		return found_resource
	
	# Temporary function to test loading the test XML scene.
	def load_scene(self, scenefile):
		# In future create full path to file based on ResourceManager file
		scenefile = os.path.join(os.path.dirname(__file__),"..",scenefile)
		self._scene_loader.load_scene_file(scenefile)