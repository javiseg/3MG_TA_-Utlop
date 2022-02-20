#pragma once
#include <memory>
#include <stdlib.h>
#include "glm/glm.hpp"
#include <vector>

namespace Utlop {

	using namespace std;

	enum Geo {
		kConst_Triangle,
		kConst_Cube,
		kConst_OBJ
	};
	
	const float TriangleArray[] = {
		// first triangle
		 0.0f,  0.5f, 0.0f,  // top 
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  -0.5f, 0.0f,  // bottom left 
	};
	const unsigned int TriangleArrayIndices[] = { 
		0, 1, 2 
	};



	const float CubeArray[] = {
			-0.5f, -0.5f, 0.5f,        // 0
			0.5f, -0.5f, 0.5f,        // 1
			0.5f, 0.5f, 0.5f,            // 2
			-0.5f, 0.5f, 0.5f,         // 3

			0.5f, -0.5f, 0.5f,      // 4
			0.5f, -0.5f, -0.5f,        // 5
			0.5f, 0.5f, -0.5f,        // 6
			0.5f, 0.5f, 0.5f,        // 7

			0.5f, -0.5f, -0.5f,        // 8
			-0.5f, -0.5f, -0.5f,    // 9
			-0.5f, 0.5f, -0.5f,        // 10
			00.5f, 0.5f, -0.5f,        // 11

			-0.5f, -0.5f, -0.5f,    // 12
			-0.5f, -0.5f, 0.5f,      // 13
			-0.5f, 0.5f, 0.5f,        // 14
			-0.5f, 0.5f, -0.5f,        // 15

			0.5f, 0.5f, 0.5f,        // 16
			0.5f, 0.5f, -0.5f,        // 17
			-0.5f, 0.5f, -0.5f,      // 18
			-0.5f, 0.5f, 0.5f,    // 19

			-0.5f, -0.5f, -0.5f,    // 20
			-0.5f, -0.5f, 0.5f,        // 21
			0.5f, -0.5f, 0.5f,        // 22
			0.5f, -0.5f, -0.5f,        // 23
	};
	const float CubeArrayIndices[] = {
			0, 1, 2,      0, 2, 3,      4, 5, 6,
			4, 6, 7,      8, 9, 10,     8, 10, 11,
			12, 13, 14,   12, 14, 15,	  16, 17, 18,
			16, 18, 19,   20, 21, 22,   20, 22, 23 
	};
	const float ColorCubeArray[24] = {
		1,1,1,   1,0,0,   1,1,0,   0,1,0,
	  0,0,1,   1,0,1,   0,0,0,   0,1,1
	};

	class Geometry {
		public:

			Geometry() : id_(0) {
			}
			Geometry(int id) : id_(id) {
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
			inline void setType(Geo type) {
				type_ = type;
			}


			inline const int getID() const {
				return id_;
			}
			inline const Geo getType() const {
				return type_;
			}
			inline const std::vector<float> getVertices() const {
				return vertices_;
			}
			inline const std::vector<unsigned int> getIndices() const {
				return indices_;
			}

			void newGeometryData(Geo type);
			void newGeometryData(char* src);


		protected:

			int id_;
			std::vector<float> vertices_;
			std::vector<unsigned int> indices_;
			Geo type_;
	};



}

