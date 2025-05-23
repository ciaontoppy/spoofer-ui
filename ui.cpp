#include "main.h"
#include "fonts.h"
#include "!items.h"
#include "snowflakes.hpp"
#include "serials.h"
#include "keyauth_credentials.h"
#include "imspinner.h"

std::vector<Snowflake::Snowflake> snow;
char license[40] = "";

int subtab1 = 0;
int subtab2 = 10;

bool spoof;
bool done;

void check_serials();
void save_key(std::string license);
BOOL exec(WCHAR command[]);

const std::string _mobo = GetMoboSerialNumber();
const std::string _bios = GetBiosSerialNumber();
const std::string _uuid = GetSystemUUID();
const std::string _mac = GetMacAddress();

inline void check_key(char buf[], size_t buf_size);

void ui::Before_Loop()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo, sizeof(logo), nullptr, nullptr, &images::logo, 0);
	D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, disc, sizeof(disc), nullptr, nullptr, &images::disc, 0);
	D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_p, sizeof(circle_p), nullptr, nullptr, &images::circle, 0);

	fonts::inter = io.Fonts->AddFontFromMemoryTTF(inter, sizeof(inter), 20);
	fonts::inter2 = io.Fonts->AddFontFromMemoryTTF(inter_2, sizeof(inter_2), 45);
	fonts::inter3 = io.Fonts->AddFontFromMemoryTTF(inter, sizeof(inter), 15);
	fonts::icon = io.Fonts->AddFontFromMemoryTTF(icon, sizeof(icon), 20);
	fonts::t = io.Fonts->AddFontFromMemoryTTF(icont, sizeof(icont), 39);

	Snowflake::CreateSnowFlakes(snow, 40, 3.f/*minimum size*/, 20.f/*maximum size*/, 0/*imgui window x position*/, 0/*imgui window y position*/, window::size.x, window::size.y, Snowflake::vec3(0.08, .50f)/*gravity*/, ImGui::GetColorU32(colors::particles)/*color*/);
	custom::Blur(g_hwnd);
	custom::style();
	check_key(license, IM_ARRAYSIZE(license));
}

static ImVec4 b_serial_col = colors::red;
static ImVec4 c_serial_col = colors::red;
static ImVec4 m_serial_col = colors::red;
static ImVec4 u_serial_col = colors::red;

void ui::Render()
{
	ImGui::PushFont(fonts::inter2);

	custom::begin("JINX", window::size);
	{
		auto* window = ImGui::GetCurrentWindow();
		auto& style = ImGui::GetStyle();
		auto& io = ImGui::GetIO();

		ImGui::PushFont(fonts::inter);
		style.Alpha = alpha;
		static float timer = io.DeltaTime * 3;

		if (custom::icon_button("c", { window->Size.x - 50, 31 }, true)) exit(0);


		if (tab == 0)
		{
			alpha_anim();
			custom::discord_button(L"https://discord.com/invite/CsuXTegewj", { 35, 415 });

			items::input("LICENSE KEY", { (window->Size.x - 300) / 2, 190 }, license, IM_ARRAYSIZE(license), 0);
			items::checkbox("Remember me", { (window->Size.x - 300) / 2, 240 }, &remember, false);

			if (items::button("Login", { (window->Size.x - 300) / 2, 270 }, { 300, 37 })) { 

				KeyAuthApp.license(license);

				if (KeyAuthApp.response.success)
				{
					if (remember) save_key(license);
					alpha = 0;
					tab = 5;
				}
			}
		}

		if (tab == 5)
		{
			timer += 0.1; if (timer < 3) return;

			alpha_anim();
			ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - 50.f * 2.0f) * 0.5f, 180.0f }); 
			ImGui::ImageRotation(images::circle, { 90, 90 }, ImVec2(1, 1), ImVec2(0, 0), ImGui::GetColorU32(colors::main_color), 0.1f);

			if (timer > 100) {

				alpha2 = 0;
				a3 = 0;
				alpha = 0;
				tab = 1;
			}
		}

		if (tab > 0 && tab < 5)
		{

			alpha2 = std::clamp(alpha2 + (2 * ImGui::GetIO().DeltaTime * 1.5f), 0.0f, 1.f);
			a3 = std::clamp(a3 + (2 * ImGui::GetIO().DeltaTime * 1.5f), 0.0f, 1.f);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, a3);
			window->DrawList->AddLine(window->Pos + ImVec2(0, 80), window->Pos + ImVec2(window->Size.x, 80), ImGui::GetColorU32(colors::main_color), 2);
			window->DrawList->AddLine(window->Pos + ImVec2(100, 80), window->Pos + ImVec2(100, window->Size.y - 10), ImGui::GetColorU32(colors::main_color), 2);
			window->DrawList->AddLine(window->Pos + ImVec2(0, window->Size.y - 100), window->Pos + ImVec2(100, window->Size.y - 100), ImGui::GetColorU32(colors::main_color), 2.f);
			window->DrawList->AddImage(images::logo, window->Pos + ImVec2(0, 0), window->Pos + ImVec2(100, 90), {0,0}, {1,1}, ImGui::GetColorU32(ImVec4(1,1,1,1)));
			custom::discord_button(L"https://discord.com/invite/CsuXTegewj", { 35, 415 });

			ImGui::PushFont(fonts::t);
			if (items::tab("G##1", ImVec2((100 - ImGui::CalcTextSize("G").x) / 2, 180), tab == 1)) { alpha = 0; tab = 1; subtab1 = 0; subtab2 = 10; }
			if (items::tab("I##1", ImVec2((100 - ImGui::CalcTextSize("I").x) / 2, 240), tab == 2)) { alpha = 0; tab = 2; subtab2 = 0; subtab1 = 10; }
			ImGui::PopFont();
			ImGui::PopStyleVar();
		}

		if (tab == 1)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha2);
			if (items::subtab("Announcements", { 140, 38 }, subtab1 == 0)) subtab1 = 0;
			if (items::subtab("Instructions", { 290, 38 }, subtab1 == 1)) { 

				WCHAR link[] = L"cmd /c start https://jinx-solutions.gitbook.io/guide";
				exec(link);
				subtab1 = 0; 
			}
			ImGui::PopStyleVar();
		}

		if (tab == 2)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha2);
			if (items::subtab("Device", { 140, 38 }, subtab2 == 0)) subtab2 = 0;
			if (items::subtab("Spoofer", { 230, 38 }, subtab2 == 1)) subtab2 = 1;
			if (items::subtab("Miscellaneous", { 320, 38 }, subtab2 == 2)) subtab2 = 2;
			ImGui::PopStyleVar();
		}

		if (subtab1 == 0 && tab > 0 && tab < 5)
		{
			alpha_anim();

			custom::announcement("You are on the latest Verse V4.45 | 06/02/24", "Added option to unspoof all serials, & overall stability.", { 120, 45 });
			custom::announcement("Verse V4.41 | 05/02/24", "User-interface improvements, bug fixes & overall stability.", { 120, 45 });
			custom::announcement("Verse V4.40 | 04/03/24", "Asus improved, UI Changes, Insyde support, Bug  fixes.", { 120, 45 });
			custom::announcement("Verse V4.35 | 03/22/24", "Revamped LEGIT Serials, Optimizations & Bug fixes.", { 120, 45 });
		}

		if (subtab2 == 0 && tab > 0)
		{
			alpha_anim();

			custom::model("Motherboard Manufacturer: ", mobo_manufacturer, { 120, 40 });
			custom::model("Motherboard Model: ", mobo_model, { 120, 45 });
			custom::model("Ethernet Adapter: ", ethernet_adapter, { 120, 50 });
			custom::model("Disk Drives: ", disks, { 120, 55 });
			custom::model("CPU: ", _cpu, { 120, 60 });
			
			window->DrawList->AddLine(window->DC.CursorPos + ImVec2(120, 65), window->DC.CursorPos + ImVec2(window->Size.x, 65), ImGui::GetColorU32(colors::dark_gray), 2.f);

			custom::status_check("WIFI:", { 120, 300 }, &wifi, true);
			custom::status_check("Bluetooth:", { 275, 300 }, &bt, false);
			custom::serial("Baseboard:", mobo, { 120, 333 }, b_serial_col, true);
			custom::serial("BIOS:", bios, { 150 + ImGui::CalcTextSize("Baseboard").x + ImGui::CalcTextSize(mobo.c_str()).x, 333 }, c_serial_col);
			custom::serial("MAC Address:", mac, { 120, 366 }, m_serial_col, true);
			custom::serial("File Traces:", "Not found", { 145 + ImGui::CalcTextSize("MAC Address:").x + ImGui::CalcTextSize(mac.c_str()).x, 366}, colors::green);
			custom::serial("UUID:", uuid, { 120, 399 }, u_serial_col);
			custom::status_check("TPM [Trusted Computing] Status:", { 120, 432 }, &tpm); //This will only say its real status if you run the program as admin
		}

		if (subtab2 == 1)
		{
			alpha_anim();

			custom::begin_child("LOGS", { 130, 110 }, { window->Size.x - 130 - 30, 200 });
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
				items::render_logs();
				ImGui::Spacing();
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();

			if (items::button("PERMANENT SPOOF", { 130, 325 }, { window->Size.x - 130 - 30, 48 })) {

				if (!done)
				{
					items::add_log("Launched Permanent Spoofing!", none, { 0, 35 });
					items::add_log("Please be patient for 2 minutes.", none, { 0, 40 }); // Using some spacing at the end, position will be relative, test it by yourself

					spoof = true;
				}
			}

			if (items::button("ASUS SPOOF", { 130, 385 }, { (window->Size.x - 160) / 2 - 7, 51 }));
			if (items::button("MAC SPOOF", { 391, 385 }, { (window->Size.x - 160) / 2 - 7, 51 }));
		}

		if (subtab2 == 2)
		{
			alpha_anim();

			items::button_hint("DISK SPOOF", { 130, 110}, "");

			items::button_hint("DISABLE TPM", { 130, 180 }, 
				"This will disable the trusted computing for your computer", 
				"Warning: This could damage your motherboard");

			items::button_hint("HVCI BYPASS", { 130, 250 }, "");
		}

		if (spoof)
		{
			auto& io = ImGui::GetIO();
			static float timer = 0.f;
			timer += 0.1;

			if (timer > 10) {

				items::add_log("Succesfully Spoofed, restart pc.", success, { 0, 50 });

				check_serials();

				done = true;
				spoof = false;
			}
		}
	}
	ImGui::End();
}

void check_serials()
{
	mobo = "test wwwww";
	bios = "womp womp";
	uuid = GetSystemUUID();
	mac = GetMacAddress();

	if (mobo == _mobo) b_serial_col = colors::red; else b_serial_col = colors::green;
	if (bios == _bios) c_serial_col = colors::red; else c_serial_col = colors::green;
	if (mac == _mac) m_serial_col = colors::red; else m_serial_col = colors::green;
	if (uuid == _uuid) u_serial_col = colors::red; else u_serial_col = colors::green;
}

void save_key(std::string license)
{
	std::ofstream key("key.txt");

	if (!key) std::cerr << "Error creating the key file";

	key << license;
	key.close();
}

inline void check_key(char buf[], size_t buf_size)
{
	std::ifstream key("key.txt");

	std::string license;
	if (key) std::getline(key, license);
	key.close();

	strncpy_s(buf, buf_size, license.c_str(), buf_size - 1);
	buf[buf_size - 1] = '\0';
}

BOOL exec(WCHAR command[])
{
	STARTUPINFO i;
	PROCESS_INFORMATION p;

	ZeroMemory(&i, sizeof(i));
	ZeroMemory(&p, sizeof(p));

	i.wShowWindow = SW_HIDE;
	i.dwFlags = STARTF_USESHOWWINDOW;

	if (!CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &i, &p))
	{
		std::cerr << "Error executing the command";
		return FALSE;
	}

	WaitForSingleObject(p.hProcess, INFINITE);

	CloseHandle(p.hProcess);
	CloseHandle(p.hThread);
}