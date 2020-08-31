#ifndef _TRIANGLE_CLASS_H_
#define _TRIANGLE_CLASS_H_

#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"shader.h"

using namespace DirectX;
// 頂点構造体
struct Vertex {
	XMFLOAT3	Pos;
};

class TriangleClass
{
public:

	ID3D11Buffer*			m_VertexBuffer = nullptr;	// 頂点バッファ
	ID3D11VertexShader*		m_VertexShader = nullptr;	// 頂点シェーダー
	ID3D11PixelShader*		m_PixelShader = nullptr;	// ピクセルシェーダー
	ID3D11InputLayout*		m_VertexLayout = nullptr;	// 頂点レイアウト


	bool InitTrinagle();
	void DrawTriangle();
	void UninitTriangle();

	TriangleClass();
	~TriangleClass();

private:
};


#endif // !_TRIANGLE_CLASS_H_


