//
//  AudioBuffer.cpp
//  Epsilon
//
//  Created by Scott Porter on 14/05/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
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
		
		filename = ResourceManager::GetInstance().GetResourceFullPath(filename);
		UpdateResourceFilename(filename);
	}
    
	AudioBuffer::~AudioBuffer(void)
	{
		// Ensure we cleanup one of the only raw pointers we have.
		if (loaded && AudioBufferData)
		{
			delete AudioBufferData;
		}
        
		// Cleanup buffer
		if ( AudioBufferId != 0 )
		{
			glDeleteAudioBuffers(1, &AudioBufferId);
			AudioBufferId = 0;
		}
	}
    
	bool AudioBuffer::SetAudioBufferLoader(AudioBufferLoaderInterface::Ptr texLoader)
	{
		bool result = false;
		if (texLoader->CanLoad(GetFilepath().GetString()))
		{
			AudioBufferLoader = texLoader;
			result = true;
		}
		return result;
	}
    
	void AudioBuffer::LoadAudioBuffer()
	{
		// Load AudioBuffer
		if (AudioBufferLoader)
		{
			// Read the file from disk
			AudioBufferData = AudioBufferLoader->LoadAudioBuffer(GetFilepath().GetString());
            
			if (AudioBufferData != NULL)
			{
				loaded = true;
			}
            
			// mark the file as reloaded regardless of whether the AudioBuffer was loaded or not.
			SetReloaded();
		}
	}
    
	void AudioBuffer::BindAudioBufferData()
	{
		if (AudioBufferData && AudioBufferData->IsValid())
		{
			// If the data is new or has changed dimensions
			if (AudioBufferData->NeedNewBuffer())
			{
				// Clean up the existing AudioBuffer
				if (AudioBufferId != 0)
				{
					glDeleteAudioBuffers(1, &AudioBufferId);
					AudioBufferId = 0;
					CheckOpenGLError("Deleting AudioBuffer");
				}
                
				// Generate and Bind a new AudioBuffer
				glGenAudioBuffers(1, &AudioBufferId);
				CheckOpenGLError("Generating AudioBuffer");
			}
			
			glBindAudioBuffer(GL_AudioBuffer_2D, AudioBufferId);
			CheckOpenGLError("Binding AudioBuffer Id");
            
			glTexImage2D(GL_AudioBuffer_2D,		 // GLenum target
                         0,							 // GLint level
                         GL_RGB, 					 // GLint internalformat
                         AudioBufferData->GetWidth(),	 // GLsizei width
                         AudioBufferData->GetHeight(),	 // GLsizei height
                         0,  						 // GLint border
                         AudioBufferData->GetFormat(),	 // GLenum format
                         GL_UNSIGNED_BYTE, 			 // GLenum type
                         AudioBufferData->GetPointer()	 // const GLvoid *pixels
                         );
            
			CheckOpenGLError("Loading AudioBuffer Data for:" + name);
            
			glTexParameteri(GL_AudioBuffer_2D, GL_AudioBuffer_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_AudioBuffer_2D, GL_AudioBuffer_MIN_FILTER, GL_NEAREST);
            
			// Mipmapping options
            
			/*
             // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
             glTexParameteri(GL_AudioBuffer_2D, GL_AudioBuffer_MAG_FILTER, GL_LINEAR);
             // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
             glTexParameteri(GL_AudioBuffer_2D, GL_AudioBuffer_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
             // Generate mipmaps, by the way.
             glGenerateMipmap(GL_AudioBuffer_2D);
             */
            
			glBindAudioBuffer(GL_AudioBuffer_2D, 0);
            
			// Mark the AudioBuffer data as read
			AudioBufferData->MarkRead();
		}
	}
    
	void AudioBuffer::RefreshFromFile()
	{
		// Do the refresh
		if (AudioBufferLoader)
		{
			// Load the AudioBuffer data from disk
			AudioBufferLoader->RefreshAudioBuffer(&AudioBufferData, GetFilepath().GetString());
		}
        
		// mark the file as reloaded regardless of whether the AudioBuffer was loaded or not.
		SetReloaded();
	}
    
	void AudioBuffer::RefreshBuffers()
	{
		// If the AudioBuffer needs to be refresh continue.
		if (loaded && AudioBufferData->HasChanged())
		{
			// Push the new AudioBuffer data to OpenGL
			BindAudioBufferData();
		}
	}
    
	AudioBuffer::Ptr AudioBuffer::SetBindLocation(GLint location)
	{
		bindLocation = location;
		return ThisPtr();
	}
    
	// AudioBuffer Control
	bool AudioBuffer::Bind()
	{
		if (bindLocation != -1)
		{
			// Bind the AudioBuffer to the bind location
			glActiveAudioBuffer(GL_AudioBuffer0 + bindLocation);
			glBindAudioBuffer(GL_AudioBuffer_2D, AudioBufferId);
			
			active = CheckOpenGLError("Binding AudioBuffer");
            
			if ( !active )
			{
				Log(str(format("Error Binding AudioBuffer: %s to ID: %d") % name % AudioBufferId));
			}
		}
		return active;
	}
    
	bool AudioBuffer::Disable()
	{
		if (active)
		{
			// We don't need to unbind? as Bind automatically breaks previous bind.
			glActiveAudioBuffer(GL_AudioBuffer0 + bindLocation);
			glBindAudioBuffer(GL_AudioBuffer_2D, 0);
            
			active = false;
		}
        
		return active;
	}
    
	int	AudioBuffer::GetWidth() 
	{
		return (AudioBufferData) ? AudioBufferData->GetWidth() : 0;
	}
    
	int	AudioBuffer::GetHeight() 
	{
		return (AudioBufferData) ? AudioBufferData->GetHeight() : 0;
	}
    
	unsigned int AudioBuffer::GetSize() 
	{
		return (AudioBufferData) ? AudioBufferData->GetSize() : 0;
	}
    
	bool AudioBuffer::OnGPU()
	{
		return (AudioBufferData) ? !AudioBufferData->HasChanged() : true;
	}
}