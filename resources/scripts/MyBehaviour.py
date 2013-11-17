from epsilon import *

from epsilon.math import Vector3
from CycleValue import CycleValue

print "epsilon"
print dir(epsilon)

print "math"
print dir(math)

print "scene"
print dir(scene)

# Example class-less script
spheretrans = None
pos = Vector3.ZERO

cycle = CycleValue()

def on_start():
	sm = epsilon.SceneManager.get_instance()
	scene = sm.current_scene()
	root = scene.root()

	global spheretrans
	spheretrans = root.transform.find_child_with_name("sphere")

def on_update(dt):
	global spheretrans
	global cycle
	
	pos.x = cycle.get_value(dt)

	# Move sphere on X-axis
	spheretrans.set_position(pos)

	#angle = dt * 10 * (pymath.pi / 180.0 )
	#spheretrans.rotate(Vector3.UP, angle)

def on_destroy():
	pass

def on_draw_gizmos():
	pass


