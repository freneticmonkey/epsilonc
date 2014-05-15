#include "audio/AudioManager.h"

#include "resource/ResourceManager.h"

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
		// build a regular expression of supported texture formats
		std::string audioRegex;
		std::vector<std::string> exts;
        
        exts.push_back("ogg");
		exts.push_back("wav");
		exts.push_back("flac");
        
		// Pre-allocate the regex assuming a maximum extension length of 5
		audioRegex.reserve(exts.size() * 5);
        
		for (std::vector<std::string>::iterator e = exts.begin(); e != exts.end(); e++)
		{
			audioRegex += (*e);
            
			// If not the last extension, insert a pipe
			if (e < (exts.end() - 1))
			{
				audioRegex += "|";
			}
		}
        
		audioRegex = str(format(".*(%s)$") % audioRegex);
        
		// Search the ResourceManager for all files with supported audio extensions
		ResourceList results = ResourceManager::GetInstance().FindResources(audioRegex);
        
		// For each of the results
		std::for_each(results.begin(), results.end(), [&](Resource::Ptr resource){
            
			// Create a new audio buffer
			AudioBuffer::Ptr newAudioBuffer = AudioBuffer::CreateFromFile(resource->GetFilepath().GetString());
			// Add it to the managed buffers
			buffers[newAudioBuffer->GetName()] = newAudioBuffer;
			
            // Register it for change events
			RegisterResource(newAudioBuffer);
            
		});

		Log("AudioManager", boost::str(format("Registered %d audio files.") % results.size()));

		// Create the single listener for the scene
		listener = AudioListener::Create();
	}

	void AudioManager::Update(float el)
	{
		// Cycle through any sound sources
		std::for_each(sources.begin(), sources.end(), [&](AudioSource::Ptr audioSource){

			// Only update playing active and playing sounds
			if (audioSource->IsActive() && (audioSource->GetStatus() != sf::Sound::Status::Stopped) )
			{
				audioSource->Update();
			}
		});
        
        // Finally update the audio listener
        listener->Update();
	}

	void AudioManager::Destroy()
	{
		
	}

	void AudioManager::RefreshResources(ResourceIdVector resources)
	{
		std::for_each(resources.begin(), resources.end(), [&](std::size_t resourceId){

			// Cycle through any active sounds
			std::for_each(sources.begin(), sources.end(), [&](AudioSource::Ptr audioSource){
				if (audioSource->GetStatus() != sf::Sound::Status::Stopped &&
					audioSource->GetBuffer()->GetResourceId() == resourceId)
				{
					audioSource->SuspendPlayback();
				}
			});
		});

		// Now reload changed AudioBuffers on disk
		std::for_each(buffers.begin(), buffers.end(), [&](std::pair < std::string, AudioBuffer::Ptr> buffer){

			if (std::find(resources.begin(), resources.end(), buffer.second->GetResourceId()) != resources.end())
			{
				buffer.second->RefreshFromFile();
			}
		});

		// Now reactivate any suspended audio files.

		std::for_each(resources.begin(), resources.end(), [&](std::size_t resourceId){

			// Cycle through any suspended sounds
			std::for_each(sources.begin(), sources.end(), [&](AudioSource::Ptr audioSource){
				if (audioSource->IsSuspended() &&
					audioSource->GetBuffer()->GetResourceId() == resourceId)
				{
					audioSource->ResumePlayback();
				}
			});
		});
		
	}

	AudioBuffer::Ptr AudioManager::GetBuffer(std::string path)
	{
		AudioBuffer::Ptr found;

		Buffers::iterator foundBuffer = buffers.find(path);

		if ( foundBuffer != buffers.end())
		{
			found = foundBuffer->second;
		}

		return found;
	}

	AudioSource::Ptr AudioManager::CreateAudioSource(std::string path)
	{
		AudioSource::Ptr newAudioSource = AudioSource::Create();
		
        sources.push_back(newAudioSource);
        
        // Assign a buffer if a path is provided
        AudioBuffer::Ptr buffer = GetBuffer(path);
        if ( buffer )
        {
            newAudioSource->SetBuffer(buffer);
        }
        
		return newAudioSource;
	}
}
