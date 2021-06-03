#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "GameStates.h"
#include "Common.h"
#include <imgui/imgui.h>
using namespace Rasterizer;
namespace SimpleCircles {

	void DrawSimpleCircles();
	void DrawFilledCircle();
	void DrawRingCircle();
	void DrawPieCircle();
	void DrawSimpleEllipses();
	void FillEllipse();
	void FillCircleColorLerp();

	const char* DemoStr[] = {
		"Simple Circles",
		"Fill Circle",
		"Ring Circle",
		"Pie Circle",
		"Simple Ellipses",
		"Fill Ellipse",
		"Fill Circle (Color Lerp)"
	};

	void (*DemoFn[])(void) = {
		DrawSimpleCircles,
		DrawFilledCircle,
		DrawRingCircle,
		DrawPieCircle,
		DrawSimpleEllipses,
		FillEllipse,
		FillCircleColorLerp
	};
	u32 demoCount = sizeof(DemoFn) / (sizeof(void*));
	u32 currentDemo = 0;
	void Menu() {
		auto prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		auto red = ImVec4(1, 0, 0, 1);
		auto col = prevColor;
		for (u32 i = 0; i < demoCount; ++i) {

			bool selected = i == currentDemo;
			ImGui::GetStyle().Colors[ImGuiCol_Text] = selected ? red : prevColor;
			if (ImGui::MenuItem(DemoStr[i], 0, &selected)) currentDemo = i;
		}
		// reset imgui color
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
	}
	void Init()
	{
	}
	void Update()
	{
		ShowFrameworkMenu(SimpleCircles::Menu);
	}
	void Render()
	{
		FrameBuffer::Clear(Color().FromU32(AE_COLORS_WHITE));
		DemoFn[currentDemo]();
		FrameBuffer::Present();
	}


	void DrawSimpleCircles() {
		AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;
		DrawCircle(winCenter + AEVec2(-100, 0), 100, Color());
	}
	void DrawFilledCircle() {
		AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;
		FillCircle(winCenter + AEVec2(100, 0), 100, Color().FromU32(AE_COLORS_BLUSH));
	}
	void DrawRingCircle() {
		static f32 outerRad = 100, innerRad = 50;
		AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;
		FillRing(winCenter, outerRad, innerRad, Color().FromU32(AE_COLORS_ALMOND));

		ImGui::Begin("Ring Options");
		ImGui::DragFloat("inner radius", &innerRad, 1.0f, 0.000001f, 5000.f);
		ImGui::DragFloat("outer radius", &outerRad, 1.0f, 0.000001f, 5000.f);
		ImGui::End();
	}
	void DrawPieCircle() {
		static f32 minA = 0.0f, maxA = TWO_PI;
		AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;

		ImGui::Begin("Pie Options");
		minA = AERadToDeg(minA);
		ImGui::DragFloat("min angle", &minA, 0.5f);
		minA = AEDegToRad(minA);

		maxA = AERadToDeg(maxA);
		ImGui::DragFloat("max angle", &maxA, 0.5f);
		maxA = AEDegToRad(maxA);
		ImGui::End();
		FillPie(winCenter, 100, minA, maxA, Color().FromU32(AE_COLORS_ALMOND));
	}
	void DrawSimpleEllipses() {
		AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;
		DrawEllipse(winCenter + AEVec2(-200, 0), 50, 200, Color());
		DrawEllipse(winCenter + AEVec2(100, 100), 200, 50, Color());
		DrawEllipse(winCenter + AEVec2(100, 0), 50, 50, Color());
	}
	void FillEllipse() {
		AEVec2 winCenter = AEVec2((f32)gAESysWinWidth, (f32)gAESysWinHeight) * 0.5f;
		FillEllipse(winCenter + AEVec2(-320, 0), 50, 200, Color().FromU32(AE_COLORS_ALMOND));
		FillEllipse(winCenter + AEVec2(100, -100), 200, 50, Color().FromU32(AE_COLORS_APPLE_GREEN));
	}
	void FillCircleColorLerp() {}
}