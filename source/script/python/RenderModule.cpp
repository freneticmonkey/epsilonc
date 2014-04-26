#include "script/python/RenderModule.h"

#include "script/ScriptCommon.h"

#include "scene/Object.h"

#include "math/Vector.h"
#include "scene/NodeComponent.h"

#include "render/RenderManager.h"

#include "render/Camera.h"
#include "render/Colour.h"
#include "render/Light.h"
#include "render/Renderer.h"
#include "render/material/Material.h"
#include "render/material/Shader.h"
#include "render/material/ShaderUniform.h"
#include "render/texture/Texture.h"
#include "render/mesh/Mesh.h"
#include "render/mesh/MeshFactory.h"
#include "render/mesh/VertexData.h"

#include "render/gizmos/GizmoCube.h"

// Colour Python Helper Functions
void colour_setitem(Colour& v, int index, float value)
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

float colour_getitem(Colour&v, int index)
{
	int MAX = 4;
	float returnVal = 0.f;
	if ( index < 0 ) 
		index += MAX;
	if (index >= 0 && index < MAX) {
		returnVal = v[index];
	}
	else {
		PyErr_SetString(PyExc_IndexError, "index out of range");
		throw_error_already_set();
	}
	return returnVal;
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	MeshCreate, Mesh::Create, 0, 1
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	VertexDataCreate, VertexData::Create, 0, 1
)

enum OpenGLDrawTypes
{
	TRIANGLES		= GL_TRIANGLES,
	LINES			= GL_LINES,
	POINTS_			= GL_POINTS,
	LINE_STRIP		= GL_LINE_STRIP,
	LINE_LOOP		= GL_LINE_LOOP,
	TRIANGLE_STRIP	= GL_TRIANGLE_STRIP,
	TRIANGLE_FAN	= GL_TRIANGLE_FAN
};

void initRender()
{
	// make Render package
	object renderModule( handle<>( borrowed( PyImport_AddModule("epsilon.render") ) ) );
	scope().attr("render") = renderModule;

	scope renderScope = renderModule;

	enum_<OpenGLDrawTypes>("DrawTypes")
		.value("TRIANGLES", OpenGLDrawTypes::TRIANGLES)
		.value("LINES", OpenGLDrawTypes::LINES)
		.value("POINTS", OpenGLDrawTypes::POINTS_)
		.value("LINE_STRIP", OpenGLDrawTypes::LINE_STRIP)
		.value("LINE_LOOP", OpenGLDrawTypes::LINE_LOOP)
		.value("TRIANGLE_STRIP", OpenGLDrawTypes::TRIANGLE_STRIP)
		.value("TRIANGLE_FAN", OpenGLDrawTypes::TRIANGLE_FAN)
		;

	class_<Mesh, bases<Object>, Mesh::Ptr, boost::noncopyable>("Mesh", no_init)
		//.def("create", &Mesh::Create, MeshCreate() )
		.def("create", &Mesh::Create, (python::arg("type") = (GLenum)(GL_TRIANGLES) ))
		.staticmethod("create")

		.def("get_vertexdata", &Mesh::VertexData)
	;

	Renderer::Ptr (*RendererCreateStandard)() = &Renderer::Create;
	Renderer::Ptr (*RendererCreateMesh)(Mesh::Ptr) = &Renderer::Create;
	Renderer::Ptr (*RendererCreateMeshMaterial)(Mesh::Ptr, Material::Ptr) = &Renderer::Create;

	class_<Renderer, bases<NodeComponent>, Renderer::Ptr, boost::noncopyable>("Renderer", no_init)
		.def("create",RendererCreateStandard)
		.def("create",RendererCreateMesh)
		.def("create",RendererCreateMeshMaterial)
		.staticmethod("create")

		//.def("draw", &Renderer::Draw)
		.add_property("mesh", &Renderer::GetMesh, &Renderer::SetMesh)
		.add_property("material", &Renderer::GetMaterial, &Renderer::SetMaterial)

		.def("set_mesh", &Renderer::SetMesh)
		.def("set_mesh", &Renderer::SetMeshByName)
		.def("get_mesh", &Renderer::GetMesh)
	;

	implicitly_convertible<Renderer::Ptr, NodeComponent::Ptr>();
	
	class_<VertexData, VertexData::Ptr, boost::noncopyable>("VertexData", no_init)
		//.def("create", &VertexData::Create)
		//.staticmethod("create")

		.def("set_vertices", &VertexData::SetVertices)
		.def("set_normals", &VertexData::SetNormals)
		.def("set_colours", &VertexData::SetColours)
		.def("set_tex_coords", &VertexData::SetTexCoords)
		.def("set_indices", &VertexData::SetIndices)

		.def("get_vertex_index", &VertexData::GetVertexIndex)
		.def("get_normal_index", &VertexData::GetNormalIndex)
		.def("get_colour_index", &VertexData::GetColourIndex)
		.def("get_tex_coord_index", &VertexData::GetTexCoordIndex)

		.def("build_buffers", &VertexData::BuildBuffers)
	;

	class_<MeshFactory>("MeshFactory", no_init)
		.def("generate_grid", &MeshFactory::GenerateGrid)
		.staticmethod("generate_grid")
		.def("generate_cube", &MeshFactory::GenerateCube)
		.staticmethod("generate_cube")
		.def("generate_wire_cube", &MeshFactory::GenerateWireCube)
		.staticmethod("generate_wire_cube")
		.def("generate_sphere", &MeshFactory::GenerateSphere)
		.staticmethod("generate_sphere")
		.def("generate_wire_sphere", &MeshFactory::GenerateWireSphere)
		.staticmethod("generate_wire_sphere")
		.def("generate_plane", &MeshFactory::GeneratePlane)
		.staticmethod("generate_plane")
		.def("generate_triangle", &MeshFactory::GenerateTriangle)
		.staticmethod("generate_triangle")
		.def("generate_icohedron", &MeshFactory::GenerateIcoHedron)
		.staticmethod("generate_icohedron")
		.def("generate_octohedron", &MeshFactory::GenerateOctohedron)
		.staticmethod("generate_octohedron")
	;

	
	class_<Colour>("Colour")
		.def(init<float>())
		.def(init<float, float, float, float>())

		.def("from_vector4", &Colour::FromVector4)
		.staticmethod("from_vector4")

		.def_readwrite("r", &Colour::r)
		.def_readwrite("g", &Colour::g)
		.def_readwrite("b", &Colour::b)
		.def_readwrite("a", &Colour::a)

		.def("__str__", &Colour::ToString)
		
		.def("__getitem__", &colour_getitem)
		.def("__setitem__", &colour_setitem)
		
		.def(self == self)
		.def(self != self)

		.def(self == Vector4())
		.def(self != Vector4())
		
		.def("from_hex", &Colour::FromHex)
		.staticmethod("from_hex")

		.def("to_8bit", &Colour::To8bit)

		.def("invert", &Colour::Invert)
		.def("inverted", &Colour::Inverted)

		.def("tint", &Colour::Tint)
		.def("tinted", &Colour::Tinted)

		.def_readonly("RED", &Colour::RED)
		.def_readonly("GREEN", &Colour::GREEN)
		.def_readonly("BLUE", &Colour::BLUE)
		.def_readonly("YELLOW", &Colour::YELLOW)
		.def_readonly("ORANGE", &Colour::ORANGE)
		.def_readonly("CYAN", &Colour::CYAN)
		.def_readonly("PURPLE", &Colour::PURPLE)
		.def_readonly("BLACK", &Colour::BLACK)
		.def_readonly("GREY", &Colour::GREY)
	;
	
	class_<Material, Material::Ptr, boost::noncopyable>("Material", no_init)
		.add_property("name", &Material::GetName)

		// Colour Information
		.def_readwrite("ambient", &Material::ambient)
		.def_readwrite("diffuse", &Material::diffuse)
		.def_readwrite("specular", &Material::specular)
		.def_readwrite("reflectance", &Material::reflectance)

		// Texture Access
		.def("add_texture", &Material::AddTexture)
		.def("add_texture", &Material::AddTextureByName)
		.add_property("textures", &Material::GetTextures)

		// Shader Access
		.add_property("shader", &Material::GetShader, &Material::SetShader)
	;

	// Shader and Shader Uniform access
	enum_<ShaderUniform::OpenGLTypes>("ShaderUniformType")
		.value("INT",		ShaderUniform::OpenGLTypes::INT)
		.value("FLOAT",		ShaderUniform::OpenGLTypes::FLOAT)
		.value("VECTOR2",	ShaderUniform::OpenGLTypes::VECTOR2)
		.value("VECTOR3",	ShaderUniform::OpenGLTypes::VECTOR3)
		.value("VECTOR4",	ShaderUniform::OpenGLTypes::VECTOR4)
		.value("MATRIX",	ShaderUniform::OpenGLTypes::MATRIX)
		;

	class_<ShaderUniform, ShaderUniform::Ptr, boost::noncopyable>("ShaderUniform", no_init)
		.add_property("changed", &ShaderUniform::HasChanged)
		.add_property("type",	 &ShaderUniform::GetType)

		.add_property("float",	 &ShaderUniform::GetFloat, &ShaderUniform::SetFloat)
		.add_property("vector2", &ShaderUniform::GetVector2, &ShaderUniform::SetVector2)
		.add_property("vector3", &ShaderUniform::GetVector3, &ShaderUniform::SetVector3)
		.add_property("vector4", &ShaderUniform::GetVector4, &ShaderUniform::SetVector4)
		.add_property("matrix4", &ShaderUniform::GetMatrix4, &ShaderUniform::SetMatrix4)
	;

	class_<Shader, Shader::Ptr, boost::noncopyable>("Shader", no_init)
		.add_property("name", &Shader::GetName)
		.def("get_uniform", &Shader::GetUniform)
		;

	class_<Camera, bases<NodeComponent>, Camera::Ptr, boost::noncopyable>("Camera", no_init)
		.def("get_projection_matrix", &Camera::GetProjectionMatrix)
		.def("get_view_matrix", &Camera::GetViewMatrix)

		.def("screen_to_world", &Camera::ScreenToWorldCoordinate)
		.def("world_to_screen", &Camera::WorldToScreenCoordinate)
	;
	implicitly_convertible<Camera::Ptr, NodeComponent::Ptr>();

	// Lighting
	enum_<Light::Type>("LightType")
		.value("POINT", Light::Type::POINT)
		.value("SPOT", Light::Type::SPOT)
		.value("DIRECTIONAL", Light::Type::DIRECTIONAL)
		.value("SUN", Light::Type::SUN)
		;

	enum_<Light::ShadowType>("LightShadowType")
		.value("NONE", Light::ShadowType::NONE)
		.value("HARD", Light::ShadowType::HARD)
		.value("SOFT", Light::ShadowType::SOFT)
		;

	class_<Light, bases<NodeComponent>, Light::Ptr, boost::noncopyable>("Light", no_init)
		.def("position", &Light::GetPosition)
		.def("direction", &Light::GetDirection)

		.def_readwrite("ambient", &Light::ambient)
		.def_readwrite("diffuse", &Light::diffuse)
		.def_readwrite("specular", &Light::specular)

		.def_readwrite("attenuation", &Light::attenuation)

		.def_readwrite("stength", &Light::strength)
		.def_readwrite("spot_cutoff", &Light::spotCutoff)
		.def_readwrite("spot_exponent", &Light::spotExponent)

		.def_readwrite("type", &Light::type)
		.def_readwrite("shadow_type", &Light::shadowType)
	;

	class_<Texture, Texture::Ptr, boost::noncopyable>("Texture", no_init)
		.add_property("name", &Texture::GetName)
		.add_property("width", &Texture::GetWidth)
		.add_property("height", &Texture::GetHeight)
		.add_property("size", &Texture::GetSize)
		.add_property("on_gpu", &Texture::OnGPU)
		;

	class_<Textures>("Textures")
		.def("__iter__", python::iterator<Textures>())
		.def("__len__", &Textures::size)
		;

	object renderConstModule(handle<>(borrowed(PyImport_AddModule("epsilon.render.const"))));
	renderScope.attr("const") = renderConstModule;
	scope constScope = renderConstModule;

	// Add Render Constants
	constScope.attr("WIDTH") = RenderManager::GetInstance().GetResolution().x;
	constScope.attr("HEIGHT") = RenderManager::GetInstance().GetResolution().y;


}