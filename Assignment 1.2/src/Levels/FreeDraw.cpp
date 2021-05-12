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

namespace FreeDraw {
	// ----------------------------------------------------------------------------
	// GLOBALS AND DEFINITIONS
	std::vector<std::pair<AEVec2, AEVec2>> gLineArray;
	u32	gCurrentLinePoint = 0;
	// ----------------------------------------------------------------------------
	// GAMESTATEFUNCTIONS
	void Init()
	{
		gLineArray.clear();
		gCurrentLinePoint = 0;
	}
	void Update()
	{
		KeyboardInput();
		bool isGuiActive = ShowFrameworkMenu();

		// current the mouse position to frame buffer space (origin at topleft)
		AEVec2 mouseVP = gAEMousePosition + AEVec2((f32)gAESysWinWidth / 2.0f, (f32)gAESysWinHeight / 2.0f);

		// clear all primitives
		if (AEInputKeyTriggered('C'))
		{
			gLineArray.clear();
			gCurrentLinePoint = 0;
		}
		if (!isGuiActive) {
			// new line
			if (AEInputMouseTriggered(AE_MOUSE_LEFT))
			{
				if (gCurrentLinePoint == 0 || AEInputKeyPressed(AE_KEY_CTRL)) {
					gLineArray.push_back(std::pair<AEVec2, AEVec2>(mouseVP, mouseVP));
					gCurrentLinePoint++;
				}
				else gCurrentLinePoint = 0;
			}

			// follow mouse
			if (gCurrentLinePoint)
			{
				gLineArray.back().second = mouseVP;
			}
		}
	}
	void Render()
	{
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color(1.0f, 1.0f, 1.0f, 1.0f));

		// render all the lines
		for (u32 i = 0; i < gLineArray.size(); ++i)
		{
			// get line endpoints
			AEVec2& p0 = gLineArray[i].first;
			AEVec2& p1 = gLineArray[i].second;

			// Draw the line
			Rasterizer::DrawLine(p0, p1, Rasterizer::Color());
		}
		Rasterizer::FrameBuffer::Present();
	}
}