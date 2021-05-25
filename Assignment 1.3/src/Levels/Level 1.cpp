#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Utils\OpenSaveFile.h"	// opensavefile dialog
#include "Level 1.h"
#include <imgui/imgui.h>

using namespace Rasterizer;
void Demo1();
void Demo2();
u32 mode = 0;
void Level_1_Load()
{
}
void Level_1_Init()
{
}
void Level_1_Update()
{
	KeyboardInput();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Demos")) {
			if (ImGui::MenuItem("CS 200")) mode = 0;
			if (ImGui::MenuItem("8 way pie")) mode = 1;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void Level_1_Render()
{
	// Clear the frame buffer.
	FrameBuffer::Clear(Rasterizer::Color().FromU32(0xFFFFFFFF));

	if (mode == 0)Demo1();
	else Demo2();

	// Send content of frame buffer to 
	Rasterizer::FrameBuffer::Present();

	// Debug
	SaveFrameBuffer();
}
void Level_1_Free()
{
}
void Level_1_Unload()
{

}

// ----------------------------------------------------------------------------
// Draw Line Tests
void Demo1()
{
	// draw 'CS 200'

	f32 startX = 250.0f;
	f32 letterWidth = 50.0f;
	f32 smallSpace = 65.0f;
	f32 bigSpace = 100.0f;
	Color lineCol = Color();

	// C
	Rasterizer::DrawHorizontalLine(AEVec2(startX, 400.0f), AEVec2(startX + letterWidth, 400.0f), lineCol);
	Rasterizer::DrawVerticalLine(AEVec2(startX, 400.0f), AEVec2(startX, 325.0f), lineCol);
	Rasterizer::DrawHorizontalLine(AEVec2(startX, 325.0f), AEVec2(startX + letterWidth, 325.0f), lineCol);

	startX += smallSpace;

	// S
	Rasterizer::DrawHorizontalLine(AEVec2(startX, 400.0f), AEVec2(startX + letterWidth, 400.0f), lineCol);
	Rasterizer::DrawVerticalLine(AEVec2(startX, 400.0f), AEVec2(startX, 362.5f), lineCol);
	Rasterizer::DrawHorizontalLine(AEVec2(startX, 362.5f), AEVec2(startX + letterWidth, 362.5f), lineCol);
	Rasterizer::DrawVerticalLine(AEVec2(startX + letterWidth, 362.5f), AEVec2(startX + letterWidth, 325.0f), lineCol);
	Rasterizer::DrawHorizontalLine(AEVec2(startX + letterWidth, 325.0f), AEVec2(startX, 325.0f), lineCol);

	startX += bigSpace;

	// 2
	Rasterizer::DrawHorizontalLine(AEVec2(startX, 400.0f), AEVec2(startX + letterWidth, 400.0f), lineCol);
	Rasterizer::DrawVerticalLine(AEVec2(startX + letterWidth, 400.0f), AEVec2(startX + letterWidth, 362.5f), lineCol);
	Rasterizer::DrawHorizontalLine(AEVec2(startX + letterWidth, 362.5f), AEVec2(startX, 362.5f), lineCol);
	Rasterizer::DrawVerticalLine(AEVec2(startX, 362.5f), AEVec2(startX, 325.0f), lineCol);
	Rasterizer::DrawHorizontalLine(AEVec2(startX, 325.0f), AEVec2(startX + letterWidth, 325.0f), lineCol);

	startX += smallSpace;

	// render 2 zeros
	for (int i = 0; i < 2; ++i, startX += smallSpace)
	{
		// top line
		Rasterizer::DrawHorizontalLine(AEVec2(startX, 400.0f), AEVec2(startX + letterWidth, 400.0f), lineCol);
		// bottom line								 
		Rasterizer::DrawHorizontalLine(AEVec2(startX, 325.0f), AEVec2(startX + letterWidth, 325.0f), lineCol);
		// left line								 
		Rasterizer::DrawVerticalLine(AEVec2(startX, 400.0f), AEVec2(startX, 325.0f), lineCol);
		// right line
		Rasterizer::DrawVerticalLine(AEVec2(startX + letterWidth, 400.0f), AEVec2(startX + letterWidth, 325.0f), lineCol);

	}

	// test rendering diagonal lines 
	startX = 200.0f;
	for (int i = 0; i < 3; ++i, startX += bigSpace + smallSpace) {
		Rasterizer::DrawRect(AEVec2(startX + 50.0f, 250), AEVec2(100, 100), Rasterizer::Color().FromU32(0xFFFF0000));
		Rasterizer::DrawDiagonalLine(AEVec2(startX, 200), AEVec2(startX + 100, 300), lineCol);
		Rasterizer::DrawDiagonalLine(AEVec2(startX, 300), AEVec2(startX + 100, 200), lineCol);
	}

}
void Demo2()
{
	// draw a 8-way pie
	AEVec2 center = { 400, 300 };
	AEVec2 left = { 200, 300 };
	AEVec2 right = { 600, 300 };
	AEVec2 top = { 400, 500 };
	AEVec2 bottom = {400, 100};
	AEVec2 top_right = center + AEVec2(cos(AEDegToRad(45.0f)), sin(AEDegToRad(45.0f))) * 200.0f;
	AEVec2 top_left = center + AEVec2(cos(AEDegToRad(135.0f)), sin(AEDegToRad(135.0f))) * 200.0f;
	AEVec2 bot_right = center + AEVec2(cos(AEDegToRad(-45.0f)), sin(AEDegToRad(-45.0f))) * 200.0f;
	AEVec2 bot_left = center + AEVec2(cos(AEDegToRad(-135.0f)), sin(AEDegToRad(-135.0f))) * 200.0f;

	Rasterizer::DrawRect(center, { 20,20 }, Rasterizer::Color().FromU32(AE_COLORS_BRONZE));
	Rasterizer::DrawHorizontalLine(center, right, Rasterizer::Color());
	Rasterizer::DrawHorizontalLine(center, left, Rasterizer::Color());
	Rasterizer::DrawVerticalLine(center, top, Rasterizer::Color());
	Rasterizer::DrawVerticalLine(center, bottom, Rasterizer::Color());

	Rasterizer::DrawDiagonalLine(center, top_right, Rasterizer::Color());
	Rasterizer::DrawDiagonalLine(center, top_left, Rasterizer::Color());
	Rasterizer::DrawDiagonalLine(center, bot_right, Rasterizer::Color());
	Rasterizer::DrawDiagonalLine(center, bot_left, Rasterizer::Color());
}

// ----------------------------------------------------------------------------
// COMMON

void SaveFrameBuffer()
{
	// Save the data to file
	if (AEInputKeyTriggered(VK_F1))
	{
		OpenSaveFileDlg saveDlg;
		if (saveDlg.Save("Save Frame Buffer to Binary", "*.fb"))
		{
			std::string saveFile;
			if (saveDlg.GetNextFilePath(saveFile))
				Rasterizer::FrameBuffer::SaveToFile(saveFile.c_str());
		}
	}

	if (AEInputKeyTriggered(VK_F2))
	{
		OpenSaveFileDlg saveDlg;
		if (saveDlg.Save("Save Frame Buffer to Image", "*.png"))
		{
			std::string saveFile;
			if (saveDlg.GetNextFilePath(saveFile))
				Rasterizer::FrameBuffer::SaveToImageFile(saveFile.c_str());
		}
	}
	if (AEInputKeyTriggered(VK_F3))
	{
		OpenSaveFileDlg openDlg;
		if (openDlg.Open("Open Frame Buffer File", "*.fb"))
		{
			std::string selectedFile;
			if (openDlg.GetNextFilePath(selectedFile))
				Rasterizer::FrameBuffer::LoadFromFile(selectedFile.c_str());
		}
	}
}
void KeyboardInput()
{
	if (AEInputKeyTriggered('R'))
		gAEGameStateNext = AE_GS_RESTART;
	if (AEInputKeyTriggered('Q') && AEInputKeyPressed(AE_KEY_CTRL))
		gAEGameStateNext = AE_GS_QUIT;
}