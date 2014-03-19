#include "script/python/RenderModule.h"

#include "script/ScriptCommon.h"

#include "math/Vector.h"
#include "scene/NodeComponent.h"

#include "render/RenderManager.h"

#include "render/Camera.h"
#include "render/Colour.h"
#include "render/Renderer.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include "render/MeshFactory.h"
#include "render/VertexData.h"

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

void initRender()
{
	// make Render package
	object renderModule( handle<>( borrowed( PyImport_AddModule("epsilon.render") ) ) );
	scope().attr("render") = renderModule;

	scope renderScope = renderModule;

	//register_ptr_to_python<Mesh::Ptr>();

	Mesh::Ptr (*MeshCreateStandard)() = &Mesh::Create;

	class_<Mesh, Mesh::Ptr, boost::noncopyable>("Mesh", no_init)
		.def("create",MeshCreateStandard)
		.staticmethod("create")

		.def("get_vertexdata", &Mesh::VertexData)
	;

	// Register conversion from PyObject to Mesh::Ptr
	//std_shared_ptr_from_python_converter<Mesh>();

	//register_ptr_to_python<Renderer::Ptr>();

	Renderer::Ptr (*RendererCreateStandard)() = &Renderer::Create;
	Renderer::Ptr (*RendererCreateMesh)(Mesh::Ptr) = &Renderer::Create;
	Renderer::Ptr (*RendererCreateMeshMaterial)(Mesh::Ptr, Material::Ptr) = &Renderer::Create;

	class_<Renderer, bases<NodeComponent>, Renderer::Ptr, boost::noncopyable>("Renderer", no_init)
		.def("create",RendererCreateStandard)
		.def("create",RendererCreateMesh)
		.def("create",RendererCreateMeshMaterial)
		.staticmethod("create")

		.def("draw", &Renderer::Draw)
		.add_property("mesh", &Renderer::GetMesh, &Renderer::SetMesh)
		.add_property("material", &Renderer::GetMaterial, &Renderer::SetMaterial)

		.def("set_mesh", &Renderer::SetMesh)
		.def("get_mesh", &Renderer::GetMesh)
	;

	implicitly_convertible<Renderer::Ptr, NodeComponent::Ptr>();
	//implicitly_convertible<NodeComponent::Ptr, Renderer::Ptr>();

	VertexData::Ptr (*VertexDataCreateStandard)() = &VertexData::Create;

	class_<VertexData, VertexData::Ptr, boost::noncopyable>("VertexData", no_init)
		.def("create",VertexDataCreateStandard)
		.staticmethod("create")

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
		.def("create", &Material::Create)
		.staticmethod("create")

		// Colour Information
		.def_readwrite("ambient", &Material::ambient)
		.def_readwrite("diffuse", &Material::diffuse)
		.def_readwrite("specular", &Material::specular)

		// Shader Access
		.add_property("shader", &Material::GetShader, &Material::SetShader)
	;
	
	// LookAt function pointers
	void (Camera::*LookAtTarget)(Vector3) = &Camera::LookAt;
	void (Camera::*LookAtFromTo)(Vector3, Vector3) = &Camera::LookAt;
	void (Camera::*LookAtFromToComp)(float, float, float, float, float, float) = &Camera::LookAt;

	Camera::Ptr (*CreateStandard)() = &Camera::Create;
	Camera::Ptr (*CreateName)(std::string) = &Camera::Create;
	
	class_<Camera, bases<Node>, Camera::Ptr, boost::noncopyable>("Camera", no_init)
		.def("create", CreateStandard)
		.def("create", CreateName)
		.staticmethod("create")

		
		// Look at
		.def("lookat", LookAtTarget)
		.def("lookat", LookAtFromTo)
		.def("lookat", LookAtFromToComp)

		.def("fps", &Camera::FPS)
		.def("get_projection_matrix", &Camera::GetProjectionMatrix)
		.def("get_view_matrix", &Camera::GetViewMatrix)

		.def("screen_to_world", &Camera::ScreenToWorldCoordinate)
		.def("world_to_screen", &Camera::WorldToScreenCoordinate)
	;
	implicitly_convertible<Camera::Ptr, Node::Ptr>();

	object renderConstModule(handle<>(borrowed(PyImport_AddModule("epsilon.render.const"))));
	renderScope.attr("const") = renderConstModule;
	scope constScope = renderConstModule;

	// Add Render Constants
	constScope.attr("WIDTH") = RenderManager::GetInstance().GetResolution().x;
	constScope.attr("HEIGHT") = RenderManager::GetInstance().GetResolution().y;


}