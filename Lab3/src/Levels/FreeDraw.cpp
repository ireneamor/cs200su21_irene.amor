// ----------------------------------------------------------------------------
// File: FreeDraw.cpp
// Project: CS200 Assignment 1
// Purpose: 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// INCLUDES
#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
#include "GameStates.h"
#include <imgui/imgui.h>

namespace FreeDraw {
// ----------------------------------------------------------------------------
// GLOBALS AND DEFINITIONS
std::vector<std::pair<AEVec2, f32>> gCircleArray;
std::vector<std::pair<AEVec2, AEVec2>> gEllipseArray;
std::vector<std::pair<AEVec2, AEVec2>> gLineArray;
u32	gCurrentLinePoint = 0;
enum EFreeDrawPrimitive { eFD_LINES, eFD_CIRCLES, eFD_ELLIPSES};
EFreeDrawPrimitive gCurrentPrimitive = eFD_LINES;

// ----------------------------------------------------------------------------
// FORWARD DECLARATIONS
void OptionsMenu()
{
	// determine if this method is the current one
	bool isCurrent[] = {
	gCurrentPrimitive == eFD_LINES, 
	gCurrentPrimitive == eFD_CIRCLES,
	gCurrentPrimitive == eFD_ELLIPSES
	};
	// change color if it's the selected game state
	ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
	ImVec4 red = ImVec4(1, 0, 0, 1);

	ImVec4 colors[] = {
		isCurrent[0] ? red : prevColor,
		isCurrent[1] ? red : prevColor,
		isCurrent[2] ? red : prevColor
	};

	ImGui::GetStyle().Colors[ImGuiCol_Text] = colors[0];
	if (ImGui::MenuItem("Lines", 0, &isCurrent[0])) gCurrentPrimitive = eFD_LINES;

	ImGui::GetStyle().Colors[ImGuiCol_Text] = colors[1];
	if (ImGui::MenuItem("Circles", 0, &isCurrent[1])) gCurrentPrimitive = eFD_CIRCLES;

	ImGui::GetStyle().Colors[ImGuiCol_Text] = colors[2];
	if (ImGui::MenuItem("Ellipses", 0, &isCurrent[2])) gCurrentPrimitive = eFD_ELLIPSES;

	ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;

	int step = Rasterizer::GetCircleParametricPrecision();
	ImGui::SetNextItemWidth(50);
	if (ImGui::DragInt("Parametric Precision", &step, 1.0f, 1, 250))
		Rasterizer::SetCircleParametricPrecision(step);

}

// ----------------------------------------------------------------------------
// GAMESTATEFUNCTIONS

void Init()
{
	gLineArray.clear();
	gCircleArray.clear();
	gEllipseArray.clear();
	gCurrentLinePoint = 0;
}
void Update()
{
	KeyboardInput();
	bool isGuiActive = ShowFrameworkMenu(OptionsMenu);

	// clear all primitives
	if (AEInputKeyTriggered('C'))
	{
		gLineArray.clear();
		gCircleArray.clear();
		gEllipseArray.clear();
	}

	// current the mouse position to frame buffer space (origin at botleft)
	AEVec2 mouseVP = gAEMousePosition + AEVec2((f32)gAESysWinWidth / 2.0f, (f32)gAESysWinHeight / 2.0f);
 
	// create primitives with the mouse, unless the gui is active.
	if (!isGuiActive) {

		// new line
		if (AEInputMouseTriggered(AE_MOUSE_LEFT))
		{
			if (gCurrentLinePoint == 0 || AEInputKeyPressed(AE_KEY_CTRL)) {
				switch (gCurrentPrimitive) {
				case eFD_LINES: gLineArray.push_back({ mouseVP, mouseVP }); break;
				case eFD_CIRCLES: gCircleArray.push_back({ mouseVP, 0.0f }); break;
				case eFD_ELLIPSES: gEllipseArray.push_back({ mouseVP, mouseVP }); break;
				}

				gCurrentLinePoint++;
			}
			else gCurrentLinePoint = 0;
		}

		// follow mouse
		if (gCurrentLinePoint)
		{
			switch (gCurrentPrimitive) {
			case eFD_LINES:			gLineArray.back().second = mouseVP; break;
			case eFD_CIRCLES:		gCircleArray.back().second = mouseVP.Distance(gCircleArray.back().first); break;
			case eFD_ELLIPSES: {
				auto d = gEllipseArray.back().first - mouseVP;
				gEllipseArray.back().second = { fabsf(d.x), fabs(d.y) }; break;
			}
			}
		}
	}
}
void Render()
{
	Rasterizer::FrameBuffer::Clear(Rasterizer::Color(1.0f, 1.0f, 1.0f, 1.0f));

	// render all the lines
	for (auto& line : gLineArray) 
		Rasterizer::DrawLine(line.first, line.second, Rasterizer::Color());

	// render all the circles
	for(auto& circle: gCircleArray)
		Rasterizer::DrawCircle(circle.first, circle.second, Rasterizer::Color());

	// render all the ellipses
	for (auto& ellipse : gEllipseArray)
		Rasterizer::DrawEllipse(ellipse.first, ellipse.second.x, ellipse.second.y, Rasterizer::Color());

	Rasterizer::FrameBuffer::Present();
}


}
// ----------------------------------------------------------------------------
