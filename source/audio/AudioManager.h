#pragma once

#include "EpsilonCore.h"

#include "audio/AudioSource.h"
#include "audio/AudioListener.h"

namespace epsilon
{
	class AudioManager
	{
		AudioManager(void);

	public:
		typedef std::vector<AudioSource::Ptr>	Sources;
		typedef std::vector<AudioListener::Ptr>	Listeners;
		//typedef std::vector<SoundBuffer::Ptr>	SoundBuffers;
		//typedef std::vector<Music::Ptr>			Music;

		static AudioManager & GetInstance()
		{
			static AudioManager instance;
			return instance;
		}
		~AudioManager(void);

		void	Setup(void);
		void	Update(float el);
		void	Destroy();

		// Create Managed Items
		AudioSource::Ptr	CreateAudioSource();
		//SoundBuffer::Ptr	CreateBuffer(std::string name);
		//Music::Ptr			CreateMusic(std::string name);

	private:
		Sources          sources;
		Listeners        listeners;
		//SoundBuffers    soundBuffer;
		//Music			music;
	};
}

