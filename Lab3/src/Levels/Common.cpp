#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Utils\OpenSaveFile.h"	// opensavefile dialog
#include "Common.h"

namespace cs200Common {
	bool ShowMenu = true;
	std::map<int, std::string> RegisteredStates;
	std::map<int, std::string> RoundingMethods;
	std::map<int, std::string> DrawLineMethods;
	std::map<int, std::string> DrawCircleMethods;
	std::map<int, std::string> DrawEllipseMethods;
}using namespace cs200Common;

// forward declar
void SaveFBBinary();
void SaveFBPNG();
void LoadFBBinary();

void RegisterGameState(const char* stateName, int stateID)
{
	RegisteredStates[stateID] = stateName;
}
bool ShowFrameworkMenu(MenuFn demoMenu)
{
	bool isGuiActive = false;
	// lazy init 
	if (DrawLineMethods.empty()) {
		DrawLineMethods[Rasterizer::eDL_NAIVE] = "Explicit Line Equation";
		DrawLineMethods[Rasterizer::eDL_BRESENHAM] = "Bresenham ";
		DrawLineMethods[Rasterizer::eDL_DDA] = "DDA";

		RoundingMethods[Rasterizer::eRM_TRUNCATE] = "Truncate";
		RoundingMethods[Rasterizer::eRM_TRUNCATE_SHIFT] = "Truncate Shift";

		DrawCircleMethods[Rasterizer::eDC_ALGEBRAIC] = "Algebraic";
		DrawCircleMethods[Rasterizer::eDC_PARAMETRIC] = "Parametric";
		DrawCircleMethods[Rasterizer::eDC_PARAMETRIC_INCREMENTAL] = "Parametric Incremental";
		DrawCircleMethods[Rasterizer::eDC_MIDPOINT] = "Midpoint";

		DrawEllipseMethods[Rasterizer::eDE_ALGEBRAIC] = "Algebraic";
		DrawEllipseMethods[Rasterizer::eDE_PARAMETRIC] = "Parametric";
		DrawEllipseMethods[Rasterizer::eDE_PARAMETRIC_INCREMENTAL] = "Parametric Incremental";
		DrawEllipseMethods[Rasterizer::eDE_MIDPOINT] = "Midpoint";

	}

	// show File options 
	if(ShowMenu && ImGui::BeginMainMenuBar()) 
	{
		if (ImGui::BeginMenu("File")) 
		{
			if (ImGui::MenuItem("Save FB Binary", "F3"))
				SaveFBBinary();
			if (ImGui::MenuItem("Save FB PNG", "F2"))
				SaveFBPNG();
			if (ImGui::MenuItem("Load FB Binary", "F3"))
				LoadFBBinary();
			if (ImGui::MenuItem("Hide Menu", "F8"))
				ShowMenu = !ShowMenu;
			ImGui::EndMenu();
		}

		// show configuration
		if (ImGui::BeginMenu("Config"))
		{
			// rounding method. 
			if (ImGui::BeginMenu("Rounding Method")) {

				for (auto& rm : RoundingMethods) {

					// determine if this method is the current one
					bool isCurrent = Rasterizer::GetRoundMethod() == rm.first;

					// change color if it's the selected game state
					ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Text];
					ImVec4 prevColor = color;
					if (isCurrent) {
						color = ImVec4(1, 0, 0, 1);
						ImGui::GetStyle().Colors[ImGuiCol_Text] = color;
					}

					if (ImGui::MenuItem(rm.second.c_str(), 0, &isCurrent))
						Rasterizer::SetRoundMethod((Rasterizer::ERoundMethod)rm.first);

					// restore previous style text color
					ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
				}
				ImGui::EndMenu();
			}
			// line method. 
			if (ImGui::BeginMenu("Line Method")) {

				for (auto& dlm : DrawLineMethods) {

					// determine if this method is the current one
					bool isCurrent = Rasterizer::GetDrawLineMethod() == dlm.first;

					// change color if it's the selected game state
					ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Text];
					ImVec4 prevColor = color;
					if (isCurrent) {
						color = ImVec4(1, 0, 0, 1);
						ImGui::GetStyle().Colors[ImGuiCol_Text] = color;
					}

					if (ImGui::MenuItem(dlm.second.c_str(), 0, &isCurrent))
						Rasterizer::SetDrawLineMethod((Rasterizer::EDrawLineMethod)dlm.first);

					// restore previous style text color
					ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Circle Method")) {

				for (auto& dcm : DrawCircleMethods) {

					// determine if this method is the current one
					bool isCurrent = Rasterizer::GetDrawCircleMethod() == dcm.first;

					// change color if it's the selected game state
					ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Text];
					ImVec4 prevColor = color;
					if (isCurrent) {
						color = ImVec4(1, 0, 0, 1);
						ImGui::GetStyle().Colors[ImGuiCol_Text] = color;
					}

					if (ImGui::MenuItem(dcm.second.c_str(), 0, &isCurrent))
						Rasterizer::SetDrawCircleMethod((Rasterizer::EDrawCircleMethod)dcm.first);

					// restore previous style text color
					ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Ellipse Method")) {

				for (auto& dem : DrawEllipseMethods) {

					// determine if this method is the current one
					bool isCurrent = Rasterizer::GetDrawEllipseMethod() == dem.first;

					// change color if it's the selected game state
					ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Text];
					ImVec4 prevColor = color;
					if (isCurrent) {
						color = ImVec4(1, 0, 0, 1);
						ImGui::GetStyle().Colors[ImGuiCol_Text] = color;
					}

					if (ImGui::MenuItem(dem.second.c_str(), 0, &isCurrent))
						Rasterizer::SetDrawEllipseMethod((Rasterizer::EDrawEllipseMethod)dem.first);

					// restore previous style text color
					ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
				}

				ImGui::EndMenu();
			}
			// resolution. 
			if (ImGui::BeginMenu("Resolution")) {
				static const char* resNames[] = {"640x480", "800x600", "1280x720", "1600x900" };
				static AEVec2 resSizes[] = { {640,480}, {800,600}, {1280,720}, {1600,900} };

				for (u32 i = 0; i < 4; ++i) {
					if (ImGui::MenuItem(resNames[i]))
					{
						u32 w = u32(resSizes[i].x);
						u32 h = u32(resSizes[i].y);
						AEGfxSetWindowsize(w, h);

						Rasterizer::FrameBuffer::Allocate(w, h);
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		// show game states
		if (ImGui::BeginMenu("Game States")) {
			for (auto& gs : RegisteredStates) {
				
				// determine if this game state is the current
				bool isCurrent = gAEGameStateCurr == gs.first;

				// change color if it's the selected game state
				ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Text];
				ImVec4 prevColor = color;
				if (isCurrent) {
					color = ImVec4(1, 0, 0, 1);
					ImGui::GetStyle().Colors[ImGuiCol_Text] = color;
				}

				// show menu item
				if (ImGui::MenuItem(gs.second.c_str(), 0, &isCurrent)) {
					gAEGameStateNext = gs.first;
				}

				// restore previous style text color
				ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
			}
			ImGui::EndMenu();
		}

		// demo options. 
		if (demoMenu && ImGui::BeginMenu("Demo Options")) {
			demoMenu();
			ImGui::EndMenu();
		}
		auto mSize = ImGui::GetWindowHeight();
		AEVec2 mouseVP = gAEMousePosition + AEVec2((f32)gAESysWinWidth / 2.0f, (f32)gAESysWinHeight / 2.0f);
		auto inMenu = mouseVP.y >= (gAESysWinHeight - mSize);
		isGuiActive = ImGui::IsAnyItemActive() || ImGui::IsAnyItemHovered() || inMenu;

		ImGui::EndMainMenuBar();
	}

	return isGuiActive;
}

void SaveFBBinary() {
	OpenSaveFileDlg saveDlg;
	if (saveDlg.Save("Save Frame Buffer to Binary", "*.fb"))
	{
		std::string saveFile;
		if (saveDlg.GetNextFilePath(saveFile))
			Rasterizer::FrameBuffer::SaveToFile(saveFile.c_str());
	}
}
void SaveFBPNG() {
	OpenSaveFileDlg saveDlg;
	if (saveDlg.Save("Save Frame Buffer to Image", "*.png"))
	{
		std::string saveFile;
		if (saveDlg.GetNextFilePath(saveFile))
			Rasterizer::FrameBuffer::SaveToImageFile(saveFile.c_str());
	}
}
void LoadFBBinary()
{
	OpenSaveFileDlg openDlg;
	if (openDlg.Open("Open Frame Buffer File", "*.fb"))
	{
		std::string selectedFile;
		if (openDlg.GetNextFilePath(selectedFile))
			Rasterizer::FrameBuffer::LoadFromFile(selectedFile.c_str());
	}
}
void KeyboardInput()
{
	if (AEInputKeyTriggered('R') && AEInputKeyPressed(AE_KEY_CTRL))
		gAEGameStateNext = AE_GS_RESTART;
	if (AEInputKeyTriggered('Q') && AEInputKeyPressed(AE_KEY_CTRL))
		gAEGameStateNext = AE_GS_QUIT;

	// check for keyboard input
	if (AEInputKeyTriggered(VK_F8))
		ShowMenu = !ShowMenu;

	// Save the data to file
	if (AEInputKeyTriggered(VK_F1))
		SaveFBBinary();

	if (AEInputKeyTriggered(VK_F2))
		SaveFBPNG();

	if (AEInputKeyTriggered(VK_F3))
		LoadFBBinary();
}