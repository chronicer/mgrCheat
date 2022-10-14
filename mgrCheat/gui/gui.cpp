#include "gui.h"
#include <Windows.h>
#include "../cheat/cheat.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

void gui::RenderGUI() noexcept
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		show = !show;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show)
	{
		ImGui::Begin("Cheat Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Checkbox("Infinite Fuel Container", &cheat::infiniteFc);
		ImGui::Checkbox("Infinite Health", &cheat::infiniteHealth);
		ImGui::Checkbox("One Hit Kill", &cheat::oneHitKill);
		ImGui::Checkbox("Infinite Subweapon Ammo", &cheat::infiniteSubWeapon);
		ImGui::Checkbox("No Damage Status for battle", &cheat::noDamageStat);
		ImGui::Checkbox("Stealth Cheat", &cheat::stealth);
		ImGui::Checkbox("Height Change (numpad +, -)", &cheat::heightChange);
		ImGui::SliderFloat("Height Change Rate", &cheat::heightRate, 0.0f, 100.0f, "%.3f", 1.0f);
		ImGui::Checkbox("Ground Cheat(hotkey K)", &cheat::groundCheat);
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

}