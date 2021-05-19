#pragma once
// ----------------------------------------------------------------------------
// File: Common.h
// Project: CS 200 Framework
// Purpose: Declares common functionality accross projects relating to 
//			game state changes and rasterizer configurations. 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

typedef void (*MenuFn)();

// COMMON
void KeyboardInput();
void RegisterGameState(const char* stateName, int stateID);
bool ShowFrameworkMenu(MenuFn demoMenu = nullptr);
