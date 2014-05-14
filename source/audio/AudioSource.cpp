#include "audio/AudioSource.h"
#include "audio/AudioManager.h"

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

	AudioSource::AudioSource(const private_struct &) : 
		active(false), 
		suspended(false),
		suspendTime(0),
		suspendStatus(sf::Sound::Status::Stopped),
		NodeComponent("AudioSource")
	{
		Setup();
	}

	AudioSource::AudioSource(const private_struct &, std::string name) : 
		active(false), 
		suspended(false),
		suspendTime(0),
		suspendStatus(sf::Sound::Status::Stopped),
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
		sound.setPosition(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);
	}

	void AudioSource::Play()
	{
		// If the AudioBuffer hasn't been loaded yet, load it before playing
		if (!audioBuffer->IsLoaded())
		{
			// Assign the audio buffer to the sound object, 
			// late binding to prevent unnecessary loading
			audioBuffer->RefreshFromFile();
			sound.setBuffer(audioBuffer->GetBuffer());
		}

		sound.play();
	}

	void AudioSource::Pause()
	{
		sound.pause();
	}

	void AudioSource::Stop()
	{
		sound.stop();
	}

	sf::Sound::Status AudioSource::GetStatus() const
	{
		return sound.getStatus();
	}

	// Only for Sounds?
	void AudioSource::SetLoop(bool loop)
	{
		sound.setLoop(loop);
	}

	bool AudioSource::GetLoop() const
	{
		return sound.getLoop();
	}

	void AudioSource::SetPlayingOffset(int timeOffset)
	{
		sound.setPlayingOffset(sf::milliseconds(timeOffset));
	}

	int AudioSource::GetPlayingOffset() const
	{
		return sound.getPlayingOffset().asMilliseconds();
	}

	void AudioSource::SetPitch(float pitch)
	{
		sound.setPitch(pitch);
	}

	float AudioSource::GetPitch() const
	{
		return sound.getPitch();
	}


	void AudioSource::SetVolume(float volume)
	{
		sound.setVolume(volume);
	}

	float AudioSource::GetVolume() const
	{
		return sound.getVolume();
	}

	/*
	void AudioSource::SetPosition(float x, float y, float z)
	{
		sound.setPosition(x,y,z);
	}

	void AudioSource::SetPosition(const Vector3 &position)
	{
		sound.setPosition(position.x, position.y, position.z);
	}
	*/

	Vector3 AudioSource::GetPosition() const
	{
		const sf::Vector3f p = sound.getPosition();
		return Vector3(p.x, p.y, p.z);
	}

	void AudioSource::SetMinDistance(float distance)
	{
		sound.setMinDistance(distance);
	}

	float AudioSource::GetMinDistance() const
	{
		return sound.getMinDistance();
	}

	void AudioSource::SetAttenuation(float attenuation)
	{
		sound.setAttenuation(attenuation);
	}

	float AudioSource::GetAttenuation() const
	{
		return sound.getAttenuation();
	}

	void AudioSource::SetRelativeToListener(bool relative)
	{
		sound.setRelativeToListener(relative);
	}

	bool AudioSource::IsRelativeToListener() const
	{
		return sound.isRelativeToListener();
	}

	void AudioSource::SetBuffer(AudioBuffer::Ptr buffer) 
	{ 
		audioBuffer = buffer; 
	}

	void AudioSource::SetBuffer(std::string bufferPath)
	{
		AudioBuffer::Ptr buffer = AudioManager::GetInstance().GetBuffer(bufferPath);

		if (buffer)
		{
			audioBuffer = buffer;
		}
	}

	void AudioSource::SuspendPlayback()
	{
		// Mark as suspended
		suspended = true;

		// store the current playback time.
		suspendTime = sound.getPlayingOffset().asMilliseconds();
		
		// Store the state of the sound so that when it's reloaded we know how to handle the sound
		suspendStatus = sound.getStatus();

		// stop the playback
		sound.stop();
	}

	void AudioSource::ResumePlayback()
	{
		switch (suspendStatus)
		{
		case sf::Sound::Status::Paused:
			sound.play();
			// skip to the suspend time
			sound.setPlayingOffset(sf::milliseconds(suspendTime));
			break;
		case sf::Sound::Status::Playing:
			// Start playing the sound, but immediately pause
			sound.play();
			sound.pause();

			// skip to the suspend time
			sound.setPlayingOffset(sf::milliseconds(suspendTime));
			break;
		default:
			// Else do nothing
			break;
		}
		
		suspended = false;
	}
}