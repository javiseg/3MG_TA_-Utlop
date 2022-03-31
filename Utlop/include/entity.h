#pragma once
#include <vector>

namespace Utlop {
	using namespace std;

	const int kMaxComponents = 4;

	struct Entity {
		void AddComponent(int comp_id) {
			componentsID_ |= comp_id;
		}
		int componentsID_ = 0;
		int cmp_indx_[kMaxComponents];

	};

}