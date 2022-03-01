//#include "texture.h"
//#include "glad/glad.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//Utlop::Texture::Texture(const std::string& path)
//	: id_(0), path_(path), text_buffer_(nullptr), width_(0),
//		height_(0), bpp_(0){
//
//	stbi_set_flip_vertically_on_load(1);
//	text_buffer_ = stbi_load(path.c_str(), &width_, &height_, &bpp_, 4);
//
//	glGenTextures(1, &id_);
//	glBindTexture(GL_TEXTURE_2D, id_);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, text_buffer_);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	if (text_buffer_) {
//		stbi_image_free(text_buffer_);
//	}
//
//}
//
//Utlop::Texture::~Texture()
//{
//	glDeleteTextures(1, &id_);
//}
//
//void Utlop::Texture::Bind(unsigned int slot) const
//{
//	glActiveTexture(GL_TEXTURE0 + slot);
//	glBindTexture(GL_TEXTURE_2D, id_);
//}
//
//void Utlop::Texture::Unbind() const
//{
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//bool Utlop::TextureCoords::setCoordsFromFile(char* path)
//{
//	float max = 1.0f, min = -1.0f;
//
//	FILE* file = fopen(path, "r");
//	if (file == NULL) {
//		printf("Impossible to open the file !\n");
//		return false;;
//	}
//
//	while (1) {
//
//		char lineHeader[128];
//		// Lee la primera palabra de la línea
//		int res = fscanf(file, "%s\n", lineHeader);
//		if (res == EOF)
//			break;
//
//		//printf("%s\n", lineHeader);
//		
//		if (strcmp(lineHeader, "vt") == 0) {
//			float uvX, uvY;
//			fscanf(file, "%f %f\n", &uvX, &uvY);
//			uvs_.push_back(uvX);
//			uvs_.push_back(uvY);
//
//		}
//		else if (strcmp(lineHeader, "f") == 0) {
//			
//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//			if (matches != 9) {
//				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
//				return false;
//			}
//
//			uv_indices_.push_back(uvIndex[0]);
//			uv_indices_.push_back(uvIndex[1]);
//			uv_indices_.push_back(uvIndex[2]);
//		}
//	}
//}
//
//std::vector<float> Utlop::TextureCoords::getCoords()
//{
//	return uvs_;
//}
