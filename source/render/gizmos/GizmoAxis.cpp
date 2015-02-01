#include "render/gizmos/GizmoAxis.h"
#include "render/mesh/MeshFactory.h"

namespace epsilon
{
	GizmoAxis::GizmoAxis()
	{
		mesh = MeshFactory::GenerateWireCube();
	}

	GizmoAxis::~GizmoAxis()
	{
	}

	void GizmoAxis::Draw()
	{
		// Use immediate mode to draw lines for now.
		// I wonder if this will work on OSX?

		//glBegin(GL_LINES);

		//std::for_each(operations[currentOperations].begin(), operations[currentOperations].end(), [&](GizmoOperation::Ptr op){
		//	// Configure material

		//	// Set Colour
		//	
		//	// X-axis
		//	glColor3f(1.f, 0.f, 0.f);
		//	glVertex3fv(&op->vector[0].x);

		//	// Y-axis
		//	glColor3f(0.f, 1.f, 0.f);
		//	glVertex3fv(&op->vector[1].x);

		//	// Z-axis
		//	glColor3f(0.f, 0.f, 1.f);
		//	glVertex3fv(&op->vector[2].x);

		//	op->MarkRendered();

		//});

		//glEnd();
	}
}