#include <AEEngine.h>
#include "Rasterizer.h"
#include "DrawLine.h"


namespace Rasterizer
{
	static EDrawLineMethod sDLMethod = eDL_NAIVE;

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		GetDrawLineMethod
	/// \brief	Sets the line rendering method.
	EDrawLineMethod GetDrawLineMethod()
	{
		return sDLMethod;
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	SetDrawLineMethod	
	/// \brief	Sets the line rendering method.
	void SetDrawLineMethod(EDrawLineMethod dlm)
	{
		sDLMethod = dlm;
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawLine
	/// \brief	Wrapper function, draws a line using the current method. 
	void DrawLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//chek for simple cases
		int dX = Round(p2.x - p1.x);
		int dY = Round(p2.y - p1.y);

		//Vertical line
		if (dX == 0)
			DrawVerticalLine(p1, p2, c);
		else if (dY == 0)
			DrawHorizontalLine(p1, p2, c);
		else if(abs((f32)dX) == abs((f32)dY))
			DrawDiagonalLine(p1, p2, c);

		//if not simple case, call appropiate draw line function
		//based on the value of sDLMethod
		else
		{
			switch (sDLMethod)
			{
			case eDL_NAIVE:
				DrawLineNaive(p1, p2, c);
				break;
			case eDL_DDA:
				DrawLineDDA(p1, p2, c);
				break;
			}
		}
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineNaive
	/// \brief	Draws a line using the naive algorithm presented in class, based on the explicit line equation
	void DrawLineNaive(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//explicit line equation to draw the generic line

		float dX = (p2.x - p1.x);
		float dY = (p2.y - p1.y);
		float m = dY / dX;
		float b = p1.y - m * p1.x;

		//Loop on x when the slope is < 1
		if(abs(m) < 1)
		{
			int step = 1;
			if (dX < 0)
				step = -1;

			u32 sX = Round(p1.x);
			u32 eX = Round(p2.x) + step;

			for (u32 x = sX; x != eX; x += step)
				FrameBuffer::SetPixel(x, Round(m*x + b), c);
		}
		//Loop on y when the slope is > 1
		else
		{
			int step = 1;
			if (dY < 0)
				step = -1;

			u32 sY = Round(p1.y);
			u32 eY = Round(p2.y) + step;

			for (u32 y = sY; y != eY; y += step)
				FrameBuffer::SetPixel(Round((y - b) / m), y, c);
		}

	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineDDA
	/// \brief	Draws a line using the DDA algorithm presented in class.
	void DrawLineDDA(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//explicit line equation to draw the generic line

		float dX = (p2.x - p1.x);
		float dY = (p2.y - p1.y);
		float m = dY / dX;
		float b = p1.y - m * p1.x;

		//Loop on x when the slope is < 1
		if (abs(m) < 1)
		{
			int step = 1;
			f32 step_y = dY > 0 ? abs(m) : -abs(m);
			if (dX < 0)
				step = -1;

			u32 sX = Round(p1.x);
			u32 eX = Round(p2.x) + step;
			u32 y = Round(p1.y);
			for (u32 x = sX; x != eX, y != Round(p2.y); x += step, y += step_y)
				FrameBuffer::SetPixel(x, Round(y), c);
		}
		//Loop on y when the slope is > 1
		else
		{
			f32 mInv = 1.0f / m;
			f32 step_x = dX > 0 ? abs(mInv) : -abs(mInv);
			int step = 1;
			if (dY < 0)
				step = -1;

			u32 sY = Round(p1.y);
			u32 eY = Round(p2.y) + step;
			u32 x = Round(p1.x);
			for (u32 y = sY; y != eY, x != Round(p2.x); y += step, x += step_x)
				FrameBuffer::SetPixel(Round(x), y, c);
		}
	}

	void DrawLineBresenham(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawRect
	/// \brief	Wrapper function, draws a rectangle using the above line method. 
	void DrawRect(const AEVec2& r, const AEVec2& size, const Color& c){}

	/// -----------------------------------------------------------------------
	///	LINE ALGORITHM IMPLEMENTATIONS

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawHorizontalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c){}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawVerticalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c){}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawDiagonalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c){}
}
