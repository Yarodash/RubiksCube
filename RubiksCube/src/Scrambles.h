#include <random>
#include <vector>
#include <string>


namespace Scrambles 
{
	enum axis { R, L, U, D, F, B };

	struct Rotation {
		axis _axis;
		int count;

		Rotation(axis _axis, int count) {
			this->_axis = _axis;
			this->count = count;
		}
	};

	class RotationList {

	private:
		std::vector<Rotation> list;
		int index;

	public:
		RotationList() {
			list = std::vector<Rotation>();
			index = 0;
		}

		void add(Rotation rotation) {
			list.push_back(rotation);
		}

		Rotation next() {
			if (!has_more())
				index = 0;
			
			return list[index++];
		}

		bool has_more() {
			return (index < list.size());
		}

		void reset_index() {
			index = 0;
		}
	};

	RotationList* generateScramble(int length) 
	{
		int last_glob_axis = -1; // 0 - RL, 1 - UD, 2 - FB
		RotationList* list = new RotationList();

		for (int i = 0; i < length; i++) 
		{
			axis _axis = axis(rand() % 6);
			int count = rand() % 3 + 1;

			int glob_axis = _axis / 2;

			if (glob_axis == last_glob_axis) {
				i--;
				continue;
			}

			last_glob_axis = glob_axis;

			list->add(Rotation(_axis, count));
		}

		return list;
	}

	std::string rotation_to_str(Rotation rotation) 
	{
		std::string axis_name, rotation_name;

		switch (rotation._axis) {
			case axis::R: axis_name = "R"; break;
			case axis::L: axis_name = "L"; break;
			case axis::U: axis_name = "U"; break;
			case axis::D: axis_name = "D"; break;
			case axis::F: axis_name = "F"; break;
			case axis::B: axis_name = "B"; break;
		}

		switch (rotation.count) {
			case 1: rotation_name = ""; break;
			case 2: rotation_name = "2"; break;
			case 3: rotation_name = "'"; break;
		}

		return axis_name + rotation_name;
	}

	std::string rotation_list_to_str(RotationList* rotation_list) 
	{
		std::string result;
		rotation_list->reset_index();

		while (rotation_list->has_more()) 
		{
			Rotation rotation = rotation_list->next();
			result += rotation_to_str(rotation) + " ";
		}

		result.pop_back();
		return result;
	}
}
