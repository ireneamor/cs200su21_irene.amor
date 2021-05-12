// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AETransform2D.h
// Author			:	Thomas Komair
// Creation Date	:	02/18/2016
// Purpose			:	Define Transform 2D class
// ---------------------------------------------------------------------------
#ifndef AE_TRANSFORM_2D_H_
#define AE_TRANSFORM_2D_H_
// ---------------------------------------------------------------------------

class AETransform2D
{
public:
	AETransform2D();
	AETransform2D(const AEVec2 & pos, const AEVec2 & scale, const f32 & rot);
	AETransform2D(f32 tx, f32 ty, f32 sx, f32 sy, f32 rot);

	// Matrix computation
	AEMtx33 GetMatrix() const;
	AEMtx33 GetInvMatrix() const;

	// Transform concatenation: T = this * rhs;
	AETransform2D Concat(const AETransform2D & rhs)const;
	AETransform2D InverseConcat(const AETransform2D& rhs)const;
	AETransform2D operator *(const AETransform2D & rhs)const;
	const AETransform2D & operator *=(const AETransform2D &rhs);

public:
	AEVec2	mPosition;		// Position
	AEVec2	mScale;			// Scale
	f32		mOrientation;	// Orientation in radians!
};

// ---------------------------------------------------------------------------
#endif