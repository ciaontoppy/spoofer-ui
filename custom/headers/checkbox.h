#pragma once
#include "main.h"
#include "!items.h"

struct j {
	float time = ImGui::GetIO().DeltaTime * 3;
	float circle_size;
	float alpha;

	ImVec4 text;
};
