#pragma once
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"shader.h"

using namespace DirectX;
// í∏ì_ç\ë¢ëÃ
struct Vertex2nd {
	XMFLOAT3	Pos;
};

bool InitTrinagle2nd();
void DrawTriangle2nd();
void UninitTriangle2nd();
void 	VertexUpdate(XMFLOAT3* _DeltaPos);

Vertex2nd * GetVertex2nd();
