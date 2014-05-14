#pragma once

//
//  Audio.h
//  Epsilon
//
//  Created by Scott Porter on 14/05/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "EpsilonCore.h"

#include "resource/Resource.h"
#include <SFML/Audio/SoundBuffer.hpp>

namespace epsilon
{
	class AudioBuffer :
    public std::enable_shared_from_this<AudioBuffer>,
    public Resource
	{
	protected:
		struct private_struct {};
        
	public:
		typedef std::shared_ptr<AudioBuffer> Ptr;
        
		// Use default constructor.
		// Maybe AudioBuffer buffers could be written into by scripts?
		// Not sure how that would work with Resources though.
		static AudioBuffer::Ptr Create();
        
		static AudioBuffer::Ptr CreateFromFile(std::string filename);
        
		// Default constructor.  Maybe AudioBuffer buffers could be written into by scripts?
		explicit AudioBuffer(const private_struct &);
        
		// Constructor for specifying a AudioBuffer from a file.
		explicit AudioBuffer(const private_struct &, std::string filename);
        
		~AudioBuffer(void);
        
		std::string GetName() { return name; }
        
		AudioBuffer::Ptr ThisPtr() { return shared_from_this(); }
        
		// Do initial load of AudioBuffer
		void LoadAudioBuffer();
        
		void RefreshFromFile();
        
		bool IsActive() { return active; }
        
		// Python Info Getters
        float   GetDuration() const;
        
		bool    IsLoaded() { return loaded; }
        
	private:
        
		std::string					name;
		AudioBufferData				  * AudioBufferData;
		AudioBufferLoaderInterface::Ptr AudioBufferLoader;
        
		// 
		bool	active;
		
		bool	loaded;
	};
    
	typedef std::vector<AudioBuffer::Ptr> AudioBuffers;
    
}