from epsilon.ui import IMGUI, WindowOptions
from epsilon.math import Vector3, Vector2, Quaternion
class TransformUI(object):
	
	def __init__(self):
		self._name = None

	def on_start(self):
		self._trans = self.node.transform

		self._options = WindowOptions()
		self._options.is_open = True
		self._options.position = Vector2(10,10)
		self._name = "Node: " + self.node.name

	def on_gui(self):
		if self._name is not None:
			if IMGUI.begin(self._name, self._options):
				self._trans.position = IMGUI.input_float3("position:", self._trans.position)

				ori = IMGUI.input_float3("orientation:", self._trans.orientation.get_euler().to_vector3())
				self._trans.orientation.rotate_euler(ori.x, ori.y, ori.z)

				self._trans.scale = IMGUI.input_float3("scale:", self._trans.scale)
			IMGUI.end()

# It is necessary to instantiate the class as "_instance"
# in Python so that epsilon can hook into it.
_instance = TransformUI()