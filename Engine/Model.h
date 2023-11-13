#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "Mesh.h"

class Model
{
public:
	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
};
#endif