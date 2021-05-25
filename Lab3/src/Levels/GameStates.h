#pragma once
enum EGameStatesIDs {
	GS_SIMPLE_LINES,
	GS_SIMPLE_CIRCLES,
	GS_SIMPLE_TRIANGLES,
	GS_FREE_DRAW,
	GS_STRESS_TESTS,
};

namespace SimpleLines {
	void Update();
	void Render();
}
namespace FreeDraw {
	void Init();
	void Update();
	void Render();
}

namespace StressTests {
	void Load();
	void Update();
}
namespace SimpleCircles {
	void Init();
	void Update();
	void Render();
}

namespace SimpleTriangles {
	void Init();
	void Update();
	void Render();
}