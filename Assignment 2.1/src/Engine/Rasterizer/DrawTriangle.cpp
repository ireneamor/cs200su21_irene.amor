#include <AEEngine.h>
#include "Rasterizer.h"

namespace Rasterizer
{
	static EDrawTriangleMethod sDTMethod = eDT_BARYCENTRIC;

	//Returns whether the middle is on the left
	bool DetermineCase(float y0, float y1, float y2, int& t, int& m, int& b)
	{
		if (y0 >= y1)
		{
			if (y1 >= y2)		//case 1
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
			if (y0 >= y2)		//case 4
			{
				t = 1;	m = 0;	b = 2;
				return false;
			}
			else if (y1 >= y2)	//case 5
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

	/// -----------------------------------------------------------------------
	/// \fn		FillTriangleNaive
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2 using the naive 
	///			algorithm described in class. Each pixel in the triangle share
	///			the same color.
	/// \param	v0	First triangle vertex position.
	///	\param	v1	Second triangle vertex position.
	///	\param	v2	Third triangle vertex position.
	///	\param	c	Color to fill the triangle with.
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
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method described in class. Each pixel in the triangle share
	///			the same color. 
	/// \param	v0	First triangle vertex position.
	///	\param	v1	Second triangle vertex position.
	///	\param	v2	Third triangle vertex position.
	///	\param	c	Color to fill the triangle with.	
	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{}

	/// -----------------------------------------------------------------------
	/// \fn		GetDrawTriangleMethod
	/// \brief	Getter for the current method used when DrawTriangle is called. 
	EDrawTriangleMethod GetDrawTriangleMethod()
	{
		return sDTMethod;
	}

	/// -----------------------------------------------------------------------
	/// \fn		SetDrawTriangleMethod
	/// \brief	Setter for the current method used when DrawTriangle is called. 
	void SetDrawTriangleMethod(EDrawTriangleMethod dtm)
	{
		sDTMethod = dtm;
	}

	/// -----------------------------------------------------------------------
	/// \fn		DrawTriangle
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2. It delegates
	///			the call to one of the methods implemented internally. See
	///			enum EDrawTriangleMethod above. 
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		switch (sDTMethod)
		{
		case eDT_BARYCENTRIC:
			DrawTriangleBarycentric(v0, v1, v2);
			break;
		case eDT_BILINEAR:
			DrawTriangleBiLinear(v0, v1, v2);
			break;
		case eDT_PLANE_NORMAL:
			DrawTrianglePlaneNormal(v0, v1, v2);
			break;
		}
	}

	/// ------------------------------------------------------------------------
	/// \fn		DrawTriangleBiLinear
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method and the bi-linear interpolation method for color 
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangleBiLinear(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		//1.SETUP
		//1.1. Determine case
		int TOP, MID, BOT;
		bool midIsLeft = DetermineCase(v0.mPosition.y, v1.mPosition.y, v2.mPosition.y, TOP, MID, BOT);
		const Vertex* vtx[3] = { &v0, &v1, &v2};

		//1.2. Compute slopes
		//Inverse slopes
		float mInvTB = (vtx[BOT]->mPosition.x - vtx[TOP]->mPosition.x) / (vtx[BOT]->mPosition.y - vtx[TOP]->mPosition.y);
		float mInvTM = (vtx[MID]->mPosition.x - vtx[TOP]->mPosition.x) / (vtx[MID]->mPosition.y - vtx[TOP]->mPosition.y);
		float mInvMB = (vtx[BOT]->mPosition.x - vtx[MID]->mPosition.x) / (vtx[BOT]->mPosition.y - vtx[MID]->mPosition.y);

		//Slope values
		float slopeLeft = midIsLeft ? mInvTM : mInvTB;
		float slopeRight = midIsLeft ? mInvTB : mInvTM;

		//Slopes for color variation
		Color d_TM = (vtx[MID]->mColor - vtx[TOP]->mColor) * (1.0f / (vtx[MID]->mPosition.y - vtx[TOP]->mPosition.y));
		Color d_TB = (vtx[BOT]->mColor - vtx[TOP]->mColor) * (1.0f / (vtx[BOT]->mPosition.y - vtx[TOP]->mPosition.y));
		Color d_MB = (vtx[BOT]->mColor - vtx[MID]->mColor) * (1.0f / (vtx[BOT]->mPosition.y - vtx[MID]->mPosition.y));

		//1.3. Set the data for the loop
		//Loop variables
		int yS = Round(vtx[TOP]->mPosition.y);
		int yE = Round(vtx[MID]->mPosition.y);
		float xL = vtx[TOP]->mPosition.x;
		float xR = vtx[TOP]->mPosition.x;

		//Set colors of the pixels on each side
		Color cL = vtx[TOP]->mColor;
		Color cR = vtx[TOP]->mColor;

		//Step values
		Color step_L = midIsLeft ? d_TM : d_TB;
		Color step_R = midIsLeft ? d_TB : d_TM;

		//2.TRAVERSE
		for (int i = 1; i <= 2; i++)
		{
			//2.1. Start with the data for TOP-MID
			//Loop on y
			for (int y = Ceiling(yS); y >= Ceiling(yE) + 1; y--)
			{
				//Compute the color increment
				Color step_C = (cR - cL) * (1.0f / (xR - xL));
				Color c = cL;

				//Loop on x
				for (int x = Floor(xL); x <= Floor(xR) - 1; x++)
				{
					//Set pixel on the screen
					FrameBuffer::SetPixel(x, y, c);

					//Change the color
					c += step_C;
				}

				//Update color and position
				xL -= slopeLeft;
				xR -= slopeRight;
				cL -= step_L;
				cR -= step_R;
			}

			//2.2. Change the data for MID-BOT
			//Change the first and last values of y
			yS = Round(vtx[MID]->mPosition.y);
			yE = Round(vtx[BOT]->mPosition.y);

			//Set the leftmost value of x
			if (midIsLeft)
			{
				xL = vtx[MID]->mPosition.x;
				cL = vtx[MID]->mColor;
			}
			else
			{
				xR = vtx[MID]->mPosition.x;
				cR = vtx[MID]->mColor;
			}

			//Update the slopes and steps depending on which vertex is to the left
			slopeLeft = midIsLeft ? mInvMB : mInvTB;
			slopeRight = midIsLeft ? mInvTB : mInvMB;
			step_L = midIsLeft ? d_MB : d_TB;
			step_R = midIsLeft ? d_TB : d_MB;
		}
	}

	/// ------------------------------------------------------------------------
	/// \fn		DrawTrianglePlaneNormal
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method and the color interpolation method using the normal
	///			plane equation (you cannot use the bi-linear method).
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTrianglePlaneNormal(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{}

	/// ------------------------------------------------------------------------
	/// \fn		DrawTriangleBarycentric
	/// \brief	Same method as the function above but computes the 
	///			This method uses computes the barycentric coordinates INCREMENTALLY.
	///			
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangleBarycentric(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{}

}