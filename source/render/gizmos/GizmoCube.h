#pragma once
#include "render/gizmos/GizmoType.h"
#include "render/gizmos/GizmoOperation.h"

namespace epsilon
{	
	class GizmoCube : virtual public GizmoType
	{
	private:
		GizmoCube(void);

	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;

		static GizmoCube & GetInstance()
		{
			static GizmoCube instance;
			return instance;
		}
		~GizmoCube(void);
		
		Matrix4 ExtractTransform(GizmoOperation::Ptr op);
	};
}