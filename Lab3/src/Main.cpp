#include <AEEngine.h>
#include "Engine\Rasterizer\Rasterizer.h"
#include "Levels\GameStates.h"
#include "Levels\Common.h"

int main()
{
	// Initialize the alpha engine
	if(AESysInit("Lab 2 - Draw Circles") == false)
	{
		return 0;
	}

	// initialize graphics system
	if (!Rasterizer::FrameBuffer::Allocate(gAESysWinWidth, gAESysWinHeight))
		return 0;

	// Add a game state to the game state manager
	AEGameStateMgrAdd(
		GS_SIMPLE_LINES,
		0,
		0,
		SimpleLines::Update,
		SimpleLines::Render,
		0,
		0);
	AEGameStateMgrAdd(
		GS_FREE_DRAW,
		0,
		FreeDraw::Init,
		FreeDraw::Update,
		FreeDraw::Render,
		0,
		0);
	AEGameStateMgrAdd(
		GS_STRESS_TESTS,
		StressTests::Load,
		0,
		StressTests::Update,
		0,
		0,
		0);
	AEGameStateMgrAdd(
		GS_SIMPLE_CIRCLES,
		0,
		SimpleCircles::Init,
		SimpleCircles::Update,
		SimpleCircles::Render,
		0,
		0);
	AEGameStateMgrAdd(
		GS_SIMPLE_TRIANGLES,
		0,
		SimpleTriangles::Init,
		SimpleTriangles::Update,
		SimpleTriangles::Render,
		0,
		0);

	// Initialize the game state manager
	AEGameStateMgrInit(GS_SIMPLE_TRIANGLES);

	// Add the game states to the framework menu
	RegisterGameState("Simple Line Tests", GS_SIMPLE_LINES);
	RegisterGameState("Simple Circle Tests", GS_SIMPLE_CIRCLES);
	RegisterGameState("Simple Triangle Tests", GS_SIMPLE_TRIANGLES);
	RegisterGameState("Free Draw", GS_FREE_DRAW);
	RegisterGameState("Stress Tests", GS_STRESS_TESTS);

	// launch engine
	AESysGameLoop();

	// Terminate Graphics System
	Rasterizer::FrameBuffer::Delete();

	// Terminate AECore
	AESysExit();

	return 0;
}