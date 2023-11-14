#include "Model.h"

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = getData();

}

std::vector<unsigned char> Model::getData()
{
	std::string byteText;
	std::string uri = JSON["buffers"][0]["uri"];
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	byteText = get_file_contents((fileDirectory + uri).c_str());

	std::vector<unsigned char> data(byteText.begin(), byteText.end());
}
