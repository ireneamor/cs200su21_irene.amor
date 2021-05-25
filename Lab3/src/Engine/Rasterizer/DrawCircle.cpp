#include <AEEngine.h>
#include "Rasterizer.h"

namespace Rasterizer {


#pragma region CIRCLE
	
	// config data
	static EDrawCircleMethod sDC_Method = eDC_PARAMETRIC;
	static int sCirclePrecision = 25;

	EDrawCircleMethod GetDrawCircleMethod()
	{
		return sDC_Method;
	}

	void SetDrawCircleMethod(EDrawCircleMethod dcm)
	{
		sDC_Method = dcm;
	}

	int GetCircleParametricPrecision()
	{
		return sCirclePrecision;
	}

	void SetCircleParametricPrecision(int prec)
	{
		sCirclePrecision = prec;
	}


	// Lab
	void DrawCircle(const AEVec2& center, float radius, const Color& c) 
	{
		if (Round(radius) == 0)
		{
			FrameBuffer::SetPixel(Round(center.x), Round(center.y), c);
			return;
		}

		//Call the correct draw circle function based on method
		switch (sDC_Method)
		{
		case eDC_ALGEBRAIC :
			DrawCircleAlgebraic(center, radius, c);
			break;
		case eDC_PARAMETRIC :
			DrawCircleParametric(center, radius, c);
			break;
		case eDC_PARAMETRIC_INCREMENTAL:
			DrawCircleParametricInc(center, radius, c);
			break;
		case eDC_MIDPOINT :
			DrawCircleMidpoint(center, radius, c);
			break;
		}
	}

	void SetPixelEightWay(u32 cX, u32 cY, u32 x, u32 y, const Color& c)
	{
		FrameBuffer::SetPixel(cX + x, cY + y, c);
		FrameBuffer::SetPixel(cX - x, cY + y, c);
		FrameBuffer::SetPixel(cX - x, cY - y, c);
		FrameBuffer::SetPixel(cX + x, cY - y, c);

		FrameBuffer::SetPixel(cX + y, cY + x, c);
		FrameBuffer::SetPixel(cX - y, cY + x, c);
		FrameBuffer::SetPixel(cX - y, cY - x, c);
		FrameBuffer::SetPixel(cX + y, cY - x, c);
	}

	void DrawCircleAlgebraic(const AEVec2& center, float radius, const Color& c)
	{
		//y = center.y +- sqrt(radius*radius - (center.x + radius

		int xCenter = Round(center.x);
		int yCenter = Round(center.y);

		f32 cut_off = sqrtf(2) / 2.0f;

		//Use algebraic equation to draw top and bottom
		/*for (int x = -Round(cut_off * radius); x <= Round(cut_off * radius); x++)
		{
			int y = sqrtf(radius * radius - x * x);

			//Top
			FrameBuffer::SetPixel(xCenter + x, yCenter + Round(y), c);

			//Bottom
			FrameBuffer::SetPixel(xCenter + x, yCenter - Round(y), c);

		}
		//Use algebraic equation to draw top and bottom
		for (int y = -Round(cut_off * radius); y <= Round(cut_off * radius); y++)
		{
			int x = sqrtf(radius * radius - y * y);

			//Top
			FrameBuffer::SetPixel(xCenter - x, yCenter + Round(y), c);

			//Bottom
			FrameBuffer::SetPixel(xCenter + x, yCenter + Round(y), c);

		}*/

		for (int x = 0; x <= Round(cut_off * radius); x++)
		{
			int y = Round(sqrtf(radius * radius - x * x));
			SetPixelEightWay(xCenter, yCenter, x, y, c);
		}
	}

	void DrawCircleParametric(const AEVec2& center, float radius, const Color& c)
	{
		int xCenter = Round(center.x);
		int yCenter = Round(center.y);

		f32 step = PI / 4.0f / (f32)sCirclePrecision;

		for (f32 angle = 0.0f; angle <= PI/4.0f; angle += step)
		{
			int x = Round(cosf(angle) * radius);
			int y = Round(sinf(angle) * radius);
			SetPixelEightWay(xCenter, yCenter, x, y, c);
		}
	}


	// Challenge 1
	void DrawCircleParametricInc(const AEVec2& center, float radius, const Color& c){}

	// Challenge 2
	void FillCircle(const AEVec2& center, float radius, const Color& c){}

	// Challenge 3
	void FillRing(const AEVec2 & center, float outerRadius, float innerRadius, const Color & c){}
	
	// Challenge 4
	void FillPie(const AEVec2 & center, float radius, float minAngle, float maxAngle, const Color & c){}

	// Extra Credit
	void DrawCircleMidpoint(const AEVec2& center, float radius, const Color& c){}
#pragma endregion

#pragma region ELLIPSE

	static EDrawEllipseMethod sDE_Method = eDE_ALGEBRAIC;

	EDrawEllipseMethod GetDrawEllipseMethod()
	{
		return sDE_Method;
	}

	void SetDrawEllipseMethod(EDrawEllipseMethod dem)
	{
		sDE_Method = dem;
	}

	// Lab
	void DrawEllipse(const AEVec2& center, float A, float B, const Color& c){}
	void SetPixelFourWay(u32 cX, u32 cY, u32 x, u32 y, const Color& c){}
	void DrawEllipseAlgebraic(const AEVec2& center, float A, float B, const Color& c){}
	void DrawEllipseParametric(const AEVec2& center, float A, float B, const Color& c){}

	// Challenge 1
	void DrawEllipseParametricInc(const AEVec2& center, float A, float B, const Color& c){}

	// Challenge 2
	void FillEllipse(const AEVec2& center, float A, float B, const Color& c){}

	// Extra Credit
	void DrawEllipseMidpoint(const AEVec2& center, float A, float B, const Color& c){}
#pragma endregion

}