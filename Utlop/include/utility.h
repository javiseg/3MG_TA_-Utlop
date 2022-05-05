#pragma once
#include "geometry.h"
#include <map>
#include "glm/gtx/hash.hpp"

namespace std
{
	template <>
	struct hash<Utlop::Vertex>
	{
		size_t operator()(const Utlop::Vertex& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.positions) ^ (hash<glm::vec2>()(vertex.texCoords) << 1)) >> 1);
		}
	};
}
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class UtlopMap {
	std::map<glm::vec3, uint32_t> mymap;
};


