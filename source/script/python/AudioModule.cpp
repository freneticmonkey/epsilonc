//
//  AudioModule.cpp
//  Epsilon
//
//  Created by Scott Porter on 15/05/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "script/python/AudioModule.h"

#include "audio/AudioSource.h"
#include "audio/AudioListener.h"

void initAudio()
{
	using namespace epsilon;
    
	// make Audio package
	object audioModule( handle<>( borrowed( PyImport_AddModule("epsilon.audio") ) ) );
	scope().attr("audio") = audioModule;
    
	scope audioScope = audioModule;
    
    class_<AudioSource, AudioSource::Ptr, boost::noncopyable>("AudioSource", no_init)
        .add_property("active", &AudioSource::IsActive, &AudioSource::SetActive )
        
        .def("play", &AudioSource::Play)
        .def("pause", &AudioSource::Pause)
        .def("stop", &AudioSource::Stop)
        
        .def_readonly("status", &AudioSource::GetStatus)
        
        .add_property("loop", &AudioSource::GetLoop, &AudioSource::SetLoop)
        .add_property("playing_offset", &AudioSource::GetPlayingOffset, &AudioSource::SetPlayingOffset)
        .add_property("pitch", &AudioSource::GetPitch, &AudioSource::SetPitch)
        .add_property("volume", &AudioSource::GetVolume, &AudioSource::SetVolume)
        .add_property("min_distance", &AudioSource::GetMinDistance, &AudioSource::SetMinDistance)
        .add_property("attenuation", &AudioSource::GetAttenuation, &AudioSource::SetAttenuation)
        .add_property("relative_to_listener", &AudioSource::IsRelativeToListener, &AudioSource::SetRelativeToListener)
        
        .def_readonly("buffer", &AudioSource::GetBuffer)
     ;
     
    class_<AudioListener, AudioListener::Ptr, boost::noncopyable>("AudioListener", no_init)
    
        .add_property("volume", &AudioListener::GetGlobalVolume, &AudioListener::SetGlobalVolume)
     ;
}