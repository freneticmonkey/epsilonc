//
//  ResourceModule.cpp
//  Epsilon
//
//  Created by Scott Porter on 19/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "script/python/ResourceModule.h"

#include "script/ScriptCommon.h"
#include "resource/Resource.h"
#include "resource/ResourceType.h"
#include "resource/ResourceOwner.h"
#include "script/ScriptManager.h"

void initResource()
{
    using namespace epsilon;
    
	// make Resource package
	object resourceModule( handle<>( borrowed( PyImport_AddModule("epsilon.resource") ) ) );
	scope().attr("resource") = resourceModule;
    
	scope resourceScope = resourceModule;
    
    // Resource Types
    enum_<ResourceType::Type>("ResourceType")
        .value("NONE",  ResourceType::Type::UNKNOWN)
        .value("TEXT", ResourceType::Type::TEXT)
        .value("SCRIPT",  ResourceType::Type::SCRIPT)
        .value("MESH",  ResourceType::Type::MESH)
        .value("TEXTURE",  ResourceType::Type::TEXTURE)
        .value("SHADER",  ResourceType::Type::SHADER)
        .value("SOUND",  ResourceType::Type::SOUND)
	;
    
    class_<Resource, Resource::Ptr, boost::noncopyable>("Resource", no_init)
        .def_readonly("filepath", &Resource::GetFilepath)
        .def_readonly("type", &Resource::GetType)
        .def_readonly("last_modified_time", &Resource::GetLastModified)
        .def_readonly("modified_time", &Resource::GetModifiedTime)
        .def_readonly("need_reload", &Resource::NeedReload)
        
        .def("set_reloaded", &Resource::SetReloaded)
	;
    
    // And the Resource List for ResourceManager::FindResources results
    class_<ResourceList>("ResourceList")
        .def("__iter__", python::iterator<ResourceList>())
    ;
    
    // ResourceIdList for parameter to RefreshResources override
    class_<ResourceIdVector>("ResourceIdVector")
        .def("__iter__", python::iterator<ResourceIdVector>())
    ;
    
    // Class to handle overriding the virtual RefreshResources function
    struct ResourceOwnerCallback : ResourceOwner, wrapper<ResourceOwner>
    {
        void RefreshResources(ResourceIdVector changedResources)
        {
            // Lock the GIL before calling the function
            ScriptManager::GetInstance().LockGIL();
            
            if ( override f = this->get_override("refresh_resources"))
            {
                f(changedResources);
            }
            else
            {
                ResourceOwner::RefreshResources(changedResources);
            }
            
            // Release the GIL
            ScriptManager::GetInstance().ReleaseGIL();
        }
        
        void DefaultRefreshResources(ResourceIdVector changedResources)
        {
            this->ResourceOwner::RefreshResources(changedResources);
        }
    };
    
    // And the base class for ResourceOwners.  Inheriting from this and registering resources
    // will mean that refresh_resources is called when the resource changes on disk.
    class_<ResourceOwnerCallback, boost::noncopyable>("ResourceOwner")
        .def("register_resource", &ResourceOwner::RegisterResource)
        .def("refresh_resources", &ResourceOwner::RefreshResources, &ResourceOwnerCallback::DefaultRefreshResources )
    ;
    
    
    
}