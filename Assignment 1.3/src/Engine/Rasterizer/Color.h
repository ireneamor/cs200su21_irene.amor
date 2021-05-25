#ifndef CS200_COLOR_H_
#define CS200_COLOR_H_

namespace Rasterizer
{
#pragma warning (disable:4201) // nameless struct warning
	struct Color
	{
		union
		{
			struct
			{
				f32 r, g, b, a;
			};
			f32 v[4];
		};

		Color();
		Color(f32 rr, f32 gg, f32 bb, f32 aa = 1.0f);
		
		Color	operator *(const float & sc) const;
		Color&	operator *=(const float & sc);
		Color	operator +(const Color& rhs) const;
		Color&	operator += (const Color& rhs);
		Color	operator -(const Color& rhs) const;
		Color& operator -= (const Color& rhs);
		Color operator-()const;
		Color	operator *(const Color& rhs) const;
		Color&	operator *= (const Color& rhs);
		Color& FromU32(u32 colorU32);
		u32    ToU32()const;
	};
#pragma warning (default:4201) // nameless struct warning
}

#endif