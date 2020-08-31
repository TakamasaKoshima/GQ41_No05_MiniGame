//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
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
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------


//==============================================================================
//!	@fn		DrawFPS
//!	@brief	FPS�\��
//!	@param	�E�C���h�E�n���h���l
//!	@param	FPS�l
//!	@retval	�Ȃ�
//==============================================================================
void DrawFPS(HWND hwnd, int dwFrameCount) {

	HDC hdc;									// �f�o�C�X�R���e�L�X�g
	char str[128];								// ������

	hdc = GetDC(hwnd);							// �f�o�C�X�R���e�L�X�g�擾

	sprintf_s<128>(str, "FPS : %d", dwFrameCount);
	int len = static_cast<int>(strlen(str));

	TextOut(hdc, 0, 0, str, len);

	ReleaseDC(hwnd, hdc);						// �f�o�C�X�R���e�L�X�g���
}

//==============================================================================
//!	@fn		GameInit
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{

	// DirectX11 �O���t�B�b�N�X������
	bool sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(nullptr, "CDirectXGraphics Init�G���[", "error!!", MB_OK);
		return false;
	}

	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);
	// 3�p�`�̏���������
	sts = InitTrinagle();
	if (!sts) {
		MessageBox(nullptr, "Triangle Init�G���[", "error!!", MB_OK);
		return false;
	}
	sts = InitTrinagle2nd();
	if (!sts) {
		MessageBox(nullptr, "Triangle Init�G���[", "error!!", MB_OK);
		return false;
	}

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput() {
	CDirectInput::GetInstance().GetKeyBuffer();			// �L�[���͎擾
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate() {
	//�@���݂̃t���[�����W�̈ړ���
	XMFLOAT3		DeltaPos = { 0,0,0 };

	//�@�L�[���͏���
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
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender() {

	// �`��O����

	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* rtv;
	rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();

	// �^�[�Q�b�g�o�b�t�@�N���A
	devcontext->ClearRenderTargetView(
		rtv,			// �����_�[�^�[�Q�b�g�r���[
		ClearColor);	// �N���A����J���[�l

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	devcontext->ClearDepthStencilView(
		dstv,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����


	// �R�p�`�`��

	DrawTriangle2nd();
	DrawTriangle();


	// �`��㏈��
	IDXGISwapChain* swapchain;
	swapchain = CDirectXGraphics::GetInstance()->GetSwapChain();
	swapchain->Present(
		0,		// �t���[���̕\���𐂒������̋󔒂Ɠ�����������@���w�肷�鐮���B
		0);		// �X���b�v�`�F�[���\���I�v�V�������܂ސ����l�B 
				// �����̃I�v�V������DXGI_PRESENT�萔�Œ�`����Ă��܂��B

}

//==============================================================================
//!	@fn		GameMain
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameMain()
{
	GameInput();					// ����
	GameUpdate();					// �X�V
	GameRender();					// �`��
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameExit()
{
	// DirectX11 �O���t�B�b�N�X�I������
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
