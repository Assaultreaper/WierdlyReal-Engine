#include "GlobalSettings.h"

const char* GlobalSettings::GetCameraLocation(const Camera& camera, const bool inUse)
{
    std::stringstream Title;

    if (inUse == true)
    {
        Title << "Renderer : Camera Location = X : " << camera.Position.x << " Y : " << camera.Position.y << " Z : " << camera.Position.z;
        return Title.str().c_str();
    }
    else
    {
        Title << "Renderer";
        return Title.str().c_str();
    }
}
