import re

from xml.etree import ElementTree as ET

from epsilon import util

from epsilon import ScriptManager
from epsilon import MaterialManager, ShaderManager

from epsilon.math import Vector2, Vector3, Vector4, Quaternion

from epsilon.scene import Transform
from epsilon.scene import Node
from epsilon.scene import Scene
#from epsilon.scene import SceneManager

# REIMPLEMENT
#from epsilon.scripting import testscripts
#from Render.ShaderManager import ShaderManager
#from epsilon.render.texturemanager import TextureManager

from epsilon.render import Camera, Light, LightType, LightShadowType
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

	def write_node(self, node, xml_out, call_func):
		nodexml = ET.SubElement(xml_out, self.node_type())
		print nodexml
		self.process_properties(node, nodexml, call_func)
		self.process_children(node, xml_out, nodexml, call_func)

	def process_properties(self, node, nodexml, call_func):
		pass

	def process_children(self, node, parentxml, nodexml, call_func):
		pass

	# Parsing helper functions
	def string_to_float_array(self, xml_tag, name):

		string_value = xml_tag.attrib[name]

		string_value = re.sub(r"[\s|,]+", "|",string_value)

		if string_value.find('|') != -1:
			a = string_value.split("|")
		l = len(a)
		return [float(s) for s in a], l

	def parse_vector4(self, xml_tag, name=""):
		coordf, length = self.string_to_float_array(xml_tag, name)
		if length == 4:
			return Vector4(*coordf)
		else:
			self.raise_parse_issue("Invalid Vector3: %s [%s]" % (name, xml_tag.attrib[name]) )

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
			return Quaternion(Vector3(rotf[0],rotf[1],rotf[2]), rotf[3])
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

	def write_float(self, xml_tag, name, value):
		xml_tag.set(name, "%g" % value)

	def write_bool(self, xml_tag, name, value):
		if value:
			xml_tag.set(name, "True")
		else:
			xml_tag.set(name, "False")

	def write_vector2(self, xml_tag, name, value):
		xml_tag.set(name, "%g %g" % ( value.x, value.y))

	def write_vector3(self, xml_tag, name, value):
		xml_tag.set(name, "%g %g %g" % ( value.x, value.y, value.z))

	def write_vector4(self, xml_tag, name, value):
		xml_tag.set(name, "%g %g %g %g" % ( value.x, value.y, value.z, value.w))

	def write_colour(self, xml_tag, name, value):
		xml_tag.set(name, "%g %g %g %g" % ( value.r, value.g, value.b, value.a))

	def write_path(self, xml_tag, name, full_path):
		adj_path = full_path[full_path.find('resources'):]
		xml_tag.set(name, adj_path)

class SceneScene(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		scene = None

		# FIXME: This will totally break if there are nested scenes....

		# If a scene already exists (i.e. this is a scene file refresh)
		if not parse_globals.current_scene is None:
			scene = parse_globals.current_scene

			if "name" in xml_tag.attrib:
				scene.name = xml_tag.attrib["name"]

			# Clear the scene in preparation for loading the refreshed scene
			scene.root.remove_all_children()
			
		else:
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
			node = scene_node.create_child(node_name)
				
		# otherwise it's a floating node, not sure what this means...
		else:
			#node = Node.create(node_name)
			self._log("WARNING: Ignored: Scene Definition contains an unattached node: %s" % node_name)

		return node

	def process_properties(self, node, nodexml, call_func):
		nodexml.set("name", node.name)

		# process node components
		if node.camera:
			call_func(node.camera, nodexml)

		if node.light:
			call_func(node.light, nodexml)

		if node.renderer and node.renderer.material:
			call_func(node.renderer.material, nodexml)

		if node.is_audiolistener():
			ET.SubElement(nodexml, "audiolistener")

		if len(node.scripts) > 0:
			call_func(node.scripts, nodexml)

		if len(node.audiosources) > 0:
			call_func(node.audiosources, nodexml)			

		# transform
		call_func(node.transform, nodexml)

class SceneTransform(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		if not scene_node is None:

			if "position" in xml_tag.attrib:
				pos = self.parse_vector3(xml_tag, "position")
				scene_node.transform.position = pos

			if "rotation" in xml_tag.attrib:
				scene_node.transform.local_orientation = self.parse_rot_axis(xml_tag, "rotation")
				
			if "scale" in xml_tag.attrib:
				scene_node.transform.local_scale = self.parse_vector3(xml_tag, "scale")

		else:
			self.raise_parse_issue("transform without node parent.")

	def process_properties(self, node, nodexml, call_func):
		self.write_vector3(nodexml, "position", node.position)
		self.write_vector4(nodexml, "orientation", node.local_orientation)

	def process_children(self, node, parentxml, nodexml, call_func):

		# process any children of the node
		if len(node.children) > 0:
			nodexml = ET.SubElement(parentxml, 'children')

			for child in node.children:
				# process the parent node
				call_func(child.parent, nodexml)

class SceneCamera(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		# If a node is set
		if not scene_node is None:

			if "name" in xml_tag.attrib:
				camera = scene_node.create_camera(xml_tag.attrib["name"])
			else:
				camera = scene_node.create_camera()

			#Extract whether the camera is the active camera in the scene
			active = self.extract_bool_attribute(xml_tag, "active", False)

			# Parse Camera look at values
			if "look_at" in xml_tag.attrib:
				scene_node.transform.lookat(self.parse_vector3(xml_tag, "look_at"))

			if "from" in xml_tag.attrib:
				if "to" in xml_tag.attrib:
					from_pos = self.parse_vector3(xml_tag, "from")
					to_pos = self.parse_vector3(xml_tag, "to")
					scene_node.transform.lookat(from_pos, to_pos)
				else:
					self.raise_parse_issue("camera with 'from' attribute missing 'to' attribute")

			elif "to" in xml_tag.attrib:
				if "from" in xml_tag.attrib:
					from_pos = self.parse_vector3(xml_tag, "from")
					to_pos = self.parse_vector3(xml_tag, "to")
					scene_node.transform.lookat(from_pos, to_pos)
				else:
					self.raise_parse_issue("camera with 'to' attribute missing 'from' attribute")

			# Add the camera to it's node parent
			#scene_node.add_child(camera)
			#scene_node.add_component(camera)

			# Add the camera to the scene
			#parse_globals.current_scene.add_camera(camera)
			
			# if necessary make it the active camera
			if active:
				parse_globals.current_scene.active_camera = camera
				print "Made camera active: %s" % camera.name
			
			return camera

	def process_properties(self, node, nodexml, call_func):
		# process properties
		nodexml.set("name", node.name)


class SceneLight(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		if not scene_node is None:

			light = None

			if "name" in xml_tag.attrib:
				light = scene_node.create_light(xml_tag.attrib["name"])
			else:
				light = scene_node.create_light()

			if not light is None:

				if "type" in xml_tag.attrib:
					type_string = xml_tag.attrib["type"]

					if type_string == "POINT":
						light.type = LightType.POINT
					elif type_string == "SPOT":
						light.type = LightType.SPOT
					elif type_string == "DIRECTIONAL":
						light.type = LightType.DIRECTIONAL
					elif type_string == "SUN":
						light.type = LightType.SUN

				if "shadow_type" in xml_tag.attrib:
					shadow_type_string = xml_tag.attrib["shadow_type"]

					if shadow_type_string == "NONE":
						light.shadow_type = LightShadowType.NONE
					elif shadow_type_string == "HARD":
						light.shadow_type = LightShadowType.HARD
					elif shadow_type_string == "SOFT":
						light.shadow_type = LightShadowType.SOFT

				if "diffuse" in xml_tag.attrib:
					light.diffuse = self.parse_colour(xml_tag, "diffuse")

				if "attenuation" in xml_tag.attrib:
					light.attenuation = self.parse_vector3(xml_tag, "attenuation")

				if "angle" in xml_tag.attrib:
					default_angle = 0.0
					light.angle = self.extract_float_attribute(xml_tag, "angle", default_angle)

				if "strength" in xml_tag.attrib:
					default_strength = 1.0
					light.strength = self.extract_float_attribute(xml_tag, "strength", default_strength)

	def process_properties(self, node, nodexml, call_func):
		# process properties
		nodexml.set("name", node.name)

		self.write_colour(nodexml, "diffuse", node.diffuse)
		self.write_vector3(nodexml, "attenuation", node.attenuation)

		# print dir(LightShadowType)
		# if node.type == LightShadowType.SPOT:
		# 	nodexml.set("type","SPOT")
		# elif node.type == LightShadowType.HARD:
		# 	nodexml.set("type","HARD")
		# elif node.type == LightShadowType.NONE:
		# 	pass

class SceneMaterials(BaseXMLNode):
	
	# Don't do anything here each material will process itself
	def process_node(self, parse_globals, scene_node, xml_tag):	
		pass

class SceneMaterial(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):

		material = None

		# if there is a node then the material is being created and applied directly
		if not scene_node is None:
			# ensure that the node has a renderer
			if scene_node.renderer is None:
				scene_node.create_renderer()

			if not scene_node.renderer.material is None:
				material = scene_node.renderer.material
		else:
			# The material is most likely being pre-declared. So create it
			if "name" in xml_tag.attrib:
				new_material = MaterialManager.create_material(xml_tag.attrib["name"])
			else:
				new_material = MaterialManager.create_material()
				print "Warning: Material declared with no name: Name defaulting to: " + new_material.name

			material = new_material

		if not material is None:
			if "ambient" in xml_tag.attrib:
				material.ambient = self.parse_colour(xml_tag, "ambient")

			if "diffuse" in xml_tag.attrib:
				material.diffuse = self.parse_colour(xml_tag, "diffuse")

			if "specular" in xml_tag.attrib:
				material.specular = self.parse_colour(xml_tag, "specular")

			if "shader" in xml_tag.attrib:
				shader = MaterialManager.get_shader_by_name(xml_tag.attrib["shader"])

				if not shader is None:
					material.shader = shader
				else:
					self.raise_parse_issue("Material with unknown shader: " + xml_tag.attrib["shader"])
		else:
			print "%s : doesn't have a material?" % scene_node.name

	def process_properties(self, node, nodexml, call_func):
		# process node components

		self.write_colour(nodexml, "diffuse", node.diffuse)

		if node.shader:
			call_func(node.shader, nodexml)

		if len(node.textures) > 0:
			call_func(node.textures, nodexml)

class SceneColour(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		if not scene_node is None:
			# ensure that the node has a renderer
			if scene_node.renderer is None:
				scene_node.create_renderer()

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
			
			# If using a generated mesh
			if "preset" in xml_tag.attrib:
				mesh = None

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
						width_segs = self.extract_int_attribute(xml_tag, "width_segs", default_width_segs)
						height_segs = self.extract_int_attribute(xml_tag, "height_segs", default_height_segs)
						mesh = MeshFactory.generate_cube()#default_width_segs, default_height_segs)

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
				if mesh is not None:
					if scene_node.renderer is None:
						scene_node.create_renderer()

					scene_node.renderer.mesh = mesh

			# If using a mesh on disk
			elif "filename" in xml_tag.attrib:
				print "Found filename: " + xml_tag.attrib["filename"]
				if not scene_node.renderer is None:
					scene_node.renderer.set_mesh(xml_tag.attrib["filename"])

			# if scene_node.renderer is None and mesh is not None:
			# 	scene_node.add_component(Renderer.create(mesh))


class SceneBehaviour(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		if not scene_node is None:
			if "filename" in xml_tag.attrib:                
				#scene_node.add_component(ScriptManager.create_behaviour(xml_tag.attrib["filename"]))
				scene_node.create_behaviour(xml_tag.attrib["filename"])

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

	def process_children(self, scripts, parentxml, nodexml, call_func):
		# process any scripts
		for script in scripts:
			# process the parent node
			call_func(script, nodexml)

class SceneScript(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):

		if "name" in xml_tag.attrib:
			self._log("Loading scripts by name currently unsupported. Name: %s" % xml_tag.attrib["name"])

		if "filename" in xml_tag.attrib:
			# scene_node.add_component(ScriptManager.create_behaviour(xml_tag.attrib["filename"]))
			scene_node.create_behaviour(xml_tag.attrib["filename"])

		# If a scene node has been defined
		if scene_node is None:
			self.raise_parse_issue("Error: Cannot attach a script without node parent.")

	def process_properties(self, node, nodexml, call_func):
		self.write_path(nodexml, "filename", node.filename)

class SceneShader(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		material = None

		# if there is a node then the material is being created and applied directly
		if not scene_node is None:
			# ensure that the node has a renderer
			if scene_node.renderer is None:
				scene_node.create_renderer()

			if not scene_node.renderer.material is None:
				material = scene_node.renderer.material
		
		if not material is None:

			# The material is most likely being pre-declared. So create it
			if "name" in xml_tag.attrib:
				shader = ShaderManager.get_shader_by_name(xml_tag.attrib["name"])

				if not shader is None:
					material.shader = shader
				else:
					print "Warning: Unknown shader requested: " + xml_tag.attrib["name"]
		else:
			self.raise_parse_issue("Error: SceneNode: %s: doesn't have a material" % scene_node.name)

	def process_properties(self, node, nodexml, call_func):
		nodexml.set("name", node.name)

class SceneTextures(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		# If a scene node has been defined, or rather, the XML children tag is defined within a node tag
		if scene_node is None:
			self.raise_parse_issue("textures without node parent.")

	def process_children(self, textures, parentxml, nodexml, call_func):
		# process any textures
		for texture in textures:
			# process the parent node
			call_func(texture, nodexml)

class SceneTexture(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		material = None

		# find the material for this node
		if not scene_node is None:
			# ensure that the node has a renderer
			if scene_node.renderer is None:
				scene_node.create_renderer()

			if not scene_node.renderer.material is None:
				material = scene_node.renderer.material
		
		if not material is None:

			if "filename" in xml_tag.attrib:
				material.add_texture(xml_tag.attrib["filename"])

	def process_properties(self, node, nodexml, call_func):
		self.write_path(nodexml, "filename", node.filepath.string())

class SceneRigidBody(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		# find the material for this node
		if not scene_node is None:

			mass = 0
			inertia = Vector3.ZERO
			kinematic = False

			if "mass" in xml_tag.attrib:
				mass = self.extract_float_attribute(xml_tag, "mass", mass)

			if "inertia" in xml_tag.attrib:
				inertia = self.parse_vector3(xml_tag, "inertia")

			if "kinematic" in xml_tag.attrib:
				kinematic = self.extract_bool_attribute(xml_tag,"kinematic",kinematic)

			scene_node.create_rigidbody(mass, inertia, kinematic)

	def process_properties(self, node, nodexml, call_func):

		self.write_float(nodexml, "mass", node.mass)
		self.write_float(nodexml, "inertia", node.inertia)
		node.set("kinematic", node.kinematic)

class SceneAudioListener(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		# set this node as the audio listener
		if not scene_node is None:
			scene_node.set_audiolistener()

class SceneAudioSources(BaseXMLNode):
	def process_node(self, parse_globals, scene_node, xml_tag):
		# If a scene node has been defined, or rather, the XML children tag is defined within a node tag
		if scene_node is None:
			self.raise_parse_issue("audiosources without node parent.")

	def process_children(self, sources, parentxml, nodexml, call_func):
		# process any textures
		for audio in sources:
			# process the parent node
			call_func(audio, nodexml)

class SceneAudioSource(BaseXMLNode):

	def process_node(self, parse_globals, scene_node, xml_tag):
		
		# attach a new audio source to this node.
		if not scene_node is None:
			filename = ""
			
			if "filename" in xml_tag.attrib:
				filename = xml_tag.attrib["filename"]

			audiosource = scene_node.create_audiosource(filename)

			if audiosource:
				# if the audiosource was successfully created, assign any settings
				volume = 50.0
				loop = False
				play = False

				if "volume" in xml_tag.attrib:
					audiosource.volume = self.extract_float_attribute(xml_tag,"volume", volume)

				if "loop" in xml_tag.attrib:
					audiosource.loop = self.extract_bool_attribute(xml_tag,"loop",loop)

				if "play" in xml_tag.attrib:
					play = self.extract_bool_attribute(xml_tag,"play",play)
					if play:
						audiosource.play()

				audiosource.min_distance = 8.0
				audiosource.relative_to_listener = False

	def process_properties(self, node, nodexml, call_func):
		
		self.write_path(nodexml, "filename", node.buffer.filepath.string())
		self.write_float(nodexml, "volume", node.volume)
		self.write_bool(nodexml, "loop", node.loop)