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

static bool initialised = false;

ScriptManager * SetupSM()
{
	ResourceManager * rm = &ResourceManager::GetInstance();
	if (!initialised)
	{
		rm->SetBasePath("resources_unittest");
		rm->BuildResourceInfo();
	}

	ScriptManager * sm = &ScriptManager::GetInstance();
	if (!initialised)
	{
		sm->SetScriptsFolderPath("scripts/");
		sm->SetCoreScript("core/unittest_main.py");
		sm->Setup();
	}

	initialised = true;

	return sm;
}

TEST(Script, ScriptManagerInit)
{
	ScriptManager * sm = SetupSM();
	EXPECT_TRUE(sm != nullptr);
}

TEST(Script, ScriptLoad)
{
	ScriptManager * sm = &ScriptManager::GetInstance();

	ScriptBehaviour::Ptr behav = sm->CreateBehaviour("TestClass.py");
	EXPECT_TRUE(behav != nullptr);
}

/*
TEST(Script, ScriptStartCalled)
{
	ScriptManager * sm = &ScriptManager::GetInstance();
	ScriptBehaviour::Ptr behav = sm->CreateBehaviour("TestStart.py");
	EXPECT_TRUE(behav != nullptr);

	// Tick the start function to trigger the initial start
	sm->Update(0.1f);

	// Check that start was called on the first update
	EXPECT_EQ(true, behav->GetUnitTestResult());
}


TEST(Script, ScriptUpdateCalled)
{
	ScriptManager * sm = &ScriptManager::GetInstance();
	ScriptBehaviour::Ptr behav = sm->CreateBehaviour("TestUpdate.py");
	EXPECT_TRUE(behav != nullptr);

	// Tick the start function to trigger the initial start
	sm->Update(0.1f);

	// And again to fire a call to update 
	sm->Update(0.1f);

	// Check that start was called on the first update
	EXPECT_EQ(true, behav->GetUnitTestResult());
}
*/