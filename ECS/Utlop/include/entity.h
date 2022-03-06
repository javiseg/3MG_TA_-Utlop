#pragma once
#include <vector>

namespace Utlop {
	using namespace std;

	struct Entity {
		void AddComponent(int comp_id) {
			componentsID_ |= comp_id;
		}
		int componentsID_ = 0;
		vector<int> cmp_indx_;

	};

}