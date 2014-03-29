#pragma once
//
//  Resource.h
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

namespace epsilon
{
    class ResourceManager;
 
    enum ResourceType
    {
        TEXT = 1,
        SCRIPT,
        MESH,
        TEXTURE,
        SHADER
    };
    
    class Resource
    {
        friend class ResourceManager;
     private:
        struct private_struct {};
        
        Resource(const private_struct &, long nOwnerId, int iType = ResourceType::TEXT)
        {
            uID = -1;
            ownerId = nOwnerId;
            type = iType;
        }

     public:
        typedef std::shared_ptr<Resource> Ptr;
        
        static Resource::Ptr Create(long owner, int type)
        {
            return std::make_shared<Resource>(private_struct(), owner, type);
        }
        ~Resource();
        
        long GetId() { return uID; }
        int GetType() { return type; }
        
        // Ensure that type based comparison is implemented?
//        virtual int operator=;
        
        // This is used by the ResourceManager to set a unique id for the Resource
    protected:
        void SetId(long nId)
        {
            uID = nId;
        }
        
    private:
        long uID;
        long ownerId;
        int type;
        
    };
}