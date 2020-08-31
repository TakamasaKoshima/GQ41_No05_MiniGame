#ifndef _TRIANGLE_CLASS_H_
#define _TRIANGLE_CLASS_H_

#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"shader.h"

using namespace DirectX;
// ���_�\����
struct Vertex {
	XMFLOAT3	Pos;
};

class TriangleClass
{
public:

	ID3D11Buffer*			m_VertexBuffer = nullptr;	// ���_�o�b�t�@
	ID3D11VertexShader*		m_VertexShader = nullptr;	// ���_�V�F�[�_�[
	ID3D11PixelShader*		m_PixelShader = nullptr;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*		m_VertexLayout = nullptr;	// ���_���C�A�E�g


	bool InitTrinagle();
	void DrawTriangle();
	void UninitTriangle();

	TriangleClass();
	~TriangleClass();

private:
};


#endif // !_TRIANGLE_CLASS_H_


