#pragma once

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

		std::string to_str()
		{
			std::string axis_name, rotation_name;

			switch (_axis) {
				case axis::R: axis_name = "R"; break;
				case axis::L: axis_name = "L"; break;
				case axis::U: axis_name = "U"; break;
				case axis::D: axis_name = "D"; break;
				case axis::F: axis_name = "F"; break;
				case axis::B: axis_name = "B"; break;
			}

			switch (count) {
				case 1: rotation_name = ""; break;
				case 2: rotation_name = "2"; break;
				case 3: rotation_name = "'"; break;
			}

			return axis_name + rotation_name;
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

		std::string to_str()
		{
			std::string result;
			reset_index();

			while (has_more())
			{
				Rotation rotation = next();
				result += rotation.to_str() + " ";
			}

			result.pop_back();
			return result;
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
}
