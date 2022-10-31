#include "gui.h"
#include <Windows.h>
#include <string>
#include "../cheat/cheat.h"
#include "../injector/injector.hpp"
#include "../IniReader.h"
#include "../KeyBinds.h"
#include "../KeyBind.h"
#include "../enums.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

#pragma warning(disable : 4996)

bool once1 = false;
void gui::RenderGUI() noexcept
{
	if (!once1)
	{
		LoadConfig();
		once1 = true;
	}

	if ((KeyBind::IsKeyPressed(menuKey) & 1) && cheat::OnFocus)
		show = !show;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show)
	{
		ImGui::Begin("Cheat Menu", NULL, ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize({width, height});
		ImGuiIO io = ImGui::GetIO();
		io.MouseDrawCursor = true;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		if (ImGui::BeginTabBar("NOTITLE", ImGuiTabBarFlags_NoTooltip))
		{
			if (ImGui::BeginTabItem("Player"))
			{
				ImGui::Checkbox("Infinite Fuel Container", &cheat::infiniteFc);
				ImGui::Checkbox("Infinite Health", &cheat::infiniteHealth);
				ImGui::Checkbox("Infinite Subweapon Ammo", &cheat::infiniteSubWeapon);
				ImGui::Checkbox("Height Change (numpad +, -)", &cheat::heightChange);
				ImGui::SliderFloat("Height Change Rate", &cheat::heightRate, 0.0f, 100.0f, "%.3f", 1.0f);
				KeyBind::Hotkey("Visor Hotkey: ", &cheat::temporaryVisorHotkey);
				ImGui::Checkbox("Auto HP Up", &cheat::autoHpUp);
				if (ImGui::CollapsingHeader("Adjustments")) // uhhhhh
				{
					DWORD player = injector::ReadMemory<DWORD>(cheat::base + 0x19C1490);
					if (ImGui::TreeNode("Blade Mode"))
					{
						if (player)
						{
							if (ImGui::InputFloat("Ripper Zangeki Animation Speed Rate", &cheat::ripperZangekiAnimationSpeedRate, 0.0f, 0.0f, "%.5f", 0))
								injector::WriteMemory<float>(player + 0x4068, cheat::ripperZangekiAnimationSpeedRate);
							if (ImGui::InputFloat("Ripper Zangeki Game Speed Rate", &cheat::ripperZangekiGameSpeedRate, 0.0f, 0.0f, "%.5f", 0))
								injector::WriteMemory<float>(player + 0x406C, cheat::ripperZangekiGameSpeedRate);
							if (ImGui::InputFloat("Zangeki Animation Speed Rate", &cheat::zangekiAnimationSpeedRate, 0.0f, 0.0f, "%.5f", 0))
								injector::WriteMemory<float>(player + 0x4060, cheat::zangekiAnimationSpeedRate);
							if (ImGui::InputFloat("Zangeki Game Speed Rate", &cheat::zangekiGameSpeedRate, 0.0f, 0.0f, "%.5f", 0))
								injector::WriteMemory<float>(player + 0x4064, cheat::zangekiGameSpeedRate);
						}
						ImGui::TreePop();
					}
					if (ImGui::InputFloat("Ninja Run Speed Rate", &cheat::ninjarunSpeedRate))
						injector::WriteMemory<float>(player + 0x53E0, cheat::ninjarunSpeedRate);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Entities"))
			{
				ImGui::Checkbox("Ground Cheat", &cheat::groundCheat);
				if (cheat::groundCheat)
					KeyBind::Hotkey("Hotkey: ", &cheat::groundCheatHotkey);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Enemies"))
			{
				ImGui::Checkbox("One Hit Kill", &cheat::oneHitKill);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Battle"))
			{
				ImGui::Checkbox("No Damage Status for battle", &cheat::noDamageStat);
				ImGui::Checkbox("Stealth Cheat", &cheat::stealth);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Menu"))
			{
				KeyBind::Hotkey("Menu Key: ", &menuKey);
				if (ImGui::Button("Save Config"))
				{
					cheat::SaveConfig();
					SaveConfig();
				}
				if (ImGui::Button("Load Config"))
				{
					cheat::LoadConfig();
					LoadConfig();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void gui::SaveConfig() noexcept
{
	CIniReader iniReader("CheatMenu.ini");

	iniReader.WriteInteger("Menu", "OpenMenuHotkey", menuKey);
}

void gui::LoadConfig() noexcept
{
	CIniReader iniReader("CheatMenu.ini");

	menuKey = iniReader.ReadInteger("Menu", "OpenMenuHotkey", 45);
}