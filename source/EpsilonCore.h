#pragma once

// Visual Leak Detector
// TODO: Add if win def here
//#include <vld.h>

// Epsilon wide includes

// OpenGL / GLEW
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#include <cstdlib>

// STL
#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <vector>

#include <string>

#include "logging/Logging.h"