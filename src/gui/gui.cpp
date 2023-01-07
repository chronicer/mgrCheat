#include "gui.h"
#include <Windows.h>
#include <string>
#include "../cheat/cheat.h"
#include "../injector/injector.hpp"
#include "../IniReader.h"
#include "../KeyBinds.h"
#include "../KeyBind.h"
#include "../kiero.h"
#include "../minhook/include/MinHook.h"

#include "Pl0000.h"
#include "cGameUIManager.h"
#include "GameMenuStatus.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

#pragma warning(disable : 4996)

bool once1 = false;
// Renders gui for cheats
void gui::RenderGUI() noexcept
{
	Pl0000* player = (Pl0000*)g_cGameUIManager.m_pPl;

	if (!once1)
	{
		LoadConfig();
		once1 = true;
	}

	static bool paused = false;

	if (KeyBind::IsKeyPressed(menuKey) && cheat::OnFocus)
		show = !show;

	if (show && g_GameMenuStatus == InGame)
	{
		*(unsigned int*)(shared::base + 0x17EA060) |= 0x1000;
		paused = true;
	}

	if (!show && paused && g_GameMenuStatus == InGame)
	{
		*(unsigned int*)(shared::base + 0x17EA060) &= ~0x1000;
		paused = false;
	}

	if (!show)
		Sleep(20);

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
				ImGui::Checkbox("Infinite Subweapon Ammo & Supplies", &cheat::infiniteSubWeapon);
				ImGui::Checkbox("Height Change (numpad +, -)", &cheat::heightChange);
				ImGui::SliderFloat("Height Change Rate", &cheat::heightRate, 0.0f, 100.0f, "%.3f", 1.0f);
				KeyBind::Hotkey("Visor Hotkey: ", &cheat::temporaryVisorHotkey);
				ImGui::Checkbox("Auto HP Up", &cheat::autoHpUp);
				if (ImGui::InputFloat("Ninja Run Speed Rate", &cheat::ninjaRunSpeedRate))
				{
					if (player)
						player->m_fNinjaRunSpeedRate = cheat::ninjaRunSpeedRate;
				}
				if (ImGui::Button("Toggle ripper mode"))
				{
					if (player)
						if (player->m_nRipperModeEnabled)
							player->DisableRipperMode(false);
						else
							player->EnableRipperMode();
				}
				if (ImGui::Button("Enable ripper mode effect"))
				{
					static bool ripperModeEffectSwitch = false;
					if (player)
					{
						ripperModeEffectSwitch = !ripperModeEffectSwitch;
						if (ripperModeEffectSwitch)
							player->CallEffect(100, &player->field_3470);
						else
							player->field_3470.SetEffectDuration(0.1f, 0.0f);
					}
				}
				ImGui::EndTabItem();		
			}
			if (ImGui::BeginTabItem("Entities"))
			{
				ImGui::Checkbox("Ground Cheat", &cheat::groundCheat);
				if (cheat::groundCheat)
					KeyBind::Hotkey("Hotkey: ", &cheat::groundCheatHotkey);
				ImGui::Checkbox("Time Stop", &cheat::timeStop);
				if (cheat::timeStop)
					KeyBind::Hotkey("Time Stop hotkey: ", &cheat::timeStopHotkey);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Enemies"))
			{
				ImGui::Checkbox("One Hit Kill", &cheat::oneHitKill);
				ImGui::Checkbox("Deal Zero Damage", &cheat::dealZeroDamage);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Battle"))
			{
				ImGui::Checkbox("No Damage Status for battle", &cheat::noDamageStat);
				ImGui::Checkbox("Stealth Cheat", &cheat::stealth);
				ImGui::Checkbox("Infinite battle/VR timer", &cheat::infTimer);
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
				if (ImGui::Button("Reset Config"))
				{
					cheat::Reset();
					Reset();

					cheat::SaveConfig();
					SaveConfig();
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

// Currently saves only menu hotkey
void gui::SaveConfig() noexcept
{
	CIniReader iniReader("CheatMenu.ini");

	iniReader.WriteInteger("Menu", "OpenMenuHotkey", menuKey);
}

// Loads only hotkey variable
void gui::LoadConfig() noexcept
{
	CIniReader iniReader("CheatMenu.ini");

	menuKey = iniReader.ReadInteger("Menu", "OpenMenuHotkey", 45);
}

// Resets gui variables
void gui::Reset() noexcept
{
	width = 900.0f;
	height = 600.0f;
	menuKey = 45;
}