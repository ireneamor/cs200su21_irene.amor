#include <AEEngine.h> // f32, u32, etc...
#include "FrameBuffer.h"
#include "Color.h"

// file io
#include <fstream>
#include <sstream>

#define COLOR_COMP 4

namespace Rasterizer
{
	// ------------------------------------------------------------------------
	// Our framebuffer
	u8 *	FrameBuffer::frameBuffer = NULL;
	u32		FrameBuffer::frameBufferWidth = 0;
	u32		FrameBuffer::frameBufferHeight = 0;

	// ---------------------------------------------------------------------------
	// \fn		Allocate
	// \brief	Allocate memory for the frame buffer given by the width and height. 
	bool FrameBuffer::Allocate(u32 width, u32 height)
	{
		// free any data
		Delete();

		frameBufferWidth = width;
		frameBufferHeight = height;
		frameBuffer = new u8[width * height * COLOR_COMP];
		if (frameBuffer)
		{
			Clear(0, 0, 0);
			return true;
		}
		return false;
	}
	// ---------------------------------------------------------------------------
	// \fn		Delete
	// \brief	Free the memory allocated in the function above. 
	void FrameBuffer::Delete()
	{
		// delete the data

		if (frameBuffer)
			delete[] frameBuffer;
	}

	// ---------------------------------------------------------------------------
	// \fn		Present
	// \brief	Draws the contents of the frame buffer to the screen using Alpha
	//			Engine.
	void FrameBuffer::Present()
	{
		if (frameBuffer && frameBufferWidth != 0 && frameBufferHeight != 0) {
			auto tex = AEGfxTextureLoad(frameBufferWidth, frameBufferHeight, frameBuffer);
			AEGfxTriStart();
			AEGfxTriAdd(
				-0.5f, 0.5f, AE_COLORS_WHITE, 0, 1,
				-0.5f, -0.5f, AE_COLORS_WHITE, 0, 0,
				0.5f, -0.5f, AE_COLORS_WHITE, 1, 0);
			AEGfxTriAdd(
				-0.5f, 0.5f, AE_COLORS_WHITE, 0, 1,
				0.5f, -0.5f, AE_COLORS_WHITE, 1, 0,
				0.5f, 0.5f, AE_COLORS_WHITE, 1, 1);
			auto quad = AEGfxTriEnd();

			AEGfxTextureSet(tex);
			AEMtx33 mtx = AEMtx33::Scale((f32)gAESysWinWidth, (f32)gAESysWinHeight);
			AEGfxSetTransform(&mtx);
			AEGfxTriDraw(quad);

			AEGfxTextureUnload(tex);
			AEGfxTriFree(quad);
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		GetBufferData
	// \brief	Returns the pointer to the frame buffer variable
	u8 *	FrameBuffer::GetBufferData()
	{
		return frameBuffer;
	}

	// ---------------------------------------------------------------------------
	// \fn		GetWidth
	// \brief	Returns the width of the frame buffer.
	u32		FrameBuffer::GetWidth()
	{
		return frameBufferWidth;
	}

	// ---------------------------------------------------------------------------
	// \fn		GetHeight
	// \brief	Returns the height of the frame buffer.
	u32		FrameBuffer::GetHeight()
	{
		return frameBufferHeight;
	}

	// ---------------------------------------------------------------------------
	// \fn		Clear
	// \brief	Sets the entire frame buffer to the provided color.
	void FrameBuffer::Clear(const Color & c)
	{
		Clear(u8(c.r * 255.0f),
			u8(c.g * 255.0f),
			u8(c.b * 255.0f),
			u8(c.a * 255.0f));
	}

	// ---------------------------------------------------------------------------
	// \fn		Clear
	// \brief	Sets the entire frame buffer to the provided color in rgb format
	void FrameBuffer::Clear(u8 r, u8 g, u8 b, u8 a)
	{
		if (frameBuffer)
		{
			u32 totalSize = frameBufferWidth * frameBufferHeight * COLOR_COMP;
			for (u32 i = 0; i < totalSize; i += COLOR_COMP)
			{
				frameBuffer[i] = r;
				frameBuffer[i + 1] = g;
				frameBuffer[i + 2] = b;
				frameBuffer[i + 3] = a;
			}
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		SetPixel
	// \brief	Sets the pixel at position x, y to the provided color. 
	void FrameBuffer::SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
	{
		// Sanity check
		if (NULL == frameBuffer || x >= frameBufferWidth || y >= frameBufferHeight)
			return;

		// advance to pixel
		u32 startOffset = COLOR_COMP * (y * frameBufferWidth + x);

		// set
		frameBuffer[startOffset] = r;
		frameBuffer[startOffset + 1] = g;
		frameBuffer[startOffset + 2] = b;
		frameBuffer[startOffset + 3] = a;
	}

	// ---------------------------------------------------------------------------
	// \fn		SetPixel
	// \brief	Sets the pixel at position x, y to the provided color. 
	void FrameBuffer::SetPixel(u32 x, u32 y, const Color& c)
	{
		SetPixel(x, y,
			u8(c.r * 255.0f),
			u8(c.g * 255.0f),
			u8(c.b * 255.0f),
			u8(c.a * 255.0f));
	}

	// ---------------------------------------------------------------------------
	// \fn		GetPixel
	// \brief	Returns the color of the pixel at position x, y.
	Color FrameBuffer::GetPixel(u32 x, u32 y)
	{
		// Sanity check
		if (NULL == frameBuffer || x >= frameBufferWidth || y >= frameBufferHeight)
			return Color();

		// advance to pixel
		u32 startOffset = COLOR_COMP * (y * frameBufferWidth + x);

		// Get the color component
		u8 r = frameBuffer[startOffset];
		u8 g = frameBuffer[startOffset + 1];
		u8 b = frameBuffer[startOffset + 2];
		u8 a = frameBuffer[startOffset + 3];

		// Convert to color class
		return Color((f32)r*255.0f, (f32)g*255.0f, (f32)b*255.0f, (f32)a*255.0f);
	}

	// ---------------------------------------------------------------------------
	// \fn		SaveToFile
	// \brief	Saves the frame buffer to a binary file. 
	void FrameBuffer::SaveToFile(const char *filename)
	{
		// Sanity Check
		if (!filename)
			return;

		// try to open the file
		std::fstream fp(filename, std::ios::out | std::ios::binary);

		if (fp.is_open() && fp.good())
		{
			// write header information - just width and height
			fp.write(reinterpret_cast<const char*>(&frameBufferWidth), sizeof(u32));
			fp.write(reinterpret_cast<const char*>(&frameBufferHeight), sizeof(u32));

			// write pixel data
			fp.write(reinterpret_cast<const char*>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

			// close the file
			fp.close();
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		LoadFromFile
	// \brief	Load the frame buffer from a binary file. 
	void FrameBuffer::LoadFromFile(const char * filename)
	{
		// Sanity check
		if (!filename)
			return;

		// try to open the file
		std::fstream fp(filename, std::ios::in | std::ios::binary);

		if (fp.is_open() && fp.good())
		{
			u32 fbWidth, fbHeight;
			fp.read(reinterpret_cast<char *>(&fbWidth), sizeof(u32));
			fp.read(reinterpret_cast<char *>(&fbHeight), sizeof(u32));

			// re-allocate the data if necessary
			if (NULL != frameBuffer && (fbWidth * fbHeight) != (frameBufferWidth * frameBufferHeight))
			{
				delete[] frameBuffer;
				frameBuffer = new u8[fbWidth * fbHeight * COLOR_COMP];
			}
			// no frame buffer, allocate one
			else if (NULL == frameBuffer)
			{
				frameBuffer = new u8[fbWidth * fbHeight * COLOR_COMP];
			}

			// store new width and new height
			frameBufferWidth = fbWidth;
			frameBufferHeight = fbHeight;

			// now read the framebuffer data
			fp.read(reinterpret_cast<char *>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

			// close the file
			fp.close();
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		SaveToImageFile
	// \brief	Save the frame buffer to image file. 
	void FrameBuffer::SaveToImageFile(const char * filename)
	{
		AEGfxSaveImagePNG(filename, frameBuffer, frameBufferWidth, frameBufferHeight);
	}

	// Extra Challenges
	void FrameBuffer::ClearCheckerboard(u32 colors[2], u32 size)
	{
		u32 c = 0;
		u32 s = 0;
		for (u32 i = 0; i < frameBufferHeight; ++i) {
			if (i % size == 0)
				s = ++s % 2;
			c = s;
			for (u32 j = 0; j < frameBufferWidth; ++j)
			{
				if (j % size == 0)
					c = ++c % 2;
				u32 p[4];
				AEGfxColorComp(colors[c], p, p + 1, p + 2, p + 3);
				for (u32 k = 0; k < 4; ++k)
					frameBuffer[(i * frameBufferWidth + j) * 4 + k] = (u8)p[k];
			}
		}
	}

	void FrameBuffer::LoadFromImageFile(const char* filename) {
		// load the file using the alpha engine
		u8* imgPixels = 0;
		u32 imgWidth = 0, imgHeight = 0;
		if (AEGfxLoadImagePNG(filename, imgPixels, imgWidth, imgHeight))
		{
			auto minW = min(imgWidth, frameBufferWidth);
			auto minH = min(imgHeight, frameBufferHeight);

			// center image if not bigger than frame buffer
			auto sX =  minW == frameBufferWidth ? 0 : (frameBufferWidth - imgWidth) / 2;
			auto sY =  minH == frameBufferHeight ? 0 : (frameBufferHeight - imgHeight) / 2;

			// copy data
			for (u32 i = 0; i < minH; ++i) {
				for (u32 j = 0; j < minW; ++j) {
					for (u32 k = 0; k < 4; ++k) {
						u32 fbIdx= ((sY+i) * frameBufferWidth + (sX+j)) * 4 + k;
						u32 imgIdx = (i * imgWidth + j) * 4 + k;
						frameBuffer[fbIdx] = imgPixels[imgIdx];
					}
				}
			}

			// cleanup
			delete[] imgPixels;
		}
	}


	/*
		\details: This is a tricky one. Why? Because of the image size. 
		here the user is passing us what is supposed to be the cell size as 
		a parameter. 

		This wasn't a problem in the first checker board function, because
		we were setting each "square" in the checker board with one single color. 

		In this function, the user expects the image to be SCALED to the size 
		of the square. But we don't know how to scale an image. Copying the data
		is one thing, modifying it is another. 

		As a first step, we are going to copy the texture data into the size specified. 
		If the image is bigger then we simply copy everything (like in the previous function). 
		If the image is smaller, then we can do the same centering logic as above. 
		The result would be a small buffer that we copy into the framebuffer.

		After this works, then we can tackle the scaling down of the image to fit the size of 
		the rectangle. The simplest thing is to take the average of a group of pixels in the image
		and use it as the color of 1 pixel in the cell. 
	*/
	void FrameBuffer::CheckerboardImage(const char* filename, u32 size, FrameBuffer::pixelShader shader)
	{
		// load the file using the alpha engine
		u8* imgPixels = 0;
		u32 imgWidth = 0, imgHeight = 0;
		if (AEGfxLoadImagePNG(filename, imgPixels, imgWidth, imgHeight))
		{
			
			u8* cellData1 = new u8[size * size * 4];
			u8* cellData2 = new u8[size * size * 4];
			u8* cellSrc[2] = { cellData1, cellData2 };
			
			// we are going to 
			auto minW = min(imgWidth, size);
			auto minH = min(imgHeight, size);

			// center image if not bigger than frame buffer
			auto sX = minW == size ? 0 : (size - imgWidth) / 2;
			auto sY = minH == size ? 0 : (size - imgHeight) / 2;

			// copy data into cell size
			for (u32 i = 0; i < minH; ++i) {
				for (u32 j = 0; j < minW; ++j) {
					u32 sqIdx = ((sY + i) * size + (sX + j)) * 4;
					u32 imgIdx = (i * imgWidth + j) * 4;


					// square 1 and 2: copy original color
					for (u32 k = 0; k < 4; ++k) {
						cellData1[sqIdx + k] = imgPixels[imgIdx + k];
						cellData2[sqIdx + k] = imgPixels[imgIdx + k];
					}

					// square 2: call shader execpt alpha
					u8* pix = cellData2 + sqIdx;
					shader(imgIdx, pix, pix + 1, pix + 2, pix + 3);

				}
			}

			u32 c = 0;
			u32 s = 0;
			u32 sqRowSize = size * 4;
			u32 sqRowSize2 = size;
			for (u32 i = 0; i < frameBufferHeight; ++i) {
				c = s;
				u32 sqIdx = (i%size) * size * 4;
				u32 sqIdx2 = (i * size);
				for (u32 j = 0; j < frameBufferWidth; j+=size)
				{

					u32 fbIdx2 = (i * frameBufferWidth + j);
					u32 fbRowSize2 = (frameBufferWidth - j);

					u32 fbIdx = (i * frameBufferWidth + j) * 4;
					u32 fbRowSize = (frameBufferWidth -j) * 4;
					u32 minSize = min(fbRowSize, sqRowSize);

					// copy square data
					//memcpy_s(frameBuffer + fbIdx, fbRowSize, cellSrc[c] + sqIdx, minSize);
					for (u32 k = 0; k < minSize; ++k) {
						frameBuffer[fbIdx + k] = cellSrc[c][sqIdx + k];
					}
					

					c = ++c % 2;
				}
				if (i % size == 0)
					s = ++s % 2;
			}


			// cleanup
			delete[] imgPixels;
		}
	}

	void FrameBuffer::invert_pixel_color(u32 p, u8* r, u8* g, u8* b, u8* a)
	{
		*r = 255 - *r;
		*g = 255 - *g;
		*b = 255 - *b;
		*a = *a;
	}
	void FrameBuffer::add_gradient(u32 p, u8* r, u8* g, u8* b, u8* a)
	{
		u32 size = 100;
		u32 x = (p/4) % size;
		f32 tn = 1.0f-f32(x) / f32(size);
		Color c; c.FromU32(AE_COLORS_FRENCH_BEIGE);
		Color pc(*r, *g, *b, *a);
		c = c * tn * pc;

		*r = min(255, u8(c.r * 255));
		*g = min(255, u8(c.g * 255));
		*b = min(255, u8(c.b * 255));
		*a = min(255, u8(c.a * 255));
	}

	void FrameBuffer::do_nothing(u32 p, u8* r, u8* g, u8* b, u8* a)
	{

	}

}
