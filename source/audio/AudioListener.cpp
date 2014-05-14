#include "audio/AudioListener.h"

namespace epsilon
{
	AudioListener::Ptr AudioListener::Create()
	{
		AudioListener::Ptr newAudioListener = std::make_shared<AudioListener>(private_struct());
		return newAudioListener;
	}

	AudioListener::AudioListener(const private_struct &) : active(false),
		NodeComponent("AudioListener")
	{
		Setup();
	}

	void AudioListener::OnSetParent()
	{
		if (this->componentParent)
		{
			transform = this->componentParent->GetComponent<Transform>();
		}
		else
		{
			transform = nullptr;
		}
	}

	void AudioListener::Setup()
	{

	}

	void AudioListener::Update()
	{
		if (this->componentParent)
		{
			sf::Listener::setPosition(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);

			Vector3 dir = transform->Forward();
			sf::Listener::setDirection(dir.x, dir.y, dir.z);
		}
	}

	void AudioListener::SetGlobalVolume(float volume)
	{
		sf::Listener::setGlobalVolume(volume);
	}

	float AudioListener::GetGlobalVolume()
	{
		return sf::Listener::getGlobalVolume();
	}

}

