#pragma once


#include <glad/glad.h>
#include "Object.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

#include <Color.h>
#include "stb_image.h"
using namespace std;

using TextureFormat = GLenum;
using Byte = unsigned char;
using Pixel = unsigned int;

namespace LEapsGL {
	class Texture2D : Object {
	public:
		Texture2D() :ID(0), format(GL_RGB), mipmapCount(0), data(nullptr), height(0), width(0),nrChannels(0){};
		Texture2D(const char* path);
		Texture2D(const Texture2D& rhs): Object(){
			this->ID = rhs.ID;
			format = rhs.format;
			mipmapCount = mipmapCount;
			data = rhs.data;

			int size = rhs.width * rhs.height * rhs.nrChannels;
			Byte* copiedData = new Byte[size];
			std::memcpy(copiedData, rhs.data.get(), size);
			data = std::shared_ptr<Byte>(copiedData, stbi_image_free);
			
			width = rhs.width;
			height = rhs.height;
			nrChannels = rhs.nrChannels;
			textureParams = rhs.textureParams;
		}
		Texture2D(Texture2D&& rhs) noexcept : Texture2D(){
			swap(*this, rhs);
		}
		// Move constructor
		Texture2D& operator=(Texture2D other) {
			swap(*this, other);
			return *this;
		}
		Texture2D& operator=(Texture2D&& other) noexcept {
			swap(*this, other);
			return *this;
		}
		friend void swap(Texture2D& lhs, Texture2D& rhs) {
			using std::swap;
			swap(lhs.data, rhs.data);
			swap(lhs.format, rhs.format);
			swap(lhs.height, rhs.height);
			swap(lhs.width, rhs.height);
			swap(lhs.ID, rhs.ID);
			swap(lhs.mipmapCount, rhs.mipmapCount);
			swap(lhs.nrChannels, rhs.nrChannels);
			swap(lhs.textureParams, rhs.textureParams);
		}

		// upload current image texture buffer CPU to GPU
		void Apply();
		void Load(const char *);

		void MakeEmptyTexture(int width, int height, int nrChannel);

		void use();
		void SetTextureParam(GLuint key, GLuint value);
		GLuint getID();
		void setID(GLuint id);

		int width, height, nrChannels;
	private:
		GLuint ID;
		GLuint mipmapCount;
		TextureFormat format;
		
		std::shared_ptr<Byte> data;

		std::map<GLuint, GLuint> textureParams;

		static const Texture2D blackTexture;
		static const Texture2D grayTexture;
	};
	GLuint GetTextureImageFormatFromPath(const string path);
	Texture2D InitSimpleTexture(Color);
};

