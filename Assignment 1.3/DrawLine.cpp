#include <AEEngine.h>
#include "Rasterizer.h"


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
		//Horiontal line
		else if (dY == 0)
			DrawHorizontalLine(p1, p2, c);
		//45º diagonal line
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
			case eDL_BRESENHAM:
				DrawLineBresenham(p1, p2, c);
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
		float dX = (p2.x - p1.x);		//Difference on x and y
		float dY = (p2.y - p1.y);
		float m = dY / dX;				//Slope of the line
		float b = p1.y - (m * p1.x);	//Bias of the line

		//If the slope > 1, calculate y through x
		if (abs(m) < 1.0f)
		{
			int step_x = dX > 0 ? 1 : -1;		//Change of x
			int sX = Round(p1.x);				//X value of first point
			int eX = Round(p2.x) + step_x;		//X value of last point

			//Add difference to x, and calculate y with the slope
			for (int x = sX; x != eX; x += step_x)
			{
				f32 y = m * x + b;
				FrameBuffer::SetPixel(x, Round(y), c);
			}
		}
		else
		{
			int step_y = dY > 0 ? 1 : -1;		//Change of y
			int sY = Round(p1.y);				//Y value of first point
			int eY = Round(p2.y) + step_y;		//Y value of last point
			f32 mInv = 1.0f / m;				//Inverse of the slope to avoid dividing

			//Add difference to y, and calculate x with the slope
			for (int y = sY; y != eY; y += step_y)
			{
				f32 x = (y - b) * mInv;
				FrameBuffer::SetPixel(Round(x), y, c);
			}
		}
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineDDA
	/// \brief	Draws a line using the DDA algorithm presented in class.
	void DrawLineDDA(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//explicit line equation to draw the generic line

		float dX = (p2.x - p1.x);		//Difference on x and y
		float dY = (p2.y - p1.y);
		float m = dY / dX;				//Slope of the line
		float b = p1.y - (m * p1.x);	//Bias of the line

		//If the slope > 1, calculate y with the slope
		if (abs(m) < 1.0f)
		{
			int step_x = dX > 0 ? 1 : -1;				//Change of y and x
			f32 step_y = dY > 0 ? abs(m) : -abs(m);
			int sX = Round(p1.x);						//X value of first point
			int eX = Round(p2.x) + step_x;				//X value of last point

			//Add difference to x and y
			f32 y = p1.y;
			for (int x = sX; x != eX; x += step_x, y += step_y)
			{
				FrameBuffer::SetPixel(x, Round(y), c);
			}
		}
		else
		{
			f32 mInv = 1.0f / m;							//Inverse of the slope to avoid dividing
			f32 step_x = dX > 0 ? abs(mInv) : -abs(mInv);	//Change of y and x
			int step_y = dY > 0 ? 1 : -1;
			int sY = Round(p1.y);							//Y value of first point
			int eY = Round(p2.y) + step_y;					//Y value of last point

			//Add difference to x and y
			f32 x = p1.x;
			for (int y = sY; y != eY; y += step_y, x += step_x)
			{
				FrameBuffer::SetPixel(Round(x), y, c);
			}
		}
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineBresenham
	/// \brief	Draws a line using the Bresenham algorithm presented in class.
	void DrawLineBresenham(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float dX = (p2.x - p1.x);		//Difference on x and y
		float dY = (p2.y - p1.y);
		float m = dY / dX;				//Slope of the line

		int step_x = dX > 0 ? 1 : -1;	//Change of y and x
		int step_y = dY > 0 ? 1 : -1;
		int eX = Round(p2.x) + step_x;	//X value at last point

		int dP = 2 * dY - dX;			//Decision parameter

		int x = Round(p1.x);			//Position at x and y
		int y = Round(p1.y);

		while (x != eX)
		{
			FrameBuffer::SetPixel(x, y, c);					//Set the pixel

			//e.g.: In the case 1
			//Choose between E or NE pixels
			if (abs(m) < 1)
			{
				//Passes closer to NE
				if (dP > 0)
				{
					y += step_y;							//Increase y
					dP = dP + 2 * abs(dY) - 2 * abs(dX);	//Change the decision parameter
				}
				//Passes closer to E
				else
				{
					dP = dP + 2 * abs(dY);					//Change the decision parameter
				}

				x += step_x;								//Increse x every time
			}
			//e.g.: In the case 2
			//Choose between N or NE pixels
			else
			{
				//Passes closer to N
				if (dP > 0)
				{	
					x += step_x;							//Increse x 
					dP = dP + 2 * abs(dX) - 2 * abs(dY);	//Change the decision parameter
				}
				//Passes closer to NE
				else
				{
					dP = dP + 2 * abs(dX);					//Change the decision parameter
				}

				y += step_y;								//Increse y every time
			}
		}
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawRect
	/// \brief	Wrapper function, draws a rectangle using the above line method. 
	void DrawRect(const AEVec2& r, const AEVec2& size, const Color& c)
	{
		//Get each corner of the rectangle
		AEVec2 bottomRight, bottomLeft, topLeft, topRight;
		bottomRight = { r.x + (size.x / 2), r.y - (size.y / 2) };
		bottomLeft = { r.x - (size.x / 2), r.y - (size.y / 2) };
		topLeft = { r.x - (size.y / 2), r.y + (size.y / 2) };
		topRight = { r.x + (size.x / 2), r.y + (size.y / 2) };

		//Draw the horizontal and vertical lines ofthe rectangle
		DrawHorizontalLine(bottomLeft, bottomRight, c);
		DrawHorizontalLine(topLeft, topRight, c);
		DrawVerticalLine(bottomLeft, topLeft, c);
		DrawVerticalLine(bottomRight, topRight, c);
	}

	/// -----------------------------------------------------------------------
	///	LINE ALGORITHM IMPLEMENTATIONS

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawHorizontalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		int incr = 1;		//Ammount of pixels to increase every time

		//If the second point is to the left
		if (p2.x < p1.x)
			incr = -1;		//Decrease instead

		//From the first to the second point, set every pixel to the color
		for (int x = Round(p1.x); x != Round(p2.x); x += incr)
			FrameBuffer::SetPixel(round(x), round(p1.y), c);
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawVerticalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		int incr = 1;		//Ammount of pixels to increase every time

		//If the second point is bellow
		if (p2.y < p1.y)
			incr = -1;		//Decrease instead

		//From the first to the second point, set every pixel to the color
		for (int y = Round(p1.y); y != Round(p2.y); y += incr)
			FrameBuffer::SetPixel(round(p1.x), round(y), c);
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawDiagonalLine
	/// \brief	Helper function that draws a horizontal line from left to right. 
	void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		int incr_x = 1;		//Ammount of pixels to increase horizontally every time
		int incr_y = 1;		//Ammount of pixels to increase vertically every time

		//If the second point is to the left
		if (p2.x < p1.x)
			incr_x = -1;	//Decrease instead

		//If the second point is bellow
		if (p2.y < p1.y)
			incr_y = -1;	//Decrease instead

		//From the first to the second point, set every pixel to the color
		//Set beginning of x and y
		u32 y = Round(p1.y);
		//Increase or decrease x and y
		for (u32 x = Round(p1.x); x != Round(p2.x), y != Round(p2.y); x += incr_x, y += incr_y)
			FrameBuffer::SetPixel(Round(x), Round(y), c);

	}
}
