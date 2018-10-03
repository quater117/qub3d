#pragma once

#include "engine/os.hpp"

/************************************************************************/
/* IO Libraries                                                         */
/************************************************************************/
#include <iostream>
#include <sstream>
#include <fstream>

/************************************************************************/
/* Standard Template Library Tools (put more here when we need them)    */
/************************************************************************/
#include <map>
#include <vector>

#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <qub3d/stb_image.h>

#ifdef QUB3D_OS_WINDOWS
#   define NOMINMAX
#   include <windows.h>
#   undef NOMINMAX
#endif
