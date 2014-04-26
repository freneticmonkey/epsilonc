#include "render/gizmos/GizmoLine.h"
#include "render/mesh/MeshFactory.h"

namespace epsilon
{
	GizmoLine::GizmoLine()
	{
		mesh = MeshFactory::GenerateWireCube();
	}

	GizmoLine::~GizmoLine()
	{
	}

	void GizmoLine::Draw()
	{

		// Use immediate mode to draw lines for now.
		// I wonder if this will work on OSX?
		glBegin(GL_LINES);
		
		std::for_each(operations[currentOperations].begin(), operations[currentOperations].end(), [&](GizmoOperation::Ptr op){
			// Configure material

			// Set Colour
			glColor3f(op->colour.r, op->colour.g, op->colour.b);
			
			glVertex3fv(&op->vector[0].x);
			glVertex3fv(&op->vector[1].x);

			op->MarkRendered();

		});

		glEnd();
	}
}