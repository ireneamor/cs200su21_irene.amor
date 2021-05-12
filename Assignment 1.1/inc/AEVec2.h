// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEVector2.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the 2D vector library
// History			:
// - 2008/01/31		:	- initial implementation
// - 2012/02/15		:	- added a point2d typedef
// ---------------------------------------------------------------------------

#ifndef AE_VEC2_H
#define AE_VEC2_H
// ---------------------------------------------------------------------------

#pragma warning (disable:4201) // nameless struct warning
struct AEVec2
{
	union
	{
		struct
		{
			f32 x, y;
		};
		f32 v[2];
	};

	// ------------------------------------------------------------------------
				AEVec2();
				AEVec2(const AEVec2& rhs);
				AEVec2(f32 xx, f32 yy);
	explicit	AEVec2(f32 xx); //! flag as explicit to prevent implicit conversion from f32 to AEVec2

	// add or substract
	AEVec2		operator+	(const AEVec2& rhs) const;
	AEVec2		operator+=	(const AEVec2& rhs);
	AEVec2		operator-	(const AEVec2& rhs) const;
	AEVec2		operator-=	(const AEVec2& rhs);

	// multiply or divide by a scalar
	AEVec2		operator*	(f32 s) const;
	AEVec2		operator*=	(f32 s);
	AEVec2		operator/	(f32 s) const;
	AEVec2		operator/=	(f32 s);

	// unitary negation
	AEVec2		operator-() const;

	// vector length
	f32			Length() const;
	f32			LengthSq() const;

	// Distance
	f32			Distance(const AEVec2 &rhs);
	f32			DistanceSq(const AEVec2 &rhs);

	// Normalize
	AEVec2		Normalize() const;
	const AEVec2 &NormalizeThis();

	// dot product
	f32			Dot(const AEVec2& rhs) const;
	f32			operator*		(const AEVec2& rhs) const;

	// perpendicular, projection & perpendiculat projection
	AEVec2		Perp			()const;
	AEVec2		Project			(const AEVec2& rhs)const;
	AEVec2		ProjectPerp		(const AEVec2& rhs)const;

	// cross product magnitude
	f32			CrossMag(const AEVec2& rhs);

	// To and From Angle
	f32			GetAngle();
	void		FromAngle(f32 angle);
	void		RotateBy(f32 angle_rad);
	void		RotateByDeg(f32 angle_deg);

	// Static interface
	static AEVec2 Random(f32 min_x, f32 max_x, f32 min_y, f32 max_y);
	static AEVec2 Random01();
	static AEVec2 Lerp(AEVec2 start, AEVec2 end, f32 tn);


};

// typedef for point2D
typedef AEVec2 AEPoint2;
typedef AEVec2 AEVector2;

// ---------------------------------------------------------------------------

void	AEVector2Zero				(AEVector2* pResult);
void	AEVector2Set				(AEVector2* pResult, f32 x, f32 y);

void	AEVector2Neg				(AEVector2* pResult, AEVector2* pVec0);
void	AEVector2Add				(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1);
void	AEVector2Sub				(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1);
void	AEVector2Normalize			(AEVector2* pResult, AEVector2* pVec0);
void	AEVector2Perp				(AEVector2* pResult, AEVector2* pVec0);

void	AEVector2Scale				(AEVector2* pResult, AEVector2* pVec0, f32 c);
void	AEVector2ScaleAdd			(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1, f32 s);
void	AEVector2ScaleSub			(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1, f32 s);

void	AEVector2Project			(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1);
void	AEVector2ProjectPerp		(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1);

void	AEVector2Lerp				(AEVector2* pResult, AEVector2* pVec0, AEVector2* pVec1, float t);

f32		AEVector2Length				(AEVector2* pVec0);
f32		AEVector2SquareLength		(AEVector2* pVec0);
f32		AEVector2Distance			(AEVector2* pVec0, AEVector2* pVec1);
f32		AEVector2SquareDistance		(AEVector2* pVec0, AEVector2* pVec1);

f32		AEVector2DotProduct			(AEVector2* pVec0, AEVector2* pVec1);
f32		AEVector2CrossProductMag	(AEVector2* pVec0, AEVector2* pVec1);

void	AEVector2FromAngle			(AEVector2* pResult, f32 angle);
f32		AEVector2ToAngle			(AEVector2 * pVec);

#pragma warning (default:4201) // nameless struct warning


// ---------------------------------------------------------------------------

#endif // VEC2_H