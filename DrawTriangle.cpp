#include <AEEngine.h>
#include "Rasterizer.h"
namespace Rasterizer
{
	//Returns whether the middle is on the left
	bool DetermineCase(float y0, float y1, float y2, int &t, int &m, int& b)
	{
		if (y0 >= y1)
		{
			if(y1 >= y2)		//case 1
			{
				t = 0;	m = 1;	b = 2;
				return true;
			}
			else if (y0 >= y2)	//case 2
			{
				t = 0;	m = 2;	b = 1;
				return false;
			}
			else
			{					//case 3
				t = 2;	m = 0;	b = 1;
				return true;
			}

		}
		else
		{
			if (y0>= y2)		//case 4
			{ 
				t = 1;	m = 0;	b = 2;
				return false;
			}
			else if(y1 >= y2)	//case 5
			{
				t = 1;	m = 2;	b = 0;
				return true;
			}
			else
			{					//case 6
				t = 2;	m = 1;	b = 0;
				return false;
			}
		}
	}

	// TODO
	/// -----------------------------------------------------------------------
	/// \fn		FillTriangleNaive
	/// \brief	Rasterizes a CCW triangle defined by p1, p2, p3 using the naive 
	///			algorithm described in class. Each pixel in the triangle share
	///			the same color.
	/// \param	p1	First triangle vertex position.
	///	\param	p2	Second triangle vertex position.
	///	\param	p3	Third triangle vertex position.
	//	\param	c	Color to fill the triangle with.
	void FillTriangleNaive(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{
		//1.SETUP
		//1.1. Determine case
		int TOP, MID, BOT;
		bool midIsLeft = DetermineCase(v0.y, v1.y, v2.y, TOP, MID, BOT);
		const AEVec2* vtx[3] = { &v0, &v1, &v2 };

		//1.2. Compute inverse slopes
		float mInvTB = (vtx[BOT]->x - vtx[TOP]->x) / (vtx[BOT]->y - vtx[TOP]->y);
		float mInvTM = (vtx[MID]->x - vtx[TOP]->x) / (vtx[MID]->y - vtx[TOP]->y);
		float mInvMB = (vtx[BOT]->x - vtx[MID]->x) / (vtx[BOT]->y - vtx[MID]->y);

		//1.3. Initialize loop variables
		int yS = Round(vtx[TOP]->y);
		int yE = Round(vtx[MID]->y);

		float xL = vtx[TOP]->x;
		float xR = vtx[TOP]->x;

		float slopeLeft = midIsLeft ? mInvTM : mInvTB;
		float slopeRight = midIsLeft ? mInvTB : mInvTM;

		//2. TRAVERSAL
		for (int i = 0; i < 2; i++)			//Both regions
		{
			for (int y = yS; y >= yE; y--)	//Trav on y: for every scan line
			{
				for (int x = Round(xL); x <= Round(xR); x++)
				{
					FrameBuffer::SetPixel(x, y, c);
				}

				//Update xL and xR
				xL -= slopeLeft;
				xR -= slopeRight;
			}

			//Update loop limits to draw bottom region
			yS = Round(vtx[MID]->y);
			yE = Round(vtx[BOT]->y);

			//Update xL or xR to the middle x value
			if (midIsLeft)
				xL = vtx[MID]->x;
			else
				xR = vtx[MID]->x;

			slopeLeft = midIsLeft ? mInvMB : mInvTB;
			slopeRight = midIsLeft ? mInvTB : mInvMB;
		}
		//2.1. Top region

		//2.2. Bottom region
	}

	/// -----------------------------------------------------------------------
	/// \fn		FillTriangleTopLeft
	/// \brief	Rasterizes a CCW triangle defined by p1, p2, p3, using the top
	///			left method described in class. Each pixel in the triangle share
	///			the same color. 
	/// \param	p1	First triangle vertex position.
	///	\param	p2	Second triangle vertex position.
	///	\param	p3	Third triangle vertex position.
	//	\param	c	Color to fill the triangle with.	
	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{}
}