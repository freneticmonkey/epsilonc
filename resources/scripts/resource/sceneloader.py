'''
Created on Mar 18, 2014

@author: scottporter
'''

from epsilon.resource import Resource, ResourceOwner

from epsilon import SceneManager, ResourceManager

from os.path import exists
from xml.etree import ElementTree as ET

from resource.resourcehandler import ResourceHandlerBase
from resource.resourceitem import ResourceType, Resource

from core.loginstance import LogInstance

# Import the handlers for each of the none types
from resource.nodetypes import *

class InvalidSceneFormat(Exception):
	pass

class SceneLoader(ResourceOwner):

	def __init__(self):
		ResourceOwner.__init__(self)
		
		self._parsers = {}

		# Setup a custom Log for this class
		self._log = LogInstance("SceneLoader")

		self._scene_root = None
		self._scene_file = ""
		self._scene_resource = None

		self._scene = None

		# setup parsers
		self._config_parsers()

	def _config_parsers(self):
		for parser in [SceneScene,
					   SceneNode,
					   SceneTransform,
					   SceneCamera,
					   SceneLight,
					   SceneMaterials,
					   SceneMaterial,
					   SceneColour,
					   SceneMesh,
					   SceneBehaviour,
					   SceneChildren,
					   SceneShader,
					   SceneScripts,
					   SceneScript,
					   SceneTexture]:
			self._parsers[parser.node_type()] = parser(self._log)

	def set_scene(self, scene_file):
		self._scene_file = scene_file

		# register for scene
		self.register_scene_changed()

		# Load the scene
		self.process_resource()

	def register_scene_changed(self):
		# Find the scene file.
		scenes = ResourceManager.find_resources(".*" + self._scene_file)
		
		for scene in scenes:
			print "Found scene: " + scene.filepath.string
			if scene.filepath.string.endswith(self._scene_file):
				self._scene_resource = scene

				#Register listening for file changes
				self.register_resource(scene)

				print "register complete"

	def refresh_resources(self, change_list):
		print "Scene file change detected."
		self.process_resource()
		print "Scene file reloaded"
		self._scene_resource.set_reloaded()

	def process_resource(self):

		new_scene = None
		is_loaded = False
		parse_ok = False

		# Read the Scene XML
		try:
			xml_dom = ET.parse(self._scene_file)
			parse_ok = True
		except ET.ParseError, e:
			self._log("Invalid Scene Format Detected.  Unable to load scene file: %s" % filename)

		if parse_ok:
			xml_root = xml_dom.getroot()
			
			try:
				self._log("Loading scene: %s" % self._scene_file)

				parse_globals = ParseGlobals()
				parse_globals.xml_tag = xml_root

				if not self._scene is None:
					parse_globals.current_scene = self._scene

				# Process it
				self.parse_node(parse_globals, xml_root)
				
				# If successful in parsing the scene file and a scene file hasn't yet been set
				if self._scene is None and not parse_globals.current_scene is None:
					
					# set the new scene in the SceneManager
					SceneManager.set_scene(parse_globals.current_scene)

					# Indicate that the scene was loaded
					is_loaded = True

					# Mark the scene as ready to be watched by the resource manager
					if not self._scene_resource is None:
						self._scene_resource.set_reloaded()

					print "Scene Loaded."
				else:
					print "Scene Refreshed."

			except InvalidSceneFormat, e:
				self._log("Invalid Scene Format Detected.  Unable to load scene file: %s" % filename)


	def parse_node(self, parse_globals, node, parent=None):
		
		tag = node.tag
		scene_node = None

		if parent is None:
			parent = self._scene_root

		if tag in self._parsers.keys():
			
			# If there isn't a parent node defined, this is the root node of the scene
			return_node = self._parsers[tag].process_node(parse_globals, parent, node)

			# We need to be able to handle grouping tags. i.e. <children>, <scripts> which are not
			# 'nodes' but groupings of nodes attached to a parent.

			# If the processor returned a new node, set it as the current scene node.
			if not return_node is None:
				scene_node = return_node
			# otherwise the current parent node is current scene_node
			else:
				scene_node = parent
		else:
			self._log("WARNING: Unsupported XML Tag: " + node.tag)

		# if the XML node has children
		for child in node.getchildren():
			self.parse_node(parse_globals, child, scene_node)

