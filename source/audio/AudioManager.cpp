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
        
        exts.push_back("");
        
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
        
		// Search the ResourceManager for all files with supported texture extensions
		ResourceList results = ResourceManager::GetInstance().FindResources(audioRegex);
        
		// For each of the results
		std::for_each(results.begin(), results.end(), [&](Resource::Ptr resource){
            
			// Create a new texture
			Texture::Ptr newTexture = Texture::CreateFromFile(resource->GetFilepath().GetString());
			// Add it to the managed textures
			textures[newTexture->GetName()] = newTexture;
			
            // Register it for change events
			RegisterResource(newTexture);
            
			// Assign it a texture loader
			std::find_if(textureLoaders.begin(), textureLoaders.end(), [&](TextureLoaderInterface::Ptr texLoader){
				return newTexture->SetTextureLoader(texLoader) == true;
			});
            
			//Log("TextureManager", str(format("Registered Texture: %s") % newTexture->GetName()));
		});

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
