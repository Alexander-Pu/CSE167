#include "Texture.h"

Texture::Texture(std::string fileName)
{
	// Bind texture id
	glGenTextures(1, &TEXTURE);
	glBindTexture(GL_TEXTURE_2D, TEXTURE); // Tell OpenGL which texture to edit
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture edge mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Load separate texture faces
	int width, height, numChannels;
	unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &numChannels, 0);
	if (textureData) {
		glTexImage2D(
			GL_TEXTURE_2D, // Always GL_TEXTURE_2D for image textures
			0, // 0 for now
			GL_RGB, // Format OpenGL uses for image without alpha channel
			width, height, // Width and height
			0, // The border of the image
			GL_RGB, // GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE, // GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
			textureData); // The actual RGB image data
	}
	else {
		std::cerr << "Texture failed to load at path: " << fileName << std::endl;
	}
	stbi_image_free(textureData);

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

Texture::~Texture()
{
	glDeleteTextures(1, &TEXTURE);
}

void Texture::sendTextureToShader(GLuint shader) {
	glUseProgram(shader);

	// Bind textures
	glBindTexture(GL_TEXTURE_2D, TEXTURE);

	glUseProgram(0);
}
