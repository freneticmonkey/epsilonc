#include "script/python/RenderModule.h"

#include "render/Renderer.h"
#include "render/Mesh.h"
#include "render/VertexData.h"

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

	//register_ptr_to_python<Renderer::Ptr>();

	Renderer::Ptr (*RendererCreateStandard)() = &Renderer::Create;
	Renderer::Ptr (*RendererCreateMesh)(Mesh::Ptr) = &Renderer::Create;

	class_<Renderer, Renderer::Ptr, boost::noncopyable>("Renderer", no_init)
		.def("create",RendererCreateStandard)
		.def("create",RendererCreateMesh)
		.staticmethod("create")

		.def("draw", &Renderer::Draw)
		.def("set_mesh", &Renderer::SetMesh)
		.def("get_mesh", &Renderer::GetMesh)

		.def("set_shader", &Renderer::SetShader)
		.def("get_shader", &Renderer::GetShader)
	;

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



}