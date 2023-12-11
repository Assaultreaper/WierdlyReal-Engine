#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <vector>
#include <string>

#include "Shader.h"
#include "stb/stb_image.h"

class Texture
{
public:
	unsigned int loadTexture(char const* path);
	unsigned int loadCubemap(std::vector<std::string> faces);

private:
};

#endif //!_TEXTURE_H_

