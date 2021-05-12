// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEMtx33.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the 3x3 matrix library
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AEX_VEC3_H
#define AEX_VEC3_H
// ---------------------------------------------------------------------------

#pragma warning (disable:4201) // nameless struct warning
struct AEVec3
{
	union
	{
		struct 
		{	
			f32	x, y, z;
		};
		f32		v[3];
	};
	
				AEVec3			()							;
				AEVec3			(const AEVec3& rhs)		;
				AEVec3			(f32 xx, f32 yy, f32 zz)	;
	explicit	AEVec3			(f32 xx)					;
															;
	// add or substract										;
	AEVec3		operator+		(const AEVec3& rhs) const	;
	AEVec3		operator+=		(const AEVec3& rhs) ;
	AEVec3		operator-		(const AEVec3& rhs) const	;
	AEVec3		operator-=		(const AEVec3& rhs) ;
															;
	// multiply or divide by a scalar						;
	AEVec3		operator*		(f32 s) const				;
	AEVec3		operator/		(f32 s) const				;
															;
	// get vector length									;
	f32			Length			() const					;
															;
	// calculate normalized vector							;
	AEVec3		Normalize		() const					;
	const AEVec3&	NormalizeThis	()						;
															;
	// dot product											;
	f32			operator*		(const AEVec3& rhs) const	;
															;
	// cross product										;
	AEVec3		Cross			(const AEVec3& rhs) const	;
};

// ---------------------------------------------------------------------------

template <typename T>
AEVec3 operator*(T s, const AEVec3& rhs)
{
	return (rhs * s);
}

#pragma warning (default:4201) // nameless struct warning
// ---------------------------------------------------------------------------
#endif