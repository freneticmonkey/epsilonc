#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

namespace epsilon
{
	class AudioSource : public NodeComponent
	{
	protected:
		struct private_struct {};

	public:
		typedef std::shared_ptr<AudioSource> Ptr;

		static AudioSource::Ptr Create();
		static AudioSource::Ptr Create(std::string name);

		explicit AudioSource(const private_struct &);
		AudioSource(const private_struct &, std::string name);
		~AudioSource(void) {}

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