#pragma once

namespace Rasterizer
{
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
	void FillTriangleNaive(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c);

	/// -----------------------------------------------------------------------
	/// \fn		FillTriangleTopLeft
	/// \brief	Rasterizes a CCW triangle defined by p1, p2, p3, using the top
	///			left method described in class. Each pixel in the triangle share
	///			the same color. 
	/// \param	p1	First triangle vertex position.
	///	\param	p2	Second triangle vertex position.
	///	\param	p3	Third triangle vertex position.
	//	\param	c	Color to fill the triangle with.	
	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c);
}