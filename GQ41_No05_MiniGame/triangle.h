#pragma once
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"shader.h"
using namespace DirectX;

bool InitTrinagle();
void DrawTriangle();
void UninitTriangle();
// í∏ì_ç\ë¢ëÃ
 struct Vertex {
	XMFLOAT3	Pos;
};

Vertex * GetVertex();

void TriangleUpdate();

void CollisionCheck(bool _bool);

bool Collision(XMFLOAT3 * _OtherVertex);

void CalCulateVector(XMFLOAT3 * _OtherVertex);

void CalCulateTangent(XMFLOAT3 * _OtherVertex);
