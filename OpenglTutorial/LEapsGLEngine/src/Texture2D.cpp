#include "Texture2D.h"
#include "stb_image.h"
#include <iostream>
#include <filesystem>

GLuint LEapsGL::GetTextureImageFormatFromPath(const string path) {
	static std::map<string, GLuint> extToFormat = {
		{"jpg", GL_RGB}, {"png", GL_RGBA}
	};
	std::string::size_type idx;
	idx = path.rfind('.');

	if (idx != std::string::npos)
	{
		std::string extension = path.substr(idx + 1);
		auto iter = extToFormat.find(extension);
		if (iter != extToFormat.end()) return iter->second;
	}
	std::cout << "Texture2D:: Format setting fail!!\n";
	return 0;
}
//
//LEapsGL::Texture2D LEapsGL::InitSimpleTexture(LEapsGL::Color color)
//{
//	auto texture = Texture2D();
//	return Texture2D();
//}

LEapsGL::Texture2D::Texture2D(const char* path)
{
	Load(path);
}

void LEapsGL::Texture2D::Apply()
{
	use();
	for (auto iter : textureParams) {
		glTexParameteri(GL_TEXTURE_2D, iter.first, iter.second);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.get());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void LEapsGL::Texture2D::Load(const char* path)
{
	glGenTextures(1, &ID);

	// set the texture wrapping parameters
	SetTextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT);
	SetTextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	SetTextureParam(GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	SetTextureParam(GL_TEXTURE_MAG_FILTER, GL_REPEAT);

	stbi_set_flip_vertically_on_load(true);
	data = std::shared_ptr<Byte>(stbi_load(path, &width, &height, &nrChannels, 0),
		stbi_image_free);
	format = GetTextureImageFormatFromPath(path);

	if (data == nullptr) {
		std::cout << "failed to load texture\n";
	}
}

void LEapsGL::Texture2D::MakeEmptyTexture(int width, int height, int nrChannel)
{
	int size = width * height * nrChannels;
	this->width = width;
	this->height = height;
	this->nrChannels = nrChannel;

	Byte* sample = new Byte[size];
	data = std::shared_ptr<Byte>(sample, stbi_image_free);
}

void LEapsGL::Texture2D::use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void LEapsGL::Texture2D::SetTextureParam(GLuint key, GLuint value)
{
	textureParams[key] = value;
}

GLuint LEapsGL::Texture2D::getID()
{
	return ID;
}

void LEapsGL::Texture2D::setID(GLuint id)
{
	ID = id;
}


