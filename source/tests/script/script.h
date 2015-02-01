#pragma once

#include <gtest/gtest.h>

// OpenGL / GLEW
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#include <boost/python.hpp>

#include <logging/Logging.h>

#include <resource/Resource.h>
#include <resource/ResourceManager.h>

#include <script/Script.h>
#include <script/ScriptBehaviour.h>
#include <script/ScriptManager.h>

using namespace epsilon;

TEST(Script, ScriptManager)
{
	ResourceManager * rm = &ResourceManager::GetInstance();
	rm->SetBasePath("../resources_unittest");

	ScriptManager * sm = &ScriptManager::GetInstance();
	sm->SetScriptsFolderPath("scripts/");
	sm->SetCoreScript("core/unittest_main.py");
	sm->Setup();

	ScriptBehaviour::Ptr behav = sm->CreateBehaviour("TestClass.py");


}