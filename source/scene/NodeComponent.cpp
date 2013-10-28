#include "scene/NodeComponent.h"

namespace epsilon
{
	NodeComponent::NodeComponent(string className) : Object(className)
	{
	}

	NodeComponent::~NodeComponent(void)
	{
	}

	//void NodeComponent::SetNode(Node::Ptr theNode)
	void NodeComponent::SetNode(Object * theNode)
	{
		node = theNode;
		//transform = theNode->Transform();
		//renderer = theNode->Renderer();
	}

}