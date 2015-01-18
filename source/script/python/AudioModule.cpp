//
//  AudioModule.cpp
//  Epsilon
//
//  Created by Scott Porter on 15/05/2014.
//

#include "script/python/AudioModule.h"

#include "audio/AudioSource.h"
#include "audio/AudioListener.h"

#include "resource/Resource.h"

void initAudio()
{
	using namespace epsilon;
    
	// make Audio package
	object audioModule( handle<>( borrowed( PyImport_AddModule("epsilon.audio") ) ) );
	scope().attr("audio") = audioModule;
    
	scope audioScope = audioModule;
    
	enum_<sf::Sound::Status>("Status")
		.value("STOPPED", sf::Sound::Status::Stopped)
		.value("PAUSED", sf::Sound::Status::Paused)
		.value("PLAYING", sf::Sound::Status::Playing)
		;
	
	class_<AudioBuffer, bases<Resource>, AudioBuffer::Ptr, boost::noncopyable>("AudioBuffer", no_init)

		;

	class_<AudioSource, AudioSource::Ptr, boost::noncopyable>("AudioSource", no_init)
        .add_property("active", &AudioSource::IsActive, &AudioSource::SetActive )
        
        .def("play", &AudioSource::Play)
        .def("pause", &AudioSource::Pause)
        .def("stop", &AudioSource::Stop)
        
        //.def_readonly("status", &AudioSource::GetStatus)
		.add_property("status", &AudioSource::GetStatus)
        
        .add_property("loop", &AudioSource::GetLoop, &AudioSource::SetLoop)
        .add_property("playing_offset", &AudioSource::GetPlayingOffset, &AudioSource::SetPlayingOffset)
        .add_property("pitch", &AudioSource::GetPitch, &AudioSource::SetPitch)
        .add_property("volume", &AudioSource::GetVolume, &AudioSource::SetVolume)
        .add_property("min_distance", &AudioSource::GetMinDistance, &AudioSource::SetMinDistance)
        .add_property("attenuation", &AudioSource::GetAttenuation, &AudioSource::SetAttenuation)
        .add_property("relative_to_listener", &AudioSource::IsRelativeToListener, &AudioSource::SetRelativeToListener)
        
        .def_readonly("buffer", &AudioSource::GetBuffer)
     ;
	
	class_<AudioSources>("AudioSources")
		.def("__iter__", python::iterator<AudioSources>())
		.def("__len__", &AudioSources::size)
		;
     
    class_<AudioListener, AudioListener::Ptr, boost::noncopyable>("AudioListener", no_init)
    
        .add_property("volume", &AudioListener::GetGlobalVolume, &AudioListener::SetGlobalVolume)
     ;
}