#include "render/RenderState.h"

RenderState::Ptr RenderState::Create()
{
	return std::make_shared<RenderState>(private_struct());
}

RenderState::Ptr RenderState::Create(Matrix4 newModel, 
								Matrix4 newView, 
								Matrix4 newProj)
{
	return std::make_shared<RenderState>(private_struct(), newModel, newView, newProj);
}

RenderStateStack::Ptr RenderStateStack::Create()
{
	return std::make_shared<RenderStateStack>(private_struct());
}

void RenderStateStack::Push() 
{
	RenderState::Ptr current = stack.back();
	stack.push_back(RenderState::Create(current->model, current->view, current->projection));
}

// Drop the current state
void RenderStateStack::Pop() 
{
	stack.pop_back();
}

// Remove all states to a default
void RenderStateStack::Reset()
{
	stack.empty();
	stack.push_back(RenderState::Create());
}

RenderState::Ptr RenderStateStack::State() 
{ 
	return stack.back(); 
}