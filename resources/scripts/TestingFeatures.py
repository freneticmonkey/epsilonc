from epsilon import *

from epsilon.render import Colour
from epsilon.math import Vector3
from epsilon.render import MeshFactory, Renderer
from CycleValue import CycleValue

import sys

class TestingMeshGeneration(object):
	
	def __init__(self):
		pass

	def on_start(self):
		#sm = SceneManager.get_instance()
		scene = SceneManager.current_scene
		root = scene.root

		# Create a new node and add it to the scene
		new_node = root.create_child_node()

		new_renderer = Renderer.create(MeshFactory.generate_sphere(16,16))
		new_node.add_component(new_renderer)
		new_node.transform.position = Vector3(-2,1,0)

		second_node = new_node.create_child_node()
		second_node.transform.position = Vector3(0,2,-1)
		second_node.add_component(Renderer.create(MeshFactory.generate_sphere(4,4)))

		#scriptManager = ScriptManager.get_instance()
		behav = ScriptManager.create_behaviour("MyBehaviourClass.py")
		second_node.add_component(behav)

		second_node.renderer.material.diffuse = Colour.YELLOW

		# self._stderr = epsilon.util.PythonLogListener("error")
		# self._stdout = epsilon.util.PythonLogListener()

		# sys.stderr = self._stderr
		# sys.stdout = self._stdout

		print "Testing out from python"



		#self._sphere_trans = root.transform.find_child_with_name("sphere")

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = TestingMeshGeneration()