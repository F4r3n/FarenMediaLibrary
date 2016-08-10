#pragma once
namespace fm {
	namespace tg {
		enum TAG
		{
			ALL = 31,
			TAG_DEFAULT = 1,
			TAG_1 = 2,
			TAG_2 = 4,
			TAG_3 = 8,
			TAG_4 = 16,
			NONE = 0

		};
	}

	class Tag
	{


	public:
		Tag();
		~Tag();
		static bool checkTag(int tag1, int tag2) {
			return tag1 & tag2;
		}
	};
}

