#pragma once

#include <gtest/gtest.h>

#include <logging/Logging.h>

#include <resource/Resource.h>
#include <resource/ResourceManager.h>

using namespace epsilon;

TEST(Resource, ResourceManager)
{
	ResourceManager * rm = &ResourceManager::GetInstance();

}