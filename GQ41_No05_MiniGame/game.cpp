//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <DirectXMath.h>
#include <stdio.h>
#include "CDirectxGraphics.h"
#include "CDirectInput.h"
#include "triangle.h"
#include "triangle2nd.h"
using namespace DirectX;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------


//==============================================================================
//!	@fn		DrawFPS
//!	@brief	FPS表示
//!	@param	ウインドウハンドル値
//!	@param	FPS値
//!	@retval	なし
//==============================================================================
void DrawFPS(HWND hwnd, int dwFrameCount) {

	HDC hdc;									// デバイスコンテキスト
	char str[128];								// 文字列

	hdc = GetDC(hwnd);							// デバイスコンテキスト取得

	sprintf_s<128>(str, "FPS : %d", dwFrameCount);
	int len = static_cast<int>(strlen(str));

	TextOut(hdc, 0, 0, str, len);

	ReleaseDC(hwnd, hdc);						// デバイスコンテキスト解放
}

//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{

	// DirectX11 グラフィックス初期化
	bool sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(nullptr, "CDirectXGraphics Initエラー", "error!!", MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);
	// 3角形の初期化処理
	sts = InitTrinagle();
	if (!sts) {
		MessageBox(nullptr, "Triangle Initエラー", "error!!", MB_OK);
		return false;
	}
	sts = InitTrinagle2nd();
	if (!sts) {
		MessageBox(nullptr, "Triangle Initエラー", "error!!", MB_OK);
		return false;
	}

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameInput() {
	CDirectInput::GetInstance().GetKeyBuffer();			// キー入力取得
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameUpdate() {
	//　現在のフレーム座標の移動量
	XMFLOAT3		DeltaPos = { 0,0,0 };

	//　キー入力処理
	static bool keyinput = true;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
		DeltaPos.y += 0.02f;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
		DeltaPos.y -= 0.02f;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
		DeltaPos.x -= 0.02f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
		DeltaPos.x += 0.02f;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		keyinput = true;
	}
	VertexUpdate(&DeltaPos);
	TriangleUpdate();
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameRender() {

	// 描画前処理

	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* rtv;
	rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();

	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(
		rtv,			// レンダーターゲットビュー
		ClearColor);	// クリアするカラー値

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	// Zバッファ、ステンシルバッファクリア
	devcontext->ClearDepthStencilView(
		dstv,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする


	// ３角形描画

	DrawTriangle2nd();
	DrawTriangle();


	// 描画後処理
	IDXGISwapChain* swapchain;
	swapchain = CDirectXGraphics::GetInstance()->GetSwapChain();
	swapchain->Present(
		0,		// フレームの表示を垂直方向の空白と同期させる方法を指定する整数。
		0);		// スワップチェーン表示オプションを含む整数値。 
				// これらのオプションはDXGI_PRESENT定数で定義されています。

}

//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameMain()
{
	GameInput();					// 入力
	GameUpdate();					// 更新
	GameRender();					// 描画
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameExit()
{
	// DirectX11 グラフィックス終了処理
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
