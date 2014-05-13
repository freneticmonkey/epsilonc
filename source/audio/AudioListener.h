#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

namespace epsilon
{
	class AudioListener : public NodeComponent
	{
	protected:
		struct private_struct {};

	public:
		typedef std::shared_ptr<AudioListener> Ptr;

		static AudioListener::Ptr Create();
		static AudioListener::Ptr Create(std::string name);

		explicit AudioListener(const private_struct &);
		AudioListener(const private_struct &, std::string name);
		~AudioListener(void) {}

		void OnSetParent();

		bool IsActive() { return active; }
		void SetActive(bool isActive) { active = isActive; }

		void	Update();

	private:
		void Setup();

		bool	active;

		Transform::Ptr transform;
	};
}