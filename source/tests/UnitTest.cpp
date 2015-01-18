// UnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "gtest/gtest.h"

#include "audio/audio.h"
#include "core/core.h"
#include "events/events.h"
#include "logging/logging.h"
#include "math/math.h"
#include "physics/physics.h"
#include "render/render.h"
#include "resource/resource.h"
#include "scene/scene.h"
#include "script/script.h"
#include "tools/tools.h"
#include "ui/ui.h"
#include "utilities/utilities.h"

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

