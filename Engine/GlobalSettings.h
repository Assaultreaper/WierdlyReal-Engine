#ifndef _GLOBAL_SETTINGS_H_
#define	_GLOBAL_SETTINGS_H_

#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "Camera.h"

    class GlobalSettings
    {
    public:
#pragma region Window Settings
        glm::vec2 WindowSize = glm::vec2(800, 800);
#pragma endregion

        const char* GetCameraLocation(const Camera& camera, const bool inUse = false);

    };

#endif
