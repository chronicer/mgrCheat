#include <Windows.h>
#include "imgui/imgui.h"
#include "KeyBind.h"

void KeyBind::Hotkey(const char* label, unsigned int* k) noexcept
{
	static bool waitingforkey = false;
	ImGui::TextUnformatted(label);
	ImGui::SameLine();
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames[*k]))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("...");
		Sleep(20);
		for (auto& Key : KeyCodes)
		{
			if ((ImGui::IsKeyPressed(VK_LBUTTON)) || (ImGui::IsKeyPressed(VK_RBUTTON)))
				continue;

			if (ImGui::IsKeyPressed(VK_ESCAPE))
			{
				*k = KeyCodes[0];
				waitingforkey = false;
				break;
			}

			if (ImGui::IsKeyPressed(Key)) {
				*k = Key;
				waitingforkey = false;
				break;
			}
		}
	}
}

bool KeyBind::IsKeyPressed(unsigned int k) noexcept
{
	if (k == KeyCodes[0])
		return false;

	if (ImGui::IsKeyPressed(k, false))
		return true;
	return false;
}