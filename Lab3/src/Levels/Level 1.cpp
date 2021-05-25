#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "..\Engine\Utils\OpenSaveFile.h"	// opensavefile dialog
#include "Level 1.h"

using namespace Rasterizer;

void Level_1_Load()
{
}
void Level_1_Init()
{
	FrameBuffer::Clear(Rasterizer::Color().FromU32(0xFFF23454));
	FrameBuffer::SaveToImageFile("test.png");
}
void Level_1_Update()
{
	KeyboardInput();
}
void Level_1_Render()
{
	// Clear the frame buffer.
	FrameBuffer::Clear(Rasterizer::Color().FromU32(0xFFF23454));
	
	// Send content of frame buffer to 
	FrameBuffer::Present();

	// Debug
	SaveFrameBuffer();
}
void Level_1_Free()
{
}
void Level_1_Unload()
{

}


// ----------------------------------------------------------------------------
// COMMON

void SaveFrameBuffer()
{
	// Save the data to file
	if (AEInputKeyTriggered(VK_F1))
	{
		OpenSaveFileDlg saveDlg;
		if (saveDlg.Save("Save Frame Buffer to Binary", "*.fb"))
		{
			std::string saveFile;
			if (saveDlg.GetNextFilePath(saveFile))
				Rasterizer::FrameBuffer::SaveToFile(saveFile.c_str());
		}
	}

	if (AEInputKeyTriggered(VK_F2))
	{
		OpenSaveFileDlg saveDlg;
		if (saveDlg.Save("Save Frame Buffer to Image", "*.png"))
		{
			std::string saveFile;
			if (saveDlg.GetNextFilePath(saveFile))
				Rasterizer::FrameBuffer::SaveToImageFile(saveFile.c_str());
		}
	}
	if (AEInputKeyTriggered(VK_F3))
	{
		OpenSaveFileDlg openDlg;
		if (openDlg.Open("Open Frame Buffer File", "*.fb"))
		{
			std::string selectedFile;
			if (openDlg.GetNextFilePath(selectedFile))
				Rasterizer::FrameBuffer::LoadFromFile(selectedFile.c_str());
		}
	}
}
void KeyboardInput()
{
	if (AEInputKeyTriggered('R'))
		gAEGameStateNext = AE_GS_RESTART;
	if (AEInputKeyTriggered('Q') && AEInputKeyPressed(AE_KEY_CTRL))
		gAEGameStateNext = AE_GS_QUIT;
}