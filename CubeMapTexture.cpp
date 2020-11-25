#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture(vector<std::string> textureFaceFiles)
{
	// Bind texture id
	glGenTextures(1, &TEXTURE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE);

	// Load separate texture faces
	int width, height, numChannels;
	unsigned char* textureData;
	for (unsigned int i = 0; i < textureFaceFiles.size(); i++) {
		textureData = stbi_load(textureFaceFiles[i].c_str(), &width, &height, &numChannels, 0);
		if (textureData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		}
		else {
			std::cerr << "Cubemap texture failed to load at path: " << textureFaceFiles[i] << std::endl;
		}
		stbi_image_free(textureData);
	}
	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

CubeMapTexture::~CubeMapTexture()
{
	glDeleteTextures(1, &TEXTURE);
}

void CubeMapTexture::sendTextureToShader(GLuint shader) {
	glUseProgram(shader);

	// Bind textures
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE);

	glUseProgram(0);
}
