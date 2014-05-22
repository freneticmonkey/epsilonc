from xml.etree import ElementTree as ET
from xml.dom import minidom
from epsilon import SceneManager


from resource.nodetypes import *

from core.loginstance import LogInstance

class SceneSaver(object):

	def __init__(self, path):
		self._xmlpath = path

		# Setup a custom Log for this class
		self._log = LogInstance("SceneSaver")

		self._parsers = {}

		for parser in [SceneScene,
					   #SceneSceneNode,
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
					   SceneTexture,
					   SceneRigidBody,
					   SceneAudioListener,
					   SceneAudioSource]:
			self._parsers[parser.node_type()] = parser(self._log)

		self._scenexml = None

	def save(self):
		# get the scene root
		root = SceneManager.current_scene.root
		self._scenexml = ET.Element('scene', {'name':'saved_scene'})
		self.process_node(root, self._scenexml)

		print "Save path: " + self._xmlpath
		et = ET.ElementTree(self._scenexml)
		#et.write(self._xmlpath)

		rough_string = ET.tostring(self._scenexml, "utf-8")
		formatted = minidom.parseString(rough_string)
		with open(self._xmlpath, 'w') as x:
			x.write(formatted.toprettyxml(indent="\t"))


	def process_node(self, node, xml):

		# pass node into each processor

		self._parsers[node.classname().lower()].write_node(node, xml, self.process_node)

		# for parser in self._parsers.values():
		# 	parser.write_node(node, xml, self.process_node)

