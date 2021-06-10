// ----------------------------------------------------------------------------
// Project Name		:	2.3 Draw Triangle
// File Name		:	DrawTriangle.cpp
// Author			:	Irene Amor Mendez
// Creation Date	:	June 10, 2021
// Purpose			:	Calculates different triangle filling methods after
//						classifying them
// ----------------------------------------------------------------------------

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
		float yS = vtx[TOP]->y;
		float yE = vtx[MID]->y;

		float xL = vtx[TOP]->x;
		float xR = vtx[TOP]->x;

		float slopeLeft = midIsLeft ? mInvTM : mInvTB;
		float slopeRight = midIsLeft ? mInvTB : mInvTM;

		//2. TRAVERSAL
		for (int i = 0; i < 2; i++)			//Both regions
		{
			//2.1. Top region

			for (int y = Round(yS); y >= Round(yE); y--)	//Trav on y: for every scan line
			{
				for (int x = Round(xL); x <= Round(xR); x++)
				{
					FrameBuffer::SetPixel(x, y, c);
				}

				//Update xL and xR
				xL -= slopeLeft;
				xR -= slopeRight;
			}

			//2.2. Change to bottom region

			//Update loop limits to draw bottom region
			yS = vtx[MID]->y;
			yE = vtx[BOT]->y;

			//Update xL or xR to the middle x value
			if (midIsLeft)
				xL = vtx[MID]->x;
			else
				xR = vtx[MID]->x;

			slopeLeft = midIsLeft ? mInvMB : mInvTB;
			slopeRight = midIsLeft ? mInvTB : mInvMB;
		}
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
		float yS = vtx[TOP]->y;
		float yE = vtx[MID]->y;

		float xL = vtx[TOP]->x;
		float xR = vtx[TOP]->x;

		float slopeLeft = midIsLeft ? mInvTM : mInvTB;
		float slopeRight = midIsLeft ? mInvTB : mInvTM;

		//2. TRAVERSAL
		for (int i = 0; i < 2; i++)			//Both regions
		{
			//2.1. Top region

			for (int y = Ceiling(yS); y >= Ceiling(yE) + 1; y--)	//Trav on y: for every scan line
			{
				for (int x = Floor(xL); x <= Floor(xR) - 1; x++)
				{
					FrameBuffer::SetPixel(x, y, c);
				}

				//Update xL and xR
				xL -= slopeLeft;
				xR -= slopeRight;
			}

			//2.2. Change to bottom region

			//Update loop limits to draw bottom region
			yS = vtx[MID]->y;
			yE = vtx[BOT]->y;

			//Update xL or xR to the middle x value
			if (midIsLeft)
				xL = vtx[MID]->x;
			else
				xR = vtx[MID]->x;

			slopeLeft = midIsLeft ? mInvMB : mInvTB;
			slopeRight = midIsLeft ? mInvTB : mInvMB;
		}
	}

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
		float yS = vtx[TOP]->mPosition.y;
		float yE = vtx[MID]->mPosition.y;
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
			yS = vtx[MID]->mPosition.y;
			yE = vtx[BOT]->mPosition.y;

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
	{
		//1.SETUP
		//1.1. Determine case
		int TOP, MID, BOT;
		bool midIsLeft = DetermineCase(v0.mPosition.y, v1.mPosition.y, v2.mPosition.y, TOP, MID, BOT);
		const Vertex* vtx[3] = { &v0, &v1, &v2 };

		//1.2. Compute slopes
		//Inverse slopes
		float mInvTB = (vtx[BOT]->mPosition.x - vtx[TOP]->mPosition.x) / (vtx[BOT]->mPosition.y - vtx[TOP]->mPosition.y);
		float mInvTM = (vtx[MID]->mPosition.x - vtx[TOP]->mPosition.x) / (vtx[MID]->mPosition.y - vtx[TOP]->mPosition.y);
		float mInvMB = (vtx[BOT]->mPosition.x - vtx[MID]->mPosition.x) / (vtx[BOT]->mPosition.y - vtx[MID]->mPosition.y);

		//Slope values
		float slopeLeft = midIsLeft ? mInvTM : mInvTB;
		float slopeRight = midIsLeft ? mInvTB : mInvTM;

		//1.3. Set the data for the loop
		//Loop variables
		float yS = vtx[TOP]->mPosition.y;
		float yE = vtx[MID]->mPosition.y;
		float xL = vtx[TOP]->mPosition.x;
		float xR = vtx[TOP]->mPosition.x;

		//Set colors of the pixels on each side
		Color cL = vtx[TOP]->mColor;
		Color cR = vtx[TOP]->mColor;

		//1.3. Plane data
		//Normal for RED
		AEVec3 V0V1 = { v1.mPosition.x - v0.mPosition.x, v1.mPosition.y - v0.mPosition.y, v1.mColor.r - v0.mColor.r };
		AEVec3 V0V2 = { v2.mPosition.x - v0.mPosition.x, v2.mPosition.y - v0.mPosition.y, v2.mColor.r - v0.mColor.r };
		AEVec3 rNormal = V0V1.Cross(V0V2);

		//Normal for GREEN
		V0V1.z = v1.mColor.g - v0.mColor.g;
		V0V2.z = v2.mColor.g - v0.mColor.g;
		AEVec3 gNormal = V0V1.Cross(V0V2);

		//Normal for BLUE
		V0V1.z = v1.mColor.b - v0.mColor.b;
		V0V2.z = v2.mColor.b - v0.mColor.b;
		AEVec3 bNormal = V0V1.Cross(V0V2);

		//Normal for ALPHA
		V0V1.z = v1.mColor.a - v0.mColor.a;
		V0V2.z = v2.mColor.a - v0.mColor.a;
		AEVec3 aNormal = V0V1.Cross(V0V2);

		//Color steps
		Color dx = {-(rNormal.x / rNormal.z), -(gNormal.x / gNormal.z), -(bNormal.x / bNormal.z), -(aNormal.x / aNormal.z)};
		Color dy = {-(rNormal.y / rNormal.z), -(gNormal.y / gNormal.z), -(bNormal.y / bNormal.z), -(aNormal.y / aNormal.z)};
		

		//2.TRAVERSE
		for (int i = 1; i <= 2; i++)
		{
			//2.1. Start with the data for TOP-MID
			//Loop on y
			for (int y = Ceiling(yS); y >= Ceiling(yE) + 1; y--)
			{
				//Compute the color increment
				Color c = cL;

				//Loop on x
				for (int x = Floor(xL); x <= Floor(xR) - 1; x++)
				{
					//Set pixel on the screen
					FrameBuffer::SetPixel(x, y, c);

					//Change the color
					c += dx;
				}

				//Update color and position
				xL -= slopeLeft;
				xR -= slopeRight;
				cL.r -= dy.r + slopeLeft * dx.r;
				cL.g -= dy.g + slopeLeft * dx.g;
				cL.b -= dy.b + slopeLeft * dx.b;
				cL.a -= dy.a + slopeLeft * dx.a;
			}

			//2.2. Change the data for MID-BOT
			//Change the first and last values of y
			yS = vtx[MID]->mPosition.y;
			yE = vtx[BOT]->mPosition.y;

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
		}
	}

	/// ------------------------------------------------------------------------
	/// \fn		DrawTriangleBarycentric
	/// \brief	Same method as the function above but computes the 
	///			This method uses computes the barycentric coordinates INCREMENTALLY.
	///			
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangleBarycentric(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	
	{
		//1.SETUP
		//1.1. Determine case
		int TOP, MID, BOT;
		bool midIsLeft = DetermineCase(v0.mPosition.y, v1.mPosition.y, v2.mPosition.y, TOP, MID, BOT);
		const Vertex* vtx[3] = { &v0, &v1, &v2 };

		//1.2. Compute slopes
		//Inverse slopes
		float mInvTB = (vtx[BOT]->mPosition.x - vtx[TOP]->mPosition.x) / (vtx[BOT]->mPosition.y - vtx[TOP]->mPosition.y);
		float mInvTM = (vtx[MID]->mPosition.x - vtx[TOP]->mPosition.x) / (vtx[MID]->mPosition.y - vtx[TOP]->mPosition.y);
		float mInvMB = (vtx[BOT]->mPosition.x - vtx[MID]->mPosition.x) / (vtx[BOT]->mPosition.y - vtx[MID]->mPosition.y);

		//Slope values
		float slopeLeft = midIsLeft ? mInvTM : mInvTB;
		float slopeRight = midIsLeft ? mInvTB : mInvTM;

		//1.3. Set the data for the loop
		//Loop variables
		float yS = vtx[TOP]->mPosition.y;
		float yE = vtx[MID]->mPosition.y;
		float xL = vtx[TOP]->mPosition.x;
		float xR = vtx[TOP]->mPosition.x;

		//1.4 Calculate normal of the triangle
		AEVec2 v0v1 = v0.mPosition - v1.mPosition;
		AEVec2 v0v2 = v0.mPosition - v2.mPosition;
		float n = v0v1.CrossMag(v0v2);

		//2.TRAVERSE
		for (int i = 1; i <= 2; i++)
		{
			//2.1. Start with the data for TOP-MID
			//Loop on y
			for (int y = Ceiling(yS); y >= Ceiling(yE) + 1; y--)
			{
				//Loop on x
				for (int x = Floor(xL); x <= Floor(xR) - 1; x++)
				{
					//Calculate vectors from point to each vertex
					AEVec2 P = { (float)x, (float)y };
					AEVec2 PV0 = P - v0.mPosition;
					AEVec2 PV1 = P - v1.mPosition;
					AEVec2 PV2 = P - v2.mPosition;

					//Calculate the barycentric coordinates
					float Lambda1 = (PV2.CrossMag(PV0)) / n;
					float Lambda2 = (PV0.CrossMag(PV1)) / n;
					float Lambda0 = 1 - Lambda1 - Lambda2;

					//If the point is outside the triangle, skip next steps
					if (Lambda0 < 0 || Lambda1 < 0 || Lambda2 < 0)
						continue;

					//Calculate the color
					Color color = v0.mColor;
					color.r = (Lambda0 * v0.mColor.r) + (Lambda1 * v1.mColor.r) + (Lambda2 * v2.mColor.r);
					color.g = (Lambda0 * v0.mColor.g) + (Lambda1 * v1.mColor.g) + (Lambda2 * v2.mColor.g);
					color.b = (Lambda0 * v0.mColor.b) + (Lambda1 * v1.mColor.b) + (Lambda2 * v2.mColor.b);
					color.a = (Lambda0 * v0.mColor.a) + (Lambda1 * v1.mColor.a) + (Lambda2 * v2.mColor.a);
					
					//Set the pixel
					FrameBuffer::SetPixel(x, y, color);
				}

				//Update position
				xL -= slopeLeft;
				xR -= slopeRight;
			}
			
			//2.2. Change the data for MID-BOT
			//Change the first and last values of y
			yS = vtx[MID]->mPosition.y;
			yE = vtx[BOT]->mPosition.y;

			//Set the leftmost value of x
			if (midIsLeft)
				xL = vtx[MID]->mPosition.x;
			else
				xR = vtx[MID]->mPosition.x;

			//Update the slopes and steps depending on which vertex is to the left
			slopeLeft = midIsLeft ? mInvMB : mInvTB;
			slopeRight = midIsLeft ? mInvTB : mInvMB;
		}
	}

}
