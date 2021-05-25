#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "GameStates.h"
#include "Common.h"
using namespace Rasterizer;

// -----------------------------------------------------------------------
// DEMO GLOBALS

namespace SimpleTriangles
{
	u32		gMode = 0;
	u32		gDemo = 0;
	bool	gDebug = true;

	// Renders a simple quad in the middle of the screen
	void RenderDemo1();
	// Renders a simple quad (rotated by 45 def) in the middle of the screen
	void RenderDemo2();
	// Render all possible cases (1 through 6)
	void RenderDemo3();
	// Menu
	void OptionsMenu();



	// -----------------------------------------------------------------------
	// GAMESTATE FUNCTIONS

	void Init()
	{
		gMode = 0;
		gDemo = 0;
		gDebug = true;
	}
	void Update()
	{
		ShowFrameworkMenu(SimpleTriangles::OptionsMenu);
		KeyboardInput();


		if (AEInputKeyTriggered('M'))
		{
			gMode = gMode == 1 ? 0 : 1;
			switch (gMode)
			{
			case 0: printf("TRIANGLE FILL METHOD: NAIVE\n"); break;
			case 1: printf("TRIANGLE FILL METHOD: TOP LEFT\n"); break;
			}
		}
		if (AEInputKeyTriggered('N'))
		{
			gDemo = gDemo == 2 ? 0 : gDemo + 1;
			printf("LEVEL:CURRENT SCENE: %i\n", gDemo);
		}

		if (AEInputKeyTriggered('D'))
			gDebug = !gDebug;
	}
	void Render()
	{
		FrameBuffer::Clear(Color().FromU32(AE_COLORS_WHITE));

		switch (gDemo)
		{
		case 0:
			RenderDemo1();
			break;
		case 1:
			RenderDemo2();
			break;
		case 2:
			RenderDemo3();
			break;
		}

		FrameBuffer::Present();
	}

	// -----------------------------------------------------------------------
	// DEMO FUNCTIONS

		// Renders a simple quad in the middle of the screen
	void RenderDemo1()
	{
		AEVec2 p0(300, 400);
		AEVec2 p1(300, 200);
		AEVec2 p2(500, 200);
		AEVec2 p3(500, 400);

		// Draw Outline
		if (gDebug)
		{
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p1.x, p1.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p1.x, p1.y), AEVec2(p2.x, p2.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p2.x, p2.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
		}
		// use naive
		if (gMode == 0)
		{
			Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleNaive(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
		// use topleft
		else
		{
			Rasterizer::FillTriangleTopLeft(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleTopLeft(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
	}
	// Renders a simple quad (rotated by 45 def) in the middle of the screen
	void RenderDemo2()
	{
		AEVec2 p0(-100, 100);
		AEVec2 p1(-100, -100);
		AEVec2 p2(100, -100);
		AEVec2 p3(100, 100);

		AEMtx33 rot;
		AEMtx33RotDeg(&rot, 45.0f);

		// apply rotation to all points
		AEMtx33MultVec(&p0, &rot, &p0);
		AEMtx33MultVec(&p1, &rot, &p1);
		AEMtx33MultVec(&p2, &rot, &p2);
		AEMtx33MultVec(&p3, &rot, &p3);

		// translate to middle
		p0 += AEVec2(400, 300);
		p1 += AEVec2(400, 300);
		p2 += AEVec2(400, 300);
		p3 += AEVec2(400, 300);

		// Draw Outline
		if (gDebug)
		{
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p1.x, p1.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p1.x, p1.y), AEVec2(p2.x, p2.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p2.x, p2.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
			Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p3.x, p3.y), Rasterizer::Color());
		}
		// use naive
		if (gMode == 0)
		{
			Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleNaive(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
		// use topleft
		else
		{
			Rasterizer::FillTriangleTopLeft(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			Rasterizer::FillTriangleTopLeft(p0, p2, p3, Rasterizer::Color().FromU32(0xFF0000FF));
		}
	}
	// Render all possible cases (1 through 6)
	void RenderDemo3()
	{
		AEVec2 top(75.0f, 500.0f);
		AEVec2 midLeft(0.0f, 425.0f);
		AEVec2 midRight(150.0f, 425.0f);
		AEVec2 bot(75.0f, 350.0f);

		const int TOP = 0;
		const int MID_L = 1;
		const int MID_R = 2;
		const int BOT = 3;

		AEVec2 vtx[4] = { top, midLeft, midRight, bot };
		int triangles[6 * 3] =
		{
			BOT, TOP, MID_L,	// CASE 1
			BOT, MID_R, TOP,	// CASE 2
			MID_L, BOT, TOP,	// CASE 4
			MID_R, TOP, BOT,	// CASE 3
			TOP, MID_L, BOT,	// CASE 5
			TOP, BOT, MID_R		// CASE 6
		};

		AEVec2 offset(100, 0);
		for (int i = 0; i < 18; i += 3)
		{
			AEVec2 p0 = vtx[triangles[i]];
			AEVec2 p1 = vtx[triangles[i + 1]];
			AEVec2 p2 = vtx[triangles[i + 2]];

			p0 += offset;
			p1 += offset;
			p2 += offset;

			// Draw Outline
			if (gDebug)
			{
				Rasterizer::DrawLine(AEVec2(p0.x, p0.y), AEVec2(p1.x, p1.y), Rasterizer::Color());
				Rasterizer::DrawLine(AEVec2(p1.x, p1.y), AEVec2(p2.x, p2.y), Rasterizer::Color());
				Rasterizer::DrawLine(AEVec2(p2.x, p2.y), AEVec2(p0.x, p0.y), Rasterizer::Color());
			}
			// use naive
			if (gMode == 0)
			{
				Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			}
			// use topleft
			else
			{
				Rasterizer::FillTriangleTopLeft(p0, p1, p2, Rasterizer::Color().FromU32(0xFFFF0000));
			}

			offset.x += 25.0f;
			if (i % 6) // additional offset when done a pair
				offset.x += 150.0f;
		}
	}

	void OptionsMenu()
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImVec4 red = ImVec4(1, 0, 0, 1);
		if (ImGui::BeginMenu("Fill Algorithm"))
		{
			bool isCurrent = gMode == 0;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gMode == 0 ? red : prevColor;
			if (ImGui::MenuItem("Naive", "M", &isCurrent)) gMode = 0;

			isCurrent = gMode == 1;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gMode == 1 ? red : prevColor;
			if (ImGui::MenuItem("Top-Left", "M", &isCurrent)) gMode = 1;

			ImGui::EndMenu();
		}
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;

		if (ImGui::BeginMenu("Demo"))
		{
			bool isCurrent = gDemo == 0;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gDemo == 0 ? red : prevColor;
			if (ImGui::MenuItem("Red-Blue Quad", "N", &isCurrent)) gDemo = 0;

			isCurrent = gDemo == 1;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gDemo == 1 ? red : prevColor;
			if (ImGui::MenuItem("Red-Blue Quad (45 deg)", "N", &isCurrent)) gDemo = 1;

			isCurrent = gDemo == 2;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = gDemo == 2 ? red : prevColor;
			if (ImGui::MenuItem("All Cases", "N", &isCurrent)) gDemo = 2;

			ImGui::EndMenu();
		}
		ImGui::GetStyle().Colors[ImGuiCol_Text] =  prevColor;
		// debug
		ImGui::MenuItem("Draw Triangle Outline", "D", &gDebug);

		// reset color
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	}
}