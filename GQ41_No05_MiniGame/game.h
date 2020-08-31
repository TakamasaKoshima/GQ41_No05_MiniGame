//*****************************************************************************
//!	@file	game.h
//!	@brief	ゲーム処理
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

#define		SCREEN_X		800
#define		SCREEN_Y		600
#define		FULLSCREEN      0

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
void GameMain();
void GameExit();
void GameInput();
void GameUpdate();
void GameRender();

void DrawFPS(HWND hwnd, int dwFrameCount);

//******************************************************************************
//	End of file.
//******************************************************************************
