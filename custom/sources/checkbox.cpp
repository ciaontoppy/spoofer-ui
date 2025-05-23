#include "checkbox.h"

bool items::checkbox(const char* name, ImVec2 pos, bool* v, bool DC)
{
	auto* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) return false;

	const ImGuiID id = window->GetID(name);
	const ImVec2 _pos = DC ? window->DC.CursorPos + pos : window->Pos - window->Scroll + pos;
	const ImVec2 text_size = ImGui::CalcTextSize(name);
	const ImVec2 item_size = ImVec2(300, 25);
	const ImRect total = { _pos, _pos + item_size };

	ImGui::ItemAdd(total, id); if (!ImGui::ItemAdd(total, id)) return false;
	ImGui::PushID(name);

	const bool clicked = ImGui::IsItemClicked();
	const bool hovered = ImGui::IsItemHovered();

	if (clicked) *v = !*v;

	const ImVec2 checkbox_padding = { 0, 0 };
	const ImVec2 checkbox_size = { 20, 20 };
	const ImVec2 checkbox_pos = ImVec2(total.Max.x - checkbox_size.x - checkbox_padding.x, total.Min.y + (total.GetHeight() - checkbox_size.y) / 2 - checkbox_padding.y);
	const ImRect checkbox_total = { checkbox_pos, checkbox_pos + checkbox_size };

	float circle_size = *v ? 30.f : 0.f;
	float alpha = *v ? 1.f : 0.f;
	ImVec4 text = *v ? colors::lwhite : hovered ? colors::gray2 : colors::gray2;

	static std::map<ImGuiID, j> anim;
	auto& w = anim.emplace(id, ::j{ ImGui::GetIO().DeltaTime * 3, circle_size, alpha, text }).first->second;

	w.circle_size = ImLerp(w.circle_size, circle_size, w.time);
	w.alpha = ImLerp(w.alpha, alpha, w.time);
	w.text = ImLerp(w.text, text, w.time);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, w.alpha);
	ImGui::PushClipRect(checkbox_total.Min + ImVec2(1, 1), checkbox_total.Max - ImVec2(1, 0), false);

	window->DrawList->AddCircleFilled(ImVec2(checkbox_total.Min.x, checkbox_total.Max.y), w.circle_size, ImGui::GetColorU32(colors::item_col2));
	ImGui::RenderCheckMark(window->DrawList, ImVec2(checkbox_total.Min + ImVec2(checkbox_total.GetSize().x / 2 - 6, checkbox_total.GetSize().y / 2 - 6)), ImGui::GetColorU32(colors::lwhite), 12);

	ImGui::PopClipRect();
	ImGui::PopStyleVar();

	window->DrawList->AddText(ImVec2(total.Min.x, total.Min.y + (total.GetHeight() - text_size.y) / 2), ImGui::GetColorU32(w.text), name);
	window->DrawList->AddRect(checkbox_total.Min, checkbox_total.Max, ImGui::GetColorU32(colors::dark_gray), 3);
	//window->DrawList->AddRect(total.Min, total.Max, ImGui::GetColorU32(colors::white));

	ImGui::PopID();
	return true;
}