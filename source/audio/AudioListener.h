#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include <SFML/Audio/Listener.hpp>

namespace epsilon
{
	class AudioListener : public NodeComponent
	{
	protected:
		struct private_struct {};
	public:
		typedef std::shared_ptr<AudioListener> Ptr;

		static AudioListener::Ptr Create();

		explicit AudioListener(const private_struct &);
		AudioListener(const private_struct &, std::string name);
		~AudioListener(void) {}

		void OnSetParent();

		bool IsActive() { return active; }
		void SetActive(bool isActive) { active = isActive; }

		void	Update();

		void	SetGlobalVolume(float volume);
		float	GetGlobalVolume();
		
	private:
		void Setup();

		bool	active;

		sf::Listener	listener;

		Transform::Ptr transform;
	};
}