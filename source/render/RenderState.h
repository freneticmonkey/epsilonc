#pragma once

#include <list>
#include <memory>
#include "math/Matrix.h"
#include "render/Light.h"

class RenderState
{
private:
	struct private_struct {} ;
public:

	typedef std::shared_ptr<RenderState> Ptr;

	static RenderState::Ptr Create();
	static RenderState::Ptr Create(Matrix4 newModel, 
								   Matrix4 newView, 
								   Matrix4 newProj);

	explicit RenderState(const private_struct &) {}
	RenderState(const private_struct &, 
				Matrix4 newModel, 
				Matrix4 newView, 
				Matrix4 newProj)
	{
		model = newModel;
		view = newView;
		projection = newProj;
	}
	~RenderState() {}

	Matrix4		model;
	Matrix4		view;
	Matrix4		projection;
	LightList	lights;
};

typedef std::list<RenderState::Ptr> RenderStateList;

/**
 * Manages a list of RenderStates in a similar fashion as immediate mode OpenGL.
 */
class RenderStateStack
{
private:
	struct private_struct {} ;

public:
	typedef std::shared_ptr<RenderStateStack> Ptr;

	static RenderStateStack::Ptr Create();

	explicit RenderStateStack(const private_struct &)
	{
		// Create a default state
		stack.push_back(RenderState::Create());
	}
	~RenderStateStack() {}

	// Copy the current state to a new state and put it as the head
	// TODO: Possibly extend this to take parameters if all values need to be changed?
	void Push();

	// Drop the current state
	void Pop();

	// Remove all states to a default
	void Reset();

	RenderState::Ptr State();

private:
	RenderStateList stack;
};