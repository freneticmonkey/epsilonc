from epsilon import ScriptManager
from epsilon.math import Vector2, Vector3, Quaternion

from epsilon.scene import Transform
from epsilon.scene import Node
from epsilon.scene import Scene
#from epsilon.scene import SceneManager

# REIMPLEMENT
#from epsilon.scripting import testscripts
#from Render.ShaderManager import ShaderManager
#from epsilon.render.texturemanager import TextureManager

from epsilon.render import Camera
# NYI
#from epsilon.render import Light 
from epsilon.render import Colour
from epsilon.render import Material

#from epsilon.render import MeshTypesString
from epsilon.render import MeshFactory, Renderer

# Store globals that are passed to each parse function
class ParseGlobals(object):
	current_scene = None
	scene_node = None
	xml_tag = None

class BaseXMLNode(object):
	def __init__(self, log):
		self._log = log

	#@property
	@classmethod
	def node_type(cls):
		return cls.__name__.lower()[5:]

	def process_node(self, parse_globals, scene_node, xml_tag):
		pass

	# Parsing helper functions
	def string_to_float_array(self, xml_tag, name):

		string_value = xml_tag.attrib[name]

		if string_value.find(',') != -1:
			a = string_value.split(",")
		else:
			a = string_value.split(" ")
		l = len(a)
		return [float(s) for s in a], l

	def parse_vector3(self, xml_tag, name=""):
		coordf, length = self.string_to_float_array(xml_tag, name)
		if length == 3:
			return Vector3(*coordf)
		else:
			self.raise_parse_issue("Invalid Vector3: %s [%s]" % (name, xml_tag.attrib[name]) )

	def parse_vector2(self, xml_tag, name=""):
		coordf, length = self.string_to_float_array(xml_tag, name)
		if length == 2:
			return Vector2(*coordf)
		else:
			self.raise_parse_issue("Invalid Vector2: %s [%s]" % (name, xml_tag.attrib[name]) )

	def parse_rot_axis(self, xml_tag, name=""):
		rotf, length = self.string_to_float_array(xml_tag, name)
		if length == 4:
			return Quaternion().new_rotate_axis(rotf[0], Vector3(rotf[1],rotf[2],rotf[3]))
		else:
			self.raise_parse_issue("Invalid Rot Axis: %s [%s]" % (name, xml_tag.attrib[name]) )            

	def parse_colour(self, xml_tag, name=""):
		coordf, length = self.string_to_float_array(xml_tag, name)
		if length == 4:
			return Colour(*coordf)
		else:
			self.raise_parse_issue("Invalid Colour: %s [%s]" % (name, xml_tag.attrib[name]) )

	def extract_float_attribute(self, xml_tag, name, default_value):
		return_value = default_value
		if name in xml_tag.attrib:
			return_value = float(xml_tag.attrib[name])
		return return_value

	def extract_int_attribute(self, xml_tag, name, default_value):
		return_value = default_value
		if name in xml_tag.attrib:
			return_value = int(xml_tag.attrib[name])
		return return_value

	def extract_bool_attribute(self, xml_tag, name, default_value):
		return_value = default_value
		if name in xml_tag.attrib:

			# Handle string value
			value = xml_tag.attrib[name]
			# convert to lower in case of javascript programmers
			if value.lower() == "true":
				return_value = True

			# Handle integer value.  Where value > 0 == True
			if not return_value:
				try:
					int_val = int(value)
					if int_val > 0:
						return_value = True
				except ValueError, e:
					pass

		return return_value

	def raise_parse_issue(self, issue_msg):
		self._log(issue_msg)
		raise InvalidSceneFormat(issue_msg)

class SceneScene(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		if "name" in xml_tag.attrib:
			scene = Scene.create(xml_tag.attrib["name"])
		else:
			scene = Scene.create()

		parse_globals.current_scene = scene

		# Returning the root node of the scene so that other parsers can just
		# use scene_node.add_child, rather than having checks if type == scene etc.
		return scene.root

class SceneNode(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		node = None
		node_name = ""

		if "name" in xml_tag.attrib:
			node_name = xml_tag.attrib["name"]
			
		# If a parent node exists
		if not scene_node is None:

			# create a node from the current parent node
			node = scene_node.create_child_node()

			# Set the name of the node
			node.name = node_name				
				
		# otherwise it's a floating node, not sure what this means...
		else:
			node = Node.create(node_name)
			self._log("WARNING: Scene contains an unattached node: %s" % node_name)

		return node

class SceneTransform(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		if not scene_node is None:

			if "position" in xml_tag.attrib:
				scene_node.transform.position = self.parse_vector3(xml_tag, "position")
			
			if "rotation" in xml_tag.attrib:
				scene_node.transform.rotation = self.parse_rot_axis(xml_tag, "rotation")
				
			if "scale" in xml_tag.attrib:
				scene_node.transform.local_scale = self.parse_vector3(xml_tag, "scale")

		else:
			self.raise_parse_issue("transform without node parent.")

class SceneCamera(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		if "name" in xml_tag.attrib:
			camera = Camera.create(xml_tag.attrib["name"])
		else:
			camera = Camera.create()

		#Extract whether the camera is the active camera in the scene
		active = self.extract_bool_attribute(xml_tag, "active", False)

		# Parse Camera look at values
		if "look_at" in xml_tag.attrib:
			camera.lookat(self.parse_vector3(xml_tag, "look_at"))

		if "from" in xml_tag.attrib:
			if "to" in xml_tag.attrib:
				from_pos = self.parse_vector3(xml_tag, "from")
				to_pos = self.parse_vector3(xml_tag, "to")
				camera.lookat(from_pos, to_pos)
			else:
				self.raise_parse_issue("camera with 'from' attribute missing 'to' attribute")

		elif "to" in xml_tag.attrib:
			if "from" in xml_tag.attrib:
				from_pos = self.parse_vector3(xml_tag, "from")
				to_pos = self.parse_vector3(xml_tag, "to")
				camera.lookat(from_pos, to_pos)
			else:
				self.raise_parse_issue("camera with 'to' attribute missing 'from' attribute")

		# Add the camera to it's node parent
		if not scene_node is None:
			scene_node.add_child(camera)

		# Add the camera to the scene
		parse_globals.current_scene.add_camera(camera)
		
		# if necessary make it the active camera
		if active:
			parse_globals.current_scene.active_camera = camera
			print "Made camera active: %s" % camera.name
		
		return camera

class SceneMaterial(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		if not scene_node is None:
			# ensure that the node has a renderer
			if scene_node.renderer is None:
				scene_node.renderer.create()

			if "ambient" in xml_tag.attrib:
				scene_node.renderer.material.diffuse = self.parse_colour(xml_tag, "ambient")

			if "diffuse" in xml_tag.attrib:
				scene_node.renderer.material.diffuse = self.parse_colour(xml_tag, "diffuse")

			if "specular" in xml_tag.attrib:
				scene_node.renderer.material.diffuse = self.parse_colour(xml_tag, "specular")

class SceneColour(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		if not scene_node is None:
			# ensure that the node has a renderer
			if scene_node.renderer is None:
				scene_node.renderer.create()

			if "ambient" in xml_tag.attrib:
				scene_node.renderer.material.diffuse = self.parse_colour(xml_tag, "ambient")

			if "diffuse" in xml_tag.attrib:
				scene_node.renderer.material.diffuse = self.parse_colour(xml_tag, "diffuse")

			if "specular" in xml_tag.attrib:
				scene_node.renderer.material.diffuse = self.parse_colour(xml_tag, "specular")

class SceneMesh(BaseXMLNode):

	_mesh_factory_types = [
		"GRID",
		"CUBE",
		"SPHERE",
		"PLANE",
		"TRIANGLE",
		"ICOHEDRON",
		"OCTOHEDRON"
	]

	def process_node(self, parse_globals, scene_node, xml_tag):
		if not scene_node is None:
			
			mesh = None

			if "preset" in xml_tag.attrib:
				preset = xml_tag.attrib["preset"]
				if preset in self._mesh_factory_types:
					if preset == "GRID":
						default_width = 10
						default_height = 10
						s = self.extract_int_attribute(xml_tag, "size", default_width)
						r = self.extract_int_attribute(xml_tag, "resolution", default_height)
						mesh = MeshFactory.generate_grid(s, r)

					elif preset == "CUBE":
						default_width_segs = 1
						default_height_segs = 1
						width_segs = self.extract_float_attribute(xml_tag, "width_segs", default_width_segs)
						height_segs = self.extract_float_attribute(xml_tag, "height_segs", default_height_segs)
						mesh = MeshFactory.generate_cube(default_width_segs, default_height_segs)

					elif preset == "SPHERE":
						default_stacks = 8
						default_slices = 8
						stacks = self.extract_float_attribute(xml_tag, "stacks", default_stacks)
						slices = self.extract_float_attribute(xml_tag, "slices", default_slices)
						mesh = MeshFactory.generate_sphere(slices, stacks)

					elif preset == "PLANE":
						default_width_segs = 1
						default_height_segs = 1
						width_segs = self.extract_float_attribute(xml_tag, "width_segs", default_width_segs)
						height_segs = self.extract_float_attribute(xml_tag, "height_segs", default_height_segs)
						mesh = MeshFactory.generate_plane(width_segs, height_segs)

					elif preset == "TRIANGLE":
						mesh = MeshFactory.generate_triangle()

					elif preset == "ICOHEDRON":
						mesh = MeshFactory.generate_icohedron()

					elif preset == "OCTOHEDRON":
						mesh = MeshFactory.generate_octohedron()

			# ensure that the node has a renderer and that a mesh was generated
			if scene_node.renderer is None and mesh is not None:
				scene_node.add_component(Renderer.create(mesh))


class SceneBehaviour(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		if not scene_node is None:
			if "filename" in xml_tag.attrib:                
				scene_node.add_component(ScriptManager.create_behaviour(xml_tag.attrib["filename"]))


class SceneChildren(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		# If a scene node has been defined, or rather, the XML children tag is defined within a node tag
		if scene_node is None:
			self.raise_parse_issue("transform without node parent.")

class SceneScripts(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		# If a scene node has been defined, or rather, the XML children tag is defined within a node tag
		if scene_node is None:
			self.raise_parse_issue("scripts without node parent.")

class SceneScript(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):

		if "name" in xml_tag.attrib:
			self._log("Loading scripts by name currently unsupported. Name: %s" % xml_tag.attrib["name"])

		if "filename" in xml_tag.attrib:
			scene_node.add_component(ScriptManager.create_behaviour(xml_tag.attrib["filename"]))

		# If a scene node has been defined
		if scene_node is None:
			self.raise_parse_issue("Error: Cannot attach a script without node parent.")

# NYI in engine
# 
class SceneShader(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		pass

class SceneTexture(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		pass
