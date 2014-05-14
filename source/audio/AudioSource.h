#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include <SFML/Audio/Sound.hpp>

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
        void 	play ();
        void 	pause ();
        void 	stop ();
        
        sf::Sound::Status 	getStatus () const;
        
        // Only for Sounds?
        void 	setLoop (bool loop);
        bool 	getLoop () const;
        
        void 	setPlayingOffset (float timeOffset);
        float 	getPlayingOffset () const;
        
        void 	setPitch (float pitch);
        float 	getPitch () const;
        
        void 	setVolume (float volume);
        float 	getVolume () const;
        
        void 	setPosition (float x, float y, float z);
        void 	setPosition (const Vector3 &position);
        Vector3 getPosition () const;
        
        void 	setMinDistance (float distance);
        float 	getMinDistance () const;
        
        void 	setAttenuation (float attenuation);
        float 	getAttenuation () const;
        
        void 	setRelativeToListener (bool relative);
        bool 	isRelativeToListener () const;
        
        

	private:
		void Setup();
		
		bool	active;

		Transform::Ptr transform;
	};
}