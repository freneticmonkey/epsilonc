#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include "audio/AudioBuffer.h"

#include <SFML/Audio/Sound.hpp>

namespace epsilon
{
	class AudioSource :
        public NodeComponent
	{
	protected:
		struct private_struct {};

	public:
		typedef std::shared_ptr<AudioSource> Ptr;

		static AudioSource::Ptr Create();
		static AudioSource::Ptr Create(std::string name);

		explicit AudioSource(const private_struct &);
		explicit AudioSource(const private_struct &, std::string name);
		~AudioSource(void) {}

		void OnSetParent();
        
        bool IsActive() { return active; }
		void SetActive(bool isActive) { active = isActive; }

		void	Update();

		void 	Play();
		void 	Pause();
		void 	Stop();

		sf::Sound::Status GetStatus() const;

		// Only for Sounds?
		void 	SetLoop(bool loop);
		bool 	GetLoop() const;

		void 	SetPlayingOffset(int timeOffset);
		int 	GetPlayingOffset() const;

		void 	SetPitch(float pitch);
		float 	GetPitch() const;

		void 	SetVolume(float volume);
		float 	GetVolume() const;

		//void 	SetPosition(float x, float y, float z);
		//void 	SetPosition(const Vector3 &position);
		//Vector3 GetPosition() const;

		void 	SetMinDistance(float distance);
		float 	GetMinDistance() const;

		void 	SetAttenuation(float attenuation);
		float 	GetAttenuation() const;

		void 	SetRelativeToListener(bool relative);
		bool 	IsRelativeToListener() const;
        
		void	SetBuffer(AudioBuffer::Ptr buffer);
		void	SetBuffer(std::string bufferPath);

		AudioBuffer::Ptr GetBuffer() { return audioBuffer; }

		// These functions are not a standard pause, they are used
		// to handle buffers changing while the sound is active.
		void SuspendPlayback();
		void ResumePlayback();

		bool IsSuspended() { return suspended; }

	private:
		void Setup();
		
        bool				active;

		// Suspend members
		bool				suspended;
		int					suspendTime;
		sf::Sound::Status	suspendStatus;

		// The SFML Sound object.
		sf::Sound			sound;
		AudioBuffer::Ptr	audioBuffer;

		Transform::Ptr		transform;
	};
    
    typedef std::vector<AudioSource::Ptr> AudioSources;
}