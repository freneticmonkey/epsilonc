#include "script/python/MathModule.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Rotation.h"
#include "math/Bounds.h"


// Vector2 Python Helper Functions
void vector2_setitem(Vector2& v, int index, float value)
{
	int MAX = 2;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        v[index] = value;
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

float vector2_getitem(Vector2&v, int index)
{
	int MAX = 2;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        return v[index];
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

// Vector3 Python Helper Functions
void vector3_setitem(Vector3& v, int index, float value)
{
	int MAX = 3;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        v[index] = value;
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

float vector3_getitem(Vector3&v, int index)
{
	int MAX = 3;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        return v[index];
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

// Vector4 Python Helper Functions
void vector4_setitem(Vector4& v, int index, float value)
{
	int MAX = 4;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        v[index] = value;
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

float vector4_getitem(Vector4&v, int index)
{
	int MAX = 4;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        return v[index];
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

// Matrix3 Python Helper Functions
void matrix3_setitem(Matrix3& v, int index, float value)
{
	int MAX = 9;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        v[index] = value;
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

float matrix3_getitem(Matrix3&v, int index)
{
	int MAX = 9;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        return v[index];
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

// Matrix4 Python Helper Functions
void matrix4_setitem(Matrix4& v, int index, float value)
{
	int MAX = 16;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        v[index] = value;
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

float matrix4_getitem(Matrix4&v, int index)
{
	int MAX = 16;
	if ( index < 0 ) 
		index += MAX;
    if (index >= 0 && index < MAX) {
        return v[index];
    }
    else {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        throw_error_already_set();
    }
}

// Quaternion function wrappers

Quaternion (Quaternion::*RotateAxisVF)(Vector3,float) = &Quaternion::RotateAxis;
Quaternion (Quaternion::*RotateAxisAA)(AxisAngle) = &Quaternion::RotateAxis;
Quaternion (Quaternion::*RotateEulerFFF)(float,float,float) = &Quaternion::RotateEuler;
Quaternion (Quaternion::*RotateEulerE)(Euler) = &Quaternion::RotateEuler;

// Expose Math Classes to Python
void initMath()
{
	// make math package
	object mathModule( handle<>( borrowed( PyImport_AddModule("epsilon.math") ) ) );
	scope().attr("math") = mathModule;

	scope mathScope = mathModule;

	class_<Vector2>("Vector2", init< python::optional<float> >())
			.def(init<Vector2>())
			.def(init<float,float>())
	
		.def_readwrite("x", &Vector2::x)
		.def_readwrite("y", &Vector2::y)

		.def("__str__", &Vector2::ToString)
		
		.def("__getitem__", &vector2_getitem)
		.def("__setitem__", &vector2_setitem)
		
		.def(self == self)
		.def(self != self)

		.def(-self)
		.def(self + self)
		.def(self - self)
		
		.def(self * self)

		.def(self * float())
		.def(float() * self)
		.def(self / float())

		.def(self += self)
		.def(self -= self)
		.def(self /= float())
		
		.def("length",&Vector2::Length)
		.def("length_squared", &Vector2::LengthSquared)
		.def("dot", &Vector2::Dot)
		.def("normalise", &Vector2::Normalise)
		.def("normalised", &Vector2::Normalised)

		.def("cross", &Vector2::Cross)
		.def("reflect", &Vector2::Reflect)
		.def("angle", &Vector2::Angle)
		.def("project", &Vector2::Project)

		.def("compute_extremes", &Vector2::ComputeExtremes, return_value_policy<manage_new_object>())
		;

	class_<Vector3>("Vector3", init< python::optional<float> >())
			.def(init<Vector3>())
			.def(init<float,float,float>())
	
		.def_readwrite("x", &Vector3::x)
		.def_readwrite("y", &Vector3::y)
		.def_readwrite("z", &Vector3::z)

		.def("__str__", &Vector3::ToString)
		
		.def("__getitem__", &vector3_getitem)
		.def("__setitem__", &vector3_setitem)
		
		.def(self == self)
		.def(self != self)

		.def(-self)
		.def(self + self)
		.def(self - self)
		
		.def(self * self)

		.def(self * float())
		.def(float() * self)
		.def(self / float())

		.def(self += self)
		.def(self -= self)
		.def(self /= float())
		
		.def("length",&Vector3::Length)
		.def("length_squared", &Vector3::LengthSquared)
		.def("dot", &Vector3::Dot)
		.def("normalise", &Vector3::Normalise)
		.def("normalised", &Vector3::Normalised)

		.def("cross", &Vector3::Cross)
		.def("unit_cross", &Vector3::UnitCross)
		.def("reflect", &Vector3::Reflect)
		.def("rotate_around", &Vector3::RotateAround)
		.def("distance", &Vector3::Distance)
		.def("angle", &Vector3::Angle)
		.def("project", &Vector3::Project)

		.def("compute_extremes", &Vector3::ComputeExtremes, return_value_policy<manage_new_object>())

		.def_readonly("UP", &Vector3::UP)
		.def_readonly("RIGHT", &Vector3::RIGHT)
		.def_readonly("FORWARD", &Vector3::FORWARD)
		.def_readonly("DOWN", &Vector3::DOWN)
		.def_readonly("LEFT", &Vector3::LEFT)
		.def_readonly("BACKWARD", &Vector3::BACKWARD)

		.def_readonly("ZERO", &Vector3::ZERO)
		.def_readonly("ONE", &Vector3::ONE)
		.def_readonly("IDENTITY", &Vector3::IDENTITY)

		.def_readonly("UNIT_Z", &Vector3::UNIT_Z)
		.def_readonly("UNIT_Y", &Vector3::UNIT_Y)
		.def_readonly("UNIT_X", &Vector3::UNIT_X)
		;

	class_<Vector4>("Vector4", init< python::optional<float> >())
			.def(init<Vector4>())
			.def(init<float,float,float,float>())
	
		.def_readwrite("x", &Vector4::x)
		.def_readwrite("y", &Vector4::y)
		.def_readwrite("z", &Vector4::z)
		.def_readwrite("w", &Vector4::w)

		.def("__str__", &Vector4::ToString)
		
		.def("__getitem__", &vector4_getitem)
		.def("__setitem__", &vector4_setitem)
		
		.def(self == self)
		.def(self != self)

		.def(-self)
		.def(self + self)
		.def(self - self)
		
		.def(self * self)

		.def(self * float())
		.def(float() * self)
		.def(self / float())

		.def(self += self)
		.def(self -= self)
		.def(self /= float())
		
		.def("length",&Vector4::Length)
		.def("length_squared", &Vector4::LengthSquared)
		.def("dot", &Vector4::Dot)
		.def("normalise", &Vector4::Normalise)
		.def("normalised", &Vector4::Normalised)

		.def("compute_extremes", &Vector4::ComputeExtremes, return_value_policy<manage_new_object>())
		;
	
	class_<AxisAngle>("AxisAngle", init<Vector3, float>())
		.def("__str__", &AxisAngle::ToString)
		;

	class_<Euler>("Euler", init<float, float, float>());

	class_<Matrix3>("Matrix3")
		.def(init<Matrix3>())
		.def(init<float, float, float, float, float, float, float, float, float>())
		.def(init<Vector3, Vector3, Vector3>())
		.def("__str__", &Matrix3::ToString)
		
		.def("__getitem__", &matrix3_getitem)
		.def("__setitem__", &matrix3_setitem)

		.def(self == self)
		.def(self != self)

		.def(self *= self)

		.def(self * self)
		.def(self * Vector3())
		
		.def("scale", &Matrix3::Scale)
		.def("translate", &Matrix3::Translate)
		.def("rotate", &Matrix3::Rotate)
		
		.def("create_scale", &Matrix3::CreateScale)
		.def("create_translation", &Matrix3::CreateTranslation)
		.def("create_rotation", &Matrix3::CreateRotation)

		.def("determinant", &Matrix3::Determinant)
		.def("inverse", &Matrix3::Inverse)
	;

	class_<Matrix4>("Matrix4")
		.def(init<Matrix4>())
		.def(init<Vector4, Vector4, Vector4, Vector4>())

		.def("__str__", &Matrix4::ToString)
		
		.def("__getitem__", &matrix4_getitem)
		.def("__setitem__", &matrix4_setitem)

		.def(self == self)
		.def(self != self)

		.def(self *= self)

		.def(self * self)
		.def(self * Vector3())
		
		.def("transform", &Matrix4::Transform)
		.def("scale", &Matrix4::Scale)
		.def("translate", &Matrix4::Translate)

		.def("rotate_x", &Matrix4::RotateX)
		.def("rotate_y", &Matrix4::RotateY)
		.def("rotate_z", &Matrix4::RotateZ)

		.def("rotate_axis", &Matrix4::RotateAxis)
		.def("rotate_euler", &Matrix4::RotateEuler)
		.def("rotate_triple_axis", &Matrix4::RotateTripleAxis)

		.def("transpose", &Matrix4::Transpose)
		.def("transposed", &Matrix4::Transposed)

		.def("create_scale", &Matrix4::CreateScale)
		.staticmethod("create_scale")
		.def("create_translation", &Matrix4::CreateTranslation)
		.staticmethod("create_translation")
		.def("create_rotate_x", &Matrix4::CreateRotateX)
		.staticmethod("create_rotate_x")
		.def("create_rotate_y", &Matrix4::CreateRotateY)
		.staticmethod("create_rotate_y")
		.def("create_rotate_z", &Matrix4::CreateRotateZ)
		.staticmethod("create_rotate_z")
		.def("create_rotate_axis", &Matrix4::CreateRotateAxis)
		.staticmethod("create_rotate_axis")
		.def("create_rotate_euler", &Matrix4::CreateRotateEuler)
		.staticmethod("create_rotate_euler")
		.def("create_rotate_triple_axis", &Matrix4::CreateRotateTripleAxis)
		.staticmethod("create_rotate_triple_axis")
		.def("create_rotate_quaternion", &Matrix4::CreateRotateQuaternion)
		.staticmethod("create_rotate_quaternion")
		.def("create_lookat", &Matrix4::CreateLookAt)
		.staticmethod("create_lookat")
		.def("create_perspective", &Matrix4::CreatePerspective)
		.staticmethod("create_perspective")

		.def("determinant", &Matrix4::Determinant)
		.def("inverse", &Matrix4::Inverse)
		.def("get_translation", &Matrix4::GetTranslation)
		.def("get_scale", &Matrix4::GetScale)
		.def("get_rotation", &Matrix4::GetRotation)
	;

	class_<Quaternion>("Quaternion")
		.def(init<Quaternion>())
		.def(init<float, float, float, float>())
		.def(init<Vector3, float>())
		.def(init<float, float, float>())
		.def(init<Matrix4>())

		.def("__str__", &Quaternion::ToString)

		.def_readwrite("x", &Quaternion::x)
		.def_readwrite("y", &Quaternion::y)
		.def_readwrite("z", &Quaternion::z)
		.def_readwrite("w", &Quaternion::w)

		.def(self == self)
		.def(self != self)

		.def(self * self)
		.def(self * Vector3())
		.def(self *= self)

		.def("length", &Quaternion::Length)
		.def("length_squared", &Quaternion::LengthSquared)
		.def("normalise", &Quaternion::Normalise)
		.def("normalised", &Quaternion::Normalised)
		.def("conjugate", &Quaternion::Conjugate)
		.def("conjugated", &Quaternion::Conjugated)
		.def("inverse", &Quaternion::Inverse)

		// Using Custom Overload handlers
		.def("rotate_axis", RotateAxisVF)
		.def("rotate_axis", RotateAxisAA)
		.def("rotate_euler", RotateEulerFFF)
		.def("rotate_euler", RotateEulerE)

		.def("rotate_matrix", &Quaternion::RotateMatrix)

		.def("rotate", &Quaternion::Rotate)
		.def("get_angle_axis", &Quaternion::GetAngleAxis)
		.def("get_euler", &Quaternion::GetEuler)
		.def("get_matrix", &Quaternion::GetMatrix)
		.def("interpolate", &Quaternion::Interpolate)

		.def_readonly("IDENTITY", &Quaternion::IDENTITY)
		.def_readonly("ZERO", &Quaternion::ZERO)
	;

	void(Bounds::*ExtendFloat)(const float&) = &Bounds::Extend;
	void(Bounds::*ExtendVector)(const Vector3&) = &Bounds::Extend;

	class_<Bounds>("Bounds")
		.def(init<Vector3, Vector3>())

		.def_readwrite("centre", &Bounds::centre)
		.def_readwrite("size", &Bounds::size)

		.def("__str__", &Bounds::ToString)

		.def(self + self)
		.def(self + Vector3())
		.def(self += self)

		.def_readonly("min", &Bounds::GetMin)
		.def_readonly("max", &Bounds::GetMax)

		.def("null", &Bounds::IsNull)
		.def("nullify", &Bounds::Nullify)

		.def("overlap", &Bounds::Overlap)

		.def("extend", ExtendFloat)
		.def("extend", ExtendVector)

		.def("intersect", &Bounds::Intersect)

		;
}