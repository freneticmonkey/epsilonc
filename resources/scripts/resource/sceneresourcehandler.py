'''
Created on Mar 4, 2012

@author: scottporter
'''

from epsilon import SceneManager

from os.path import exists
from xml.etree import ElementTree as ET

from resource.resourcehandler import ResourceHandlerBase
from resource.resourceitem import ResourceType, Resource

from core.loginstance import LogInstance

# Import the handlers for each of the none types
from resource.nodetypes import *

class InvalidSceneFormat(Exception):
	pass

class SceneResourceHandler(ResourceHandlerBase):

	def __init__(self):
		ResourceHandlerBase.__init__(self)
		
		#self._scene_manager = SceneManager.get_instance()
		self._resource_type = ResourceType.SCENE
		self._filetypes = ["xml"]

		self._parsers = {}

		# Setup a custom Log for this class
		self._log = LogInstance("SceneResourceHandler")

		self._config_parsers()

		self._scene_root = None

	def _config_parsers(self):
		for parser in [SceneScene,
					   SceneNode,
					   SceneTransform,
					   SceneCamera,
					   SceneMaterial,
					   SceneColour,
					   SceneMesh,
					   SceneBehaviour,
					   SceneChildren,
					   SceneShader,
					   SceneScripts,
					   SceneTexture]:
			self._parsers[parser.node_type()] = parser(self._log)

	def process_resource(self, filename, load):

		new_scene = None
		is_loaded = False
		parse_ok = False

		# Read the Scene XML
		try:
			xml_dom = ET.parse(filename)
			parse_ok = True
		except ET.ParseError, e:
			self._log("Invalid Scene Format Detected.  Unable to load scene file: %s" % filename)

		if parse_ok:
			xml_root = xml_dom.getroot()
			
			if load:
				try:
					self._log("Loading scene: %s" % filename)

					parse_globals = ParseGlobals()
					parse_globals.xml_tag = xml_root

					# Process it
					self.parse_node(parse_globals, xml_root)
					
					# If successful in parsing the scene file
					if not parse_globals.current_scene is None:
						
						# set the new scene in the SceneManager
						SceneManager.set_scene(parse_globals.current_scene)

						# Indicate that the scene was loaded
						is_loaded = True

				except InvalidSceneFormat, e:
					self._log("Invalid Scene Format Detected.  Unable to load scene file: %s" % filename)
		
		# Return a new Resource object so that the scene is tracked by the Resource Manager
		return Resource(filename=filename, resource_type=ResourceType.SCENE, data=new_scene, loaded=is_loaded)


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

