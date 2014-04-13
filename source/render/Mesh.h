#pragma once

#include "EpsilonCore.h"

#include "render/VertexData.h"

namespace epsilon
{
	class Mesh
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Mesh> Ptr;

		static Mesh::Ptr Create();
		
		VertexData::Ptr VertexData() { return vd; }

		bool Draw();
		bool DrawOk() { return drawOk; }
        void Destroy();

		explicit Mesh(const private_struct &);
		~Mesh(void);

	private:
		VertexData::Ptr vd;
		bool			drawOk;
	};

}