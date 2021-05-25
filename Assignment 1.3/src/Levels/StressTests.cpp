#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "GameStates.h"
#include "Common.h"
using namespace Rasterizer;
#include <iostream> // cout

namespace StressTests {
	void StressTestLines()
	{
		AESysShowConsole();
		int lineCount = 1000000;
		std::vector<AEVec2> points(lineCount * 2);
		// draw a random combination of lines contained within the viewport
		for (int i = 0; i < lineCount * 2; i += 2)
		{
			// generate the two random points
			AEVec2 p0(AERandFloat(0, (f32)gAESysWinWidth), AERandFloat(0, (f32)gAESysWinHeight));
			AEVec2 p1(AERandFloat(0, (f32)gAESysWinWidth), AERandFloat(0, (f32)gAESysWinHeight));

			points[i] = p0;
			points[i + 1] = p1;
		}

		f64 timeDDA, timeNaive, timeBresenham;

		Rasterizer::Color c;
		// do stress test comparison
		auto s = AEGetTime();
		for (u32 i = 0; i < points.size(); i += 2)
			Rasterizer::DrawLineDDA(points[i], points[i + 1], c);
		timeDDA = AEGetTime() - s;

		s = AEGetTime();
		for (u32 i = 0; i < points.size(); i += 2)
			Rasterizer::DrawLineNaive(points[i], points[i + 1], c);
		timeNaive = AEGetTime() - s;

		s = AEGetTime();
		for (u32 i = 0; i < points.size(); i += 2)
			Rasterizer::DrawLineBresenham(points[i], points[i + 1], c);
		timeBresenham = AEGetTime() - s;

		std::cout << "Naive Time: " << timeNaive << "\n";
		std::cout << "DDA Time: " << timeDDA << "\n";
		std::cout << "Bresenham Time: " << timeBresenham << "\n";
	}
	void Load()
	{
		StressTestLines();
	}
	void Update()
	{
		ShowFrameworkMenu();

		FrameBuffer::Present();
	}
}