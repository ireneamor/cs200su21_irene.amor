#include <AEEngine.h>
#include "Rasterizer.h"


namespace Rasterizer
{
	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawRect
	/// \brief	Wrapper function, draws a rectangle using the above line method. 
	void DrawRect(const AEVec2& r, const AEVec2& size, const Color& c)
	{
		//DrawHorizontalLine((r - (size/2)), (r + (size / 2)), c);
		//DrawVerticalLine((r - (size / 2)), (r + (size / 2)), c);

		AEVec2 bottomRight, topLeft;
		bottomRight.x = r.x + (size.x / 2); bottomRight.y = r.y - (size.y / 2);
		topLeft.x = r.x - (size.y / 2); topLeft.y = r.y + (size.y / 2);

		DrawHorizontalLine(bottomRight, (r + (size / 2)), c);
		DrawVerticalLine(topLeft, (r + (size / 2)), c);
	}

	/// -----------------------------------------------------------------------
	///	LINE ALGORITHM IMPLEMENTATIONS

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawHorizontalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float size = 0;
		if (p2.x > p1.x)
		{
			size = p2.x - p1.x;

			for (int i = 0; i < size; i++)
				FrameBuffer::SetPixel(round(p1.x + i), round(p1.y), c);
		}
		else
		{
			size = p1.x - p2.x;

			for (int i = 0; i < size; i++)
				FrameBuffer::SetPixel(round(p1.x - i), round(p1.y), c);
		}
	
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawVerticalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float size = 0;
		if (p2.y > p1.y)
		{
			size = p2.y - p1.y;

			for (int i = 0; i < size; i++)
				FrameBuffer::SetPixel(round(p1.x), round(p1.y + i), c);
		}
		else
		{
			size = p1.y - p2.y;

			for (int i = 0; i < size; i++)
				FrameBuffer::SetPixel(round(p1.x), round(p1.y - i), c);
		}

	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawDiagonalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float sizeY = 0;
		float sizeX = 0;

		if (p2.y > p1.y)
		{
			sizeY = p2.y - p1.y;
			int j = 0;

			if (p2.x > p1.x)
			{
				sizeX = p2.x - p1.x;

				for (int i = 0; i < sizeY, j < sizeX; i++, j++)
					FrameBuffer::SetPixel(round(p1.x + j), round(p1.y + i), c);
			}
			else
			{
				sizeX = p1.x - p2.x;

				for (int i = 0; i < sizeY, j > sizeX; i++, j--)
					FrameBuffer::SetPixel(round(p1.x - j), round(p1.y + i), c);
			}
		}
		else
		{
			sizeY = p1.y - p2.y;
			int j = 0;

			if (p2.x > p1.x)
			{
				sizeX = p2.x - p1.x;

				for (int i = 0; i < sizeY, j < sizeX; i++, j++)
					FrameBuffer::SetPixel(round(p1.x + j), round(p1.y - i), c);
			}
			else
			{
				sizeX = p1.x - p2.x;

				for (int i = 0; i < sizeY, j > sizeX; i++, j--)
					FrameBuffer::SetPixel(round(p1.x - j), round(p1.y - i), c);
			}

		}
				
	}
}