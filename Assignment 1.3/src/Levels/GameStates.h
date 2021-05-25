enum EGameStatesIDs {
	GS_SIMPLE_LINES,
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
