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
			if ((GetAsyncKeyState(VK_LBUTTON)) || (GetAsyncKeyState(VK_RBUTTON)))
				continue;

			if (GetAsyncKeyState(VK_ESCAPE))
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