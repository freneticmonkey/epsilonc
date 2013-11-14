import math as pymath
from epsilon import *

from epsilon.math import Vector3

print "epsilon"
print dir(epsilon)

print "math"
print dir(math)

print "scene"
print dir(scene)

spheretrans = None
pos = Vector3.ZERO

def start():
	print "Starting Script"

	sm = SceneManager.get_instance()
	scene = sm.current_scene()
	root = scene.root()

	print "Root name: " + root.name

	spheretrans = root.transform.find_child_with_name("sphere")

def update(dt):
	# Move sphere on X-axis
	spheretrans.set_position(pos)

	#pos.x += dt
	#angle = dt * 10 * (pymath.pi / 180.0 )
	#spheretrans.rotate(Vector3.UP, angle)

def destroy():
	pass

def draw_gizmos():
	pass