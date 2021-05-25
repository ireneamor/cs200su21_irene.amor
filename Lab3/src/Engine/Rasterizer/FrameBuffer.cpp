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
    u8* FrameBuffer::frameBuffer = NULL;
    u32 FrameBuffer::frameBufferWidth = 0;
    u32 FrameBuffer::frameBufferHeight = 0;

    // ---------------------------------------------------------------------------
   // \fn Allocate
   // \brief Allocate memory for the frame buffer given by the width and height.
    bool FrameBuffer::Allocate(u32 width, u32 height)
    {
        //Sanity check: free allocated memory
        Delete();

        //Sanity check: width and height are valid
        if (width == 0 || height == 0)
            return false;

        //Allocate memory for array
        frameBuffer = new u8[width * height * COLOR_COMP];
        frameBufferWidth = width;
        frameBufferHeight = height;

        //Return true if the memory can be allocated
        return frameBuffer != nullptr;
    }
    // ---------------------------------------------------------------------------
    // \fn Delete
    // \brief Free the memory allocated in the function above.
    void FrameBuffer::Delete()
    {
        if (frameBuffer)
        {
            delete[] frameBuffer;
            frameBuffer = 0;
        }
    }

    // ---------------------------------------------------------------------------
   // \fn Present
   // \brief Draws the contents of the frame buffer to the screen using Alpha
   // Engine.
    void FrameBuffer::Present()
    {
        //Sanity check
        if (frameBuffer && frameBufferWidth && frameBufferHeight)
        {
            //Create a quad to draw
            AEGfxTriStart();

            AEGfxTriAdd(
                -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f, //TL
                -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f, //BL
                0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f //BR
            );
            AEGfxTriAdd(
                -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f, //TL
                0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f, //BL
                0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f //BR
            );

            AEGfxTriList* quad = AEGfxTriEnd();

            //Crreate a texture from the frame buffer
            AEGfxTexture* tex = AEGfxTextureLoad(frameBufferWidth, frameBufferHeight, frameBuffer);

            //Draw the quad with the size of the window
            AEMtx33 mtx = AEMtx33::Scale((f32)gAESysWinWidth, (f32)gAESysWinHeight);
            AEGfxSetTransform(&mtx);
            AEGfxTextureSet(tex);
            AEGfxTriDraw(quad);

            //Free the quad
            AEGfxTriFree(quad);
            AEGfxTextureUnload(tex);
        }
    }

    // ---------------------------------------------------------------------------
   // \fn GetBufferData
   // \brief Returns the pointer to the frame buffer variable
    u8* FrameBuffer::GetBufferData()
    {
        return frameBuffer;
    }

    // ---------------------------------------------------------------------------
   // \fn GetWidth
   // \brief Returns the width of the frame buffer.
    u32 FrameBuffer::GetWidth()
    {
        return frameBufferWidth;
    }

    // ---------------------------------------------------------------------------
   // \fn GetHeight
   // \brief Returns the height of the frame buffer.
    u32 FrameBuffer::GetHeight()
    {
        return frameBufferHeight;
    }

    // ---------------------------------------------------------------------------
   // \fn Clear
   // \brief Sets the entire frame buffer to the provided color.
    void FrameBuffer::Clear(const Color& c)
    {
        Clear((u8)(c.r * 255.0f), (u8)(c.g * 255.0f), (u8)(c.b * 255.0f), (u8)(c.a * 255.0f));
    }

    // ---------------------------------------------------------------------------
   // \fn Clear
   // \brief Sets the entire frame buffer to the provided color in rgb format
    void FrameBuffer::Clear(u8 r, u8 g, u8 b, u8 a)
    {
        u32 lim = frameBufferWidth * frameBufferHeight * COLOR_COMP;

        for (u32 i = 0; i < lim; i += 4)
        {
            frameBuffer[i + 0] = r;
            frameBuffer[i + 1] = g;
            frameBuffer[i + 2] = b;
            frameBuffer[i + 3] = a;
        }
    }

    // ---------------------------------------------------------------------------
   // \fn SetPixel
   // \brief Sets the pixel at position x, y to the provided color.
    void FrameBuffer::SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
    {
        //Sanity check: x and y are within frame buffer dimension
        if (x >= frameBufferWidth || y >= frameBufferHeight)
            return;

        //Compute index of pixel's first component
        u32 idx = (x + y * frameBufferWidth) * COLOR_COMP;

        //Copy
        frameBuffer[idx + 0] = r;
        frameBuffer[idx + 1] = g;
        frameBuffer[idx + 2] = b;
        frameBuffer[idx + 3] = a;
    }

    // ---------------------------------------------------------------------------
   // \fn SetPixel
   // \brief Sets the pixel at position x, y to the provided color.
    void FrameBuffer::SetPixel(u32 x, u32 y, const Color& c)
    {
        SetPixel(x, y, (u8)(c.r * 255.0f), (u8)(c.g * 255.0f), (u8)(c.b * 255.0f), (u8)(c.a * 255.0f));
    }

    // ---------------------------------------------------------------------------
   // \fn GetPixel
   // \brief Returns the color of the pixel at position x, y.
    Color FrameBuffer::GetPixel(u32 x, u32 y)
    {
        //Sanity check: x and y are within frame buffer dimension
        if (x >= frameBufferWidth || y >= frameBufferHeight)
            return Color();

        //Compute index of pixel's first component
        u32 idx = (x + y * frameBufferWidth) * COLOR_COMP;

        //Copy
        Color c;
        c.r = (f32)frameBuffer[idx + 0] / 255.0f;
        c.g = (f32)frameBuffer[idx + 1] / 255.0f;
        c.b = (f32)frameBuffer[idx + 2] / 255.0f;
        c.a = (f32)frameBuffer[idx + 3] / 255.0f;

        //Return color
        return c;
    }

    // ---------------------------------------------------------------------------
   // \fn SaveToFile
   // \brief Saves the frame buffer to a binary file.
    void FrameBuffer::SaveToFile(const char* filename)
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
   // \fn LoadFromFile
   // \brief Load the frame buffer from a binary file.
    void FrameBuffer::LoadFromFile(const char* filename)
    {
        // Sanity check
        if (!filename)
            return;

        // try to open the file
        std::fstream fp(filename, std::ios::in | std::ios::binary);

        if (fp.is_open() && fp.good())
        {
            u32 fbWidth, fbHeight;
            fp.read(reinterpret_cast<char*>(&fbWidth), sizeof(u32));
            fp.read(reinterpret_cast<char*>(&fbHeight), sizeof(u32));

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
            fp.read(reinterpret_cast<char*>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

            // close the file
            fp.close();
        }
    }

    // ---------------------------------------------------------------------------
   // \fn SaveToImageFile
   // \brief Save the frame buffer to image file.
    void FrameBuffer::SaveToImageFile(const char* filename)
    {
        AEGfxSaveImagePNG(filename, frameBuffer, frameBufferWidth, frameBufferHeight);
    }

#pragma region// Extra Challenges

    // ---------------------------------------------------------------------------
   // \fn ClearCheckerboard
   // \brief Clear frame buffer to a checkerboard pattern.
    void FrameBuffer::ClearCheckerboard(u32 colors[2], u32 size) {}

    // ---------------------------------------------------------------------------
   // \fn LoadFromImageFile
   // \brief Loads an image from file and sets it to the framebuffer
    void FrameBuffer::LoadFromImageFile(const char* filename) {}

    // ---------------------------------------------------------------------------
   // \fn CheckerboardImage
   // \brief Loads an image from file and sets it to the framebuffer in a checker
   // board pattern. One square should be the original image, the other
   // should be the inverted color of the image (see docs for details).
    void FrameBuffer::CheckerboardImage(const char* filename, u32 size) {}

#pragma endregion
}