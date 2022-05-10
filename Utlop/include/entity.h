#pragma once
#include <vector>

namespace Utlop {
	using namespace std;

	static const int kMaxComponents = 7;

	struct Entity {
		
		void AddComponent(int comp_id) {
			componentsID_ |= comp_id;
		}
		int componentsID_ = 0;
		int entityIdx = -1;
		int cmp_indx_[kMaxComponents];

	};

}