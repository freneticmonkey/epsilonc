#include "audio/AudioSource.h"

namespace epsilon
{
	AudioSource::Ptr AudioSource::Create()
	{
		AudioSource::Ptr newAudioSource = std::make_shared<AudioSource>(private_struct());
		return newAudioSource;
	}

	AudioSource::Ptr AudioSource::Create(std::string name)
	{
		AudioSource::Ptr newAudioSource = std::make_shared<AudioSource>(private_struct(), name);
		return newAudioSource;
	}

	AudioSource::AudioSource(const private_struct &) : active(false),
											 NodeComponent("AudioSource")
	{
		Setup();
	}

	AudioSource::AudioSource(const private_struct &, std::string name) : active(false),
		NodeComponent(name, "AudioSource")
	{
		Setup();
	}

	void AudioSource::OnSetParent()
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

	void AudioSource::Setup()
	{
		
	}

	void AudioSource::Update()
	{
		
	}

}

