#include <AEEngine.h>
#include <imgui/imgui.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"
namespace PlaneNormalDemo
{
	void Init()
	{
	}
	void Update()
	{
		ShowFrameworkMenu();
	}
	void Render()
	{
		u32 checker[2] = { AE_COLORS_GRAY, AE_COLORS_ANTI_FLASH_WHITE };
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color().FromU32(checker[0]));
		
		// TODO: Insert CODE HERE

		Rasterizer::FrameBuffer::Present();
	}
}