// ----------------------------------------------------------------------------
// File: Level_1.h
// Project: CS200 Assignment 1
// Purpose: 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

#ifndef LEVEL_1_H_
#define LEVEL_1_H_


#define GS_LEVEL_1 1


void Level_1_Load();
void Level_1_Init();
void Level_1_Update();
void Level_1_Render();
void Level_1_Free();
void Level_1_Unload();

// COMMON
void SaveFrameBuffer();
void KeyboardInput();

// ----------------------------------------------------------------------------
#endif
