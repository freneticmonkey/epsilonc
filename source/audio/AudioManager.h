#pragma once

#include "EpsilonCore.h"

#include "resource/ResourceOwner.h"

#include "audio/AudioBuffer.h"
#include "audio/AudioSource.h"
#include "audio/AudioListener.h"


namespace epsilon
{
	class AudioManager :
		public ResourceOwner
	{
		AudioManager(void);

	public:
		typedef std::vector<AudioSource::Ptr>			Sources;
		typedef std::map<std::string, AudioBuffer::Ptr> Buffers;

		static AudioManager & GetInstance()
		{
			static AudioManager instance;
			return instance;
		}
		~AudioManager(void);

		void	Setup(void);
		void	Update(float el);
		void	Destroy();

		void RefreshResources(ResourceIdVector resources);

		AudioBuffer::Ptr GetBufferByName(std::string name);
		AudioBuffer::Ptr GetBufferByPath(std::string path);


		// Create Managed Items
		AudioListener::Ptr	GetListener() { return listener; }

		AudioSource::Ptr	CreateAudioSourceByName(std::string name = "");
		AudioSource::Ptr	CreateAudioSourceByPath(std::string path = "");

		//Music::Ptr			CreateMusic(std::string name);

	private:
		Sources				sources;
		Buffers				buffers;

		AudioListener::Ptr  listener;
		
		//Music			music;
	};
}

