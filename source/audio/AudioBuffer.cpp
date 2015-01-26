//
//  AudioBuffer.cpp
//  Epsilon
//
//  Created by Scott Porter on 14/05/2014.
//

#include "AudioBuffer.h"

#include "resource/ResourceManager.h"
#include <boost/format.hpp>

namespace epsilon
{
	using namespace boost;
    
	AudioBuffer::Ptr AudioBuffer::Create()
	{
		return std::make_shared<AudioBuffer>(private_struct());
	}
    
	AudioBuffer::Ptr AudioBuffer::CreateFromFile(std::string filename)
	{
		return std::make_shared<AudioBuffer>(private_struct(), filename);
	}
    
	AudioBuffer::AudioBuffer(const private_struct &) : Resource("", ResourceType::Type::SOUND),
    active(false),
    loaded(false)
	{
        
	}
    
	AudioBuffer::AudioBuffer(const private_struct &, std::string filename) : Resource(filename, ResourceType::Type::SOUND),
    active(false),
    loaded(false)
	{
		// Name the AudioBuffer - This should really be done by the AudioBuffer manager because of duplicates
		name = filename;
		
		// strip the path and just use the filename
		unsigned spos = filename.rfind("/");
		if (spos != std::string::npos)
		{
			name = filename.substr(spos + 1, std::string::npos);
		}
	}
    
	AudioBuffer::~AudioBuffer(void)
	{
		
	}
    
	void AudioBuffer::LoadAudioBuffer()
	{
		// Load AudioBuffer
		loaded = soundBuffer.loadFromFile(GetFilepath().GetString());

		// mark the file as reloaded regardless of whether the AudioBuffer was loaded or not.
		SetReloaded();
	}
    
	void AudioBuffer::RefreshFromFile()
	{
		// TODO: Add logic surrounding playing sounds here.
		LoadAudioBuffer();
        
		// mark the file as reloaded regardless of whether the AudioBuffer was loaded or not.
		SetReloaded();
	}
    
}