#pragma once



namespace Utlop {

	enum Geo_Type {
		GEO_CUBE,
		GEO_TRIANGLE
	};

	class geometric_object
	{
		public:
			void init(Utlop::Geo_Type type);
			void showVertices();

		private:

			float* vertices;
			float* normals;
			Geo_Type g_type;
	};
}


