#pragma once
#include <string>
#include <vector>

namespace Utlop {
	using namespace std;

	class Texture {
		public:
			Texture(const std::string& path);
			~Texture();

			void Bind(unsigned int slot = 0) const;
			void Unbind() const;
	
			const int getWidth() const { return width_; }
			const int getHeight() const { return height_; }

		private:
			unsigned int id_;
			std::string path_;
			unsigned char* text_buffer_;
			int width_;
			int height_;
			int bpp_;
	};

	class TextureCoords {
		public:
			bool setCoordsFromFile(char* path);
			vector<float> getCoords();

		private:
			vector<float> uvs_;
			vector<float> uv_indices_;
	};
}

