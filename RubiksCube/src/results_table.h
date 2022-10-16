#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "scrambles.h"
#include "utils.h"

#include <vector>
#include <memory>

struct SingleResult
{
	float time;
	std::shared_ptr<Scrambles::RotationList> scramble;

	SingleResult(float _time, std::shared_ptr<Scrambles::RotationList> _scramble) :
		time(_time), scramble(_scramble) {}
};

class Results 
{
	std::vector<SingleResult> list;

public:
	Results() {
		list = std::vector<SingleResult>();
	}

	void add_new(SingleResult item) 
	{
		list.push_back(item);
	}

	float get_average_time() 
	{
		float time = 0.0f;
		
		for (int i = 0; i < list.size(); i++)
			time += list[i].time;

		return time / list.size();
	}

	void show_table(ImFont* big_font, ImFont* small_font)
	{
		ImGui::PushFont(big_font);

		ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(20, 20));

		if (ImGui::BeginTable("table1", 2, flags))
		{
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
			ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
			ImGui::Text("Average:");

			ImGui::TableNextColumn();
			std::string average_time_string = format_time(get_average_time());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(average_time_string.c_str()).x
				- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
			ImGui::Text(average_time_string.c_str());

			ImGui::EndTable();
		}

		flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("table2", 2, flags))
		{
			ImGui::TableSetupColumn("Num", ImGuiTableColumnFlags_WidthFixed, 150.0f);
			ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupScrollFreeze(0, 1);
			//ImGui::TableHeadersRow();

			for (int row = list.size() - 1; row >= 0; row--)
			{
				ImGui::TableNextRow();				

				ImGui::TableSetColumnIndex(0);

				if (row == list.size() - 1) {
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImVec4(0.2, 0.5, 0.2, 1.0)));
				}

				std::string index_string = std::to_string(row + 1) + ".";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(index_string.c_str()).x
					- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
				ImGui::Text(index_string.c_str());

				/*ImGui::TableNextColumn();
				std::string scramble_string = list[row].scramble.get()->to_str();
				ImGui::Text("%s", scramble_string.c_str());*/

				ImGui::TableNextColumn();
				std::string time_string = format_time(list[row].time);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(time_string.c_str()).x
					- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
				ImGui::Text("%s", time_string.c_str());

				if (row == list.size() - 1) {
					ImGui::PopFont();
					ImGui::PushFont(small_font);
				}
			}

			//
			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
		ImGui::PopFont();
	}
	
};


