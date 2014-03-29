#pragma once
//
//  ResourceOwner.h
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//


namespace epsilon
{
    class ResourceOwner
    {
    private:
        
		struct private_struct {};
        
        ResourceOwner(const private_struct &, long nId)
        {
            uID = nId;
        }
        
    public:
        typedef std::shared_ptr<ResourceOwner> Ptr;
        
        static ResourceOwner::Ptr Create(long id);
        
        
        ~ResourceOwner();
        
    public:
        long uID;
    };
}