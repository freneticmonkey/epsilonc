
from epsilon.math import Vector3

def update(el):

	#print "Hitting Update Test: "
	
	#print "Vector3"
	#print dir(Vector3)

	if not update.called:
		print "epsilon"
		print dir(epsilon)
		print "epsilon.math"
		print dir(epsilon.math)
		print "Vector3"
		print dir(Vector3)
		#print "Here's a vector, if you may: %s" % Vector3(9.0)
		update.called = True
update.called = False