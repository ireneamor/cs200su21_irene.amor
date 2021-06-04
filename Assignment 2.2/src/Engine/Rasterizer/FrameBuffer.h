#ifndef CS200_FRAME_BUFFER_H_
#define CS200_FRAME_BUFFER_H_

namespace Rasterizer
{
	struct Color; // forward declare the color structure

	class FrameBuffer
	{
	public:
		// Initialize
		static bool Allocate(u32 width, u32 height);
		static void Delete();

		// Getters
		static u8 *		GetBufferData();
		static u32		GetWidth();
		static u32		GetHeight();

		// FrameBuffer Operations
		static void Clear(const Color & c);
		static void Clear(u8 r, u8 g, u8 b, u8 a = 255);
		static void SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a = 255);
		static void SetPixel(u32 x, u32 y, const Color & c);
		static Color GetPixel(u32 x, u32 y);
		static void Present();

		// Debug
		static void SaveToFile(const char *filename);
		static void LoadFromFile(const char * filename);

		// Debug
		static void SaveToImageFile(const char * filename);

		// Extra Challenges
		static void ClearCheckerboard(u32 Colors[2], u32 size);
		static void LoadFromImageFile(const char* filename);

		// VERY BIG MEGA CHALLENGE
		typedef void (*pixelShader)(u32 p, u8* r, u8* g, u8* b, u8* a);
		static void invert_pixel_color(u32 p, u8* r, u8* g, u8* b, u8* a);
		static void add_gradient(u32 p, u8* r, u8* g, u8* b, u8* a);
		static void do_nothing(u32 p, u8*r, u8* g, u8* b, u8* a);
		static void CheckerboardImage(const char* filename, u32 size, pixelShader fn = do_nothing);

		// Private Variables
	private:
		static u8 *		frameBuffer;
		static u32		frameBufferWidth;
		static u32		frameBufferHeight;
	};
}

#endif