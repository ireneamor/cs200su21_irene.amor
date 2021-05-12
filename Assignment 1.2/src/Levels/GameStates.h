enum EGameStatesIDs {
	GS_SIMPLE_LINES,
	GS_FREE_DRAW,
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