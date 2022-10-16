#include <Windows.h>
#include "imgui/imgui.h"
#include "KeyBind.h"

void KeyBind::Hotkey(unsigned int* k) noexcept
{
	static bool waitingforkey = false;
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
				waitingforkey = false;
				break;
			}

			if (GetAsyncKeyState(Key) & 1) {
				*k = Key;
				waitingforkey = false;
			}
		}
	}
}