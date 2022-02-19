#pragma once
#include <memory>
#include <stdlib.h>
#include "glm/glm.hpp"
#include <vector>

namespace Utlop {

	enum Geo {
		kConst_Triangle,
		kConst_Cube,
		kConst_OBJ
	};

	
	const float TriangleArray[9] = {
		0.2f, -0.5f, 0.0f, 
		-0.2f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};
	const float CubeArray[24] = {
		1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
		-1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1
	};
	const float ColorCubeArray[24] = {
		1,1,1,   1,0,0,   1,1,0,   0,1,0,
	  0,0,1,   1,0,1,   0,0,0,   0,1,1
	};

	class Geometry {
		public:

			Geometry() : id_(0) {
				printf("Constructor");
			}
			Geometry(int id) : id_(id) {
				printf("WithID");
			}
			Geometry(const Geometry& other) {
				id_ = other.id_;
				vertices_ = other.vertices_;
				indices_ = other.indices_;
			}
			~Geometry() {
				vertices_.resize(0);
				vertices_.shrink_to_fit();
				indices_.resize(0);
				indices_.shrink_to_fit();
			}
			inline void setID(int id) {
				id_ = id;
			}
			inline void setVertices(std::vector<float> vertices) {
				vertices_ = vertices;
			}
			inline void setIndices(std::vector<unsigned int> indices) {
				indices_ = indices;
			}


			inline const int getID() const {
				return id_;
			}
			inline const std::vector<float> getVertices() const {
				return vertices_;
			}
			inline const std::vector<unsigned int> getIndices() const {
				return indices_;
			}

		protected:

			int id_;
			std::vector<float> vertices_;
			std::vector<unsigned int> indices_;
	};



}

