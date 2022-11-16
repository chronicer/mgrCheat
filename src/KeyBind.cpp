#include <Windows.h>
#include "imgui/imgui.h"
#include "KeyBind.h"

// Creates hotkey, for second parameter you need to insert pointer to the key to change
void KeyBind::Hotkey(const char* label, unsigned int* k) noexcept
{
	static bool waitingforkey = false;
	ImGui::TextUnformatted(label);
	ImGui::SameLine();
	char name[128];
	auto scanCode = MapVirtualKey(*k, MAPVK_VK_TO_VSC);
	switch (*k)
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL: case VK_RMENU:
	case VK_LWIN: case VK_RWIN: case VK_APPS:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scanCode |= KF_EXTENDED;
	default:
		GetKeyNameText(scanCode << 16, (LPSTR)name, 128);
	}
	if (!waitingforkey) {
		if (ImGui::Button((*k == 0) ? "None" : name))
			waitingforkey = true;
	}
	else
	{
		ImGui::Button("...");
		Sleep(20);
		for (auto& Key : KeyCodes)
		{
			if (ImGui::IsKeyPressed(VK_LBUTTON) || ImGui::IsKeyPressed(VK_RBUTTON) || ImGui::IsKeyPressed(VK_MENU) /* unexpected random alt presses */)
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

// Checks key press passed in first parameter
bool KeyBind::IsKeyPressed(unsigned int k) noexcept
{
	if (k == KeyCodes[0])
		return false;

	return ImGui::IsKeyPressed(k, false);
}