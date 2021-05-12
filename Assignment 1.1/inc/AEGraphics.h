// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEGraphics.h
// Author			:	Thomas Komair
// Last Update		:	Jan 2020
// Purpose			:	Graphics component header file
// ---------------------------------------------------------------------------

#ifndef AE_GRAPHICS_H
#define AE_GRAPHICS_H

#define AEGFX_NAME_SIZE 512


// forward declare AEGfx structures
struct AEGfxTriList;
struct AEGfxTexture;
struct AEGfxFont;


// ---------------------------------------------------------------------------
// Defines

// maximum number of triangles per mesh
#define AE_GFX_TRI_NUM_MAX	8192

enum AEGfxBlendMode
{
	AE_GFX_BM_NONE = 0,			// Replaces the frame buffer color with the triangle pixel
	AE_GFX_BM_BLEND,			// Blends the fb color with the tri color using the alpha of the tri: FinalColor = TriColor * TriAlpha + FBColor * (1-TriAlpha)
	AE_GFX_BM_ADD,				// Adds the two colors together : FinalColor = TriColor + FBColor
	AE_GFX_BM_ADD_BLENDALPHA,	// Adds the two colors together and Multiplies the triangle color with its alpha: FinalColor = TriColor * TriAlpha + FBColor
	AE_GFX_BM_MULTIPLY,			// Multiplies the two colors together: FinalColor = TriColor * FBColor
	AE_GFX_BM_NUM				// keeps count of how many blend modes are implemented - also the value returned by AEGfxGetBlendMode if the arguments are set manually
};

// use those in order to have better control over the blending operation
enum AEGfxBlendArgument
{
	AE_GFX_BA_ONE = 0,		// 1
	AE_GFX_BA_ZERO,			// 0
	AE_GFX_BA_SRCCOLOR,		// RGB Color of the triangle pixel
	AE_GFX_BA_SRCALPHA,		// Alpha of the triangle pixel
	AE_GFX_BA_INVSRCALPHA,	// 1-Alpha of the triangle pixel
	AE_GFX_BA_DESTCOLOR,	// RGB color of the pixel currently in the frame buffer
	AE_GFX_BA_DESTALPHA,	// Alpha of the pixel currently in the frame buffer
	AE_GFX_BA_INVDESTALPHA,	// 1-Alpha of the pixel currently in the frame buffer
	AE_GFX_BA_NUM
};

// Texture filters define the behavior of sampling a texture when the size on screen 
// of the object to draw is greater or smaller than the texture size. 
enum AEGfxTextureFilter
{
	AE_GFX_TF_NEAREST = 0,	// uses the nearest pixel to the specified texture coordinates
	AE_GFX_TF_LINEAR,		// uses the weighted average of the texture elements that are closest to the specified texture coordinates. 
	AE_GFX_TF_NUM
};

// ---------------------------------------------------------------------------
// Struct/Class definitions

struct AEGfxVertex
{
	f32 x, y, z;
	f32 r, g, b, a;
	f32 tu, tv;
};

struct AEGfxTriList
{
	u32   mID;
	u32	  mVtxNum;
	char mName[AEGFX_NAME_SIZE];
};

// ---------------------------------------------------------------------------
struct AEGfxTexture
{
	u32 mID;
	u32 mWidth;
	u32 mHeight;
	char mName[AEGFX_NAME_SIZE];
};

struct AEGfxFont
{
	s8				mName[AEGFX_NAME_SIZE];
	u32				mSize;
	void*			mFontData;
	AEGfxTexture*	mFontTex;
};

// ---------------------------------------------------------------------------
// Extern variables

// assuming a Window of dimensions 800 x 600
extern f32	gAEWinMinX; // -400
extern f32	gAEWinMinY; // -300
extern f32	gAEWinMaxX; // 400
extern f32	gAEWinMaxY; // 300

// ---------------------------------------------------------------------------
// System Functions

bool	AEGfxInit(HWND hWin);
void	AEGfxReset();
void	AEGfxExit();
void	AEGfxStart();
void	AEGfxEnd();
void	AEGfxPresent();

//-----------------------------------------------------------------------------
// function to set the current transformation matrix to use
void	 AEGfxSetTransform(AEMtx33* pMtx);
void	 AEGfxSetTransform(AEMtx44* pMtx);
void	 AEGfxSetTextureTransform(AEMtx33 * pTexMtx);

//-----------------------------------------------------------------------------
// functions to set the frame buffer's clear color
void	AEGfxSetClearColor(u32 clearColor);
u32		AEGfxGetClearColor();
void	AEGfxClearBuffer();
void	AEGfxClearColorBuffer();
void	AEGfxClearDepthBuffer();
void	AEGfxSetDepthBufferEnabled(bool enabled);
bool	AEGfxGetDepthBufferEnabled();
void	AEGfxSetVSyncEnabled(bool enabled);
bool	AEGfxGetVSyncEnabled();
//-----------------------------------------------------------------------------
// function to set the current modulation color and Texture
void	 AEGfxSetModulationColor(u32 modColor);
u32		 AEGfxGetModulationColor();
void	 AEGfxEnableModulationColor(bool enabled);
bool	 AEGfxGetModulationColorEnabled();

//-----------------------------------------------------------------------------
// Blending operations
void				AEGfxSetBlendMode(AEGfxBlendMode blendMode);
void				AEGfxSetSourceBlendArgument(AEGfxBlendArgument blendArg);
void				AEGfxSetDestBlendArgument(AEGfxBlendArgument blendArg);
AEGfxBlendMode		AEGfxGetBlendMode();
AEGfxBlendArgument	AEGfxGetSourceBlendArgument();
AEGfxBlendArgument	AEGfxGetDestBlendArgument();

//-----------------------------------------------------------------------------
// MESH
void			AEGfxTriStart	();
void			AEGfxTriAdd		(f32 x0, f32 y0, u32 c0, f32 tu0, f32 tv0,
								 f32 x1, f32 y1, u32 c1, f32 tu1, f32 tv1,
								 f32 x2, f32 y2, u32 c2, f32 tu2, f32 tv2);

AEGfxTriList*	AEGfxTriEnd		(const char * name = 0);
AEGfxTriList*	AEGfxTriListCreate(AEGfxVertex* pVertexData, u32 vtxCount, const char* name = 0);
void			AEGfxTriDraw	(AEGfxTriList* pTriList);
void			AEGfxTriFree(AEGfxTriList* pTriList);

//-----------------------------------------------------------------------------
// TEXTURE

AEGfxTexture*	AEGfxTextureLoad(const char *pFileName);						// from file
AEGfxTexture*	AEGfxTextureLoad(u32 width, u32 height, u8* pPixels = NULL, const char * name = NULL);	// from 2D array of pixels
void			AEGfxTextureGetPixels(AEGfxTexture* pTexture, u8** outPixels);
void			AEGfxTextureUnload(AEGfxTexture *pTexture);
void			AEGfxTextureSet(AEGfxTexture *pTexture);				// Use NULL for 'no texture'
void			AEGfxTextureSetFilters(AEGfxTexture* pTexture, AEGfxTextureFilter minFilter, AEGfxTextureFilter magFilter);
void			AEGfxTextureGetFilters(AEGfxTexture* pTexture, AEGfxTextureFilter *minFilter, AEGfxTextureFilter *magFilter);
bool			AEGfxLoadImagePNG(const char* filename, u8*& outPixels, u32& outWidth, u32& outHeight);
bool			AEGfxSaveImagePNG(const char* filename, const u8* pixelData, u32 width, u32 height);

//-----------------------------------------------------------------------------
// RENDER TEXTURE

AEGfxTexture*	AEGfxRenderTexCreate(u32 width, u32 height, const char * name = NULL);
void			AEGfxRenderTexSet(AEGfxTexture* pRTex);	// Note: if pRTex is NULL, the the window back buffer becomes the current render target
AEGfxTexture*	AEGfxRenderTexGetCurrent();				// Note: If this function returns NULL, then the window back buffer is the current render target.


//-----------------------------------------------------------------------------
// functions to draw simple primitives
// * these are slow!!! use with caution!!
// * DO NOT call any of these function while building mesh using above functions
void			AEGfxLine		(f32 x0, f32 y0, f32 z0, u32 c0, f32 x1, f32 y1, f32 z1, u32 c1);
void			AEGfxRect		(f32 x, f32 y, f32 z, f32 w, f32 h, u32 c);
void			AEGfxRectMinMax	(f32 minX, f32 minY, f32 maxX, f32 maxY, f32 z, u32 c);
void			AEGfxRectRot	(f32 x, f32 y, f32 z, f32 w, f32 h, f32 angle_rad, u32 c);
void			AEGfxCircle		(f32 x, f32 y, f32 rad, u32 color, u32 segments = 20);
void			AEGfxFlush();

//-----------------------------------------------------------------------------
// Utility functions
// ----------------------------------------------------------------------------

u32				AEGfxColor		(u32 r, u32 g, u32 b, u32 a = 255);				// Computes a 32-bit ARGB color
void			AEGfxColorComp	(u32 color, u32 *r, u32 *g, u32 *b, u32 *a);	// Extracts the component of a 32-bit ARGB color
u32				AEGfxColInterp	(u32 c0, u32 c1, f32 t);						// Interpolates 2 color values.

// Prints text in screen coordinates (origin is top-left of client area)
void AEGfxPrint(s32 x, s32 y, u32 color, const s8* pStr);

//-----------------------------------------------------------------------------
// SIZE & VIEWPORT METHODS
void			AEGfxSetFullscreen(bool fullscreen);
bool			AEGfxGetFullscreen();
void			AEGfxSetWindowsize(u32 w, u32 h);
void			AEGfxGetScreenSize(u32*sw, u32 *sh);
void			AEGfxSetViewport(f32 left, f32 bottom, f32 right, f32 top);
void			AEGfxGetViewport(f32 *left, f32 *bottom, f32 *right, f32 *top);
//-----------------------------------------------------------------------------
// EXTRA TRANSFORM
void			AEGfxSetViewTransform(AEMtx44* pMtx);
void			AEGfxSetProjTransform(AEMtx44* pMtx);
AEMtx44			AEGfxGetViewTransform();
AEMtx44			AEGfxGetProjTransform();

//-----------------------------------------------------------------------------
// FONT SUPPORT

AEGfxFont *					AEGfxFontCreate(const char * fontName, u32 size, f32 img_size = 1.0f);
void						AEGfxFontFree(AEGfxFont * pFont);
void						AEGfxFontDraw(const s8 * pText, AEGfxFont * pFont, u32 color);
void						AEGfxFontTextRect(const s8 * pText, AEGfxFont * pFont, AEVec2* minP, AEVec2* maxP);
AEGfxTriList*				AEGfxFontMesh(AEGfxFont *pFont, const char* text);
// ---------------------------------------------------------------------------
// DEBUGGING
std::list<AEGfxTexture*>	AEGfxGetAllocatedTextures();
u32							AEGfxGetAllocatedTexturesCount();
std::list<AEGfxTexture*>	AEGfxGetAllocatedRenderTextures();
u32							AEGfxGetAllocatedRenderTexturesCount();
std::list<AEGfxTriList*>	AEGfxGetAllocatedTrilist();
u32							AEGfxGetAllocatedTrilistCount();
std::list<AEGfxFont*>		AEGfxGetAllocatedFont();
u32							AEGfxGetAllocatedFontCount();
#define						AEGfxCheckGLError() _AEGfxCheckGLError(__FILE__, __LINE__)
void						_AEGfxCheckGLError(const char* file, int line);
#endif // AE_GRAPHICS_H

