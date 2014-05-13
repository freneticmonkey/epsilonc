#include "audio/AudioManager.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	AudioManager::AudioManager(void)
	{
		
	}

	AudioManager::~AudioManager(void)
	{
		
	}

	void AudioManager::Setup(void)
	{
		
	}

	void AudioManager::Update(float el)
	{
		
	}

	void AudioManager::Destroy()
	{
		
	}

	AudioSource::Ptr AudioManager::CreateAudioSource()
	{
		AudioSource::Ptr newAudioSource = AudioSource::Create();
		sources.push_back(newAudioSource);
		return newAudioSource;
	}
}
