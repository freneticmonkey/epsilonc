#include "audio/AudioListener.h"

namespace epsilon
{
	AudioListener::Ptr AudioListener::Create()
	{
		AudioListener::Ptr newAudioListener = std::make_shared<AudioListener>(private_struct());
		return newAudioListener;
	}

	AudioListener::Ptr AudioListener::Create(std::string name)
	{
		AudioListener::Ptr newAudioListener = std::make_shared<AudioListener>(private_struct(), name);
		return newAudioListener;
	}

	AudioListener::AudioListener(const private_struct &) : active(false),
		NodeComponent("AudioListener")
	{
		Setup();
	}

	AudioListener::AudioListener(const private_struct &, std::string name) : active(false),
		NodeComponent(name, "AudioListener")
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

	}

}

