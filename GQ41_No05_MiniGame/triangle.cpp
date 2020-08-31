#include "triangle.h"
#include "triangle2nd.h"



static ID3D11Buffer*			g_VertexBuffer = nullptr;	// 頂点バッファ
static ID3D11VertexShader*		g_VertexShader = nullptr;	// 頂点シェーダー
static ID3D11PixelShader*		g_PixelShader[2] = { nullptr,nullptr };	// ピクセルシェーダー
static ID3D11InputLayout*		g_VertexLayout = nullptr;	// 頂点レイアウト
	// 頂点座標
static Vertex	g_vertex[3] = {
	XMFLOAT3(0.0f, 0.0f, 0.5f),
	XMFLOAT3(0.5f, -0.75f, 0.5f),
	XMFLOAT3(-0.5f, -0.75f, 0.5f),
};
static XMFLOAT3	g_Vector[3] = {};
static float	g_Tangent[3] = {};

// 頂点シェーダー
const char* vsfilename[] = {
	"shader/vstriangle.fx",
};
const int vsfile_num = 1;

// ピクセルシェーダー
const char* psfilename[] = {
	"shader/pstriangle.fx",
	"shader/pstriangle_Red.fx",
};
const int psfile_num = 2;

static bool g_CollisionFlag = false;
static int g_Judge = 0;

// 初期処理
bool InitTrinagle() {

	ID3D11Device*		 device;
	bool sts;
	// デバイスを取得する
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	for (int i = 0; i < vsfile_num; i++) {
		sts = CreateVertexShader(
			device,								// デバイスオブジェクト						
			vsfilename[i],					// 頂点シェーダーソースファイル					
			"main",								// エントリ関数
			"vs_5_0",							// 頂点シェーダーモデル５．０
			layout,								// 頂点データレイアウト
			numElements,						// 頂点データのエレメント数
			&g_VertexShader,					// 頂点シェーダーオブジェクト
			&g_VertexLayout);					// 頂点データレイアウトオブジェクト
		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
			return false;
		}

	}
	// ピクセルシェーダーを生成
	for (int i = 0; i < psfile_num; i++) {
		sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
			device,						// デバイスオブジェクト
			psfilename[i],			// ピクセルシェーダーソースファイル
			"main",						// エントリ関数
			"ps_5_0",					// ピクセルシェーダーモデル５．０
			&g_PixelShader[i]);			// ピクセルシェーダーオブジェクト
		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}
	}


	// 頂点バッファを生成
	sts = CreateVertexBuffer(
		device,						// デバイスオブジェクト
		sizeof(Vertex),				// １頂点当たりバイト数
		3,							// 頂点数
		g_vertex,							// 頂点データ格納メモリ先頭アドレス
		&g_VertexBuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// 描画処理
void DrawTriangle() {

	ID3D11DeviceContext*		 devicecontext;				// デバイスコンテキスト

	// デバイスコンテキストを取得する
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(Vertex);					// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;									// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devicecontext->IASetVertexBuffers(
		0,													// スタートスロット
		1,													// 頂点バッファ個数
		&g_VertexBuffer,									// 頂点バッファの先頭アドレス
		&stride,											// ストライド
		&offset);											// オフセット

	// トポロジーをセット
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点フォーマットをセット
	devicecontext->IASetInputLayout(g_VertexLayout);

	devicecontext->VSSetShader(g_VertexShader, nullptr, 0);		// 頂点シェーダーをセット


	if (g_CollisionFlag)
	{
		devicecontext->PSSetShader(g_PixelShader[1], nullptr, 0);		// ピクセルシェーダーをセット

	}
	else
	{
		devicecontext->PSSetShader(g_PixelShader[0], nullptr, 0);		// ピクセルシェーダーをセット

	}


	// デバイスコンテキストに頂点データを流し込む
	devicecontext->Draw(
		3,									// 頂点数
		0);									// 開始頂点インデックス
}

// 終了処理
void UninitTriangle() {

	if (g_VertexLayout != nullptr) {
		g_VertexLayout->Release();
		g_VertexLayout = nullptr;
	}

	if (g_VertexShader != nullptr) {
		g_VertexShader->Release();
		g_VertexShader = nullptr;
	}
	for (size_t i = 0; i < psfile_num; i++)
	{
		if (g_PixelShader[i] != nullptr) {
			g_PixelShader[i]->Release();
			g_PixelShader[i] = nullptr;
		}

	}

	if (g_VertexBuffer != nullptr) {
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

//頂点座標を渡す
Vertex* GetVertex()
{
	return g_vertex;
}
void TriangleUpdate()
{
	CollisionCheck(Collision(&GetVertex2nd()[0].Pos));
}
void CollisionCheck(bool _bool)
{
	g_CollisionFlag = _bool;
}
bool Collision(XMFLOAT3* _OtherVertex)
{
	//ベクトル作成
	CalCulateVector(_OtherVertex);

	//点と三角形のベクトルのxもしくはyが全て正もしくは全て負の時
	//当たっていない

		//ver1の部分
		//if (_OtherVertex->x < g_Vector[0].x
		//	&&_OtherVertex->x < g_Vector[1].x
		//	&&_OtherVertex->x < g_Vector[2].x)
		//{
		//	return false;
		//}
		//else if (_OtherVertex->x > g_Vector[0].x
		//	&&_OtherVertex->x > g_Vector[1].x
		//	&&_OtherVertex->x > g_Vector[2].x)
		//{
		//	return false;
		//}
		//if (_OtherVertex->y < g_Vector[0].y
		//	&&_OtherVertex->y < g_Vector[1].y
		//	&&_OtherVertex->y < g_Vector[2].y)
		//{
		//	return false;
		//}
		//else if (_OtherVertex->y > g_Vector[0].y
		//	&&_OtherVertex->y > g_Vector[1].y
		//	&&_OtherVertex->y > g_Vector[2].y)
		//{
		//	return false;
		//}


		//ver2の部分
		//if (  0.0f < g_Vector[0].x
		//	&&0.0f < g_Vector[1].x
		//	&&0.0f < g_Vector[2].x)
		//{
		//	return false;
		//}
		//else if (
		//	  0.0f > g_Vector[0].x
		//	&&0.0f > g_Vector[1].x
		//	&&0.0f > g_Vector[2].x)
		//{
		//	return false;
		//}
		//if (
		//	  0.0f < g_Vector[0].y
		//	&&0.0f < g_Vector[1].y
		//	&&0.0f < g_Vector[2].y)
		//{
		//	return false;
		//}
		//else if (
		//	  0.0f > g_Vector[0].y
		//	&&0.0f > g_Vector[1].y
		//	&&0.0f > g_Vector[2].y)
		//{
		//	return false;
		//}


		//タンジェント作成
	CalCulateTangent(_OtherVertex);

	int target, min, max;
	switch (g_Judge)
	{
	case 1:
		for (size_t i = 0; i < 3; i++)
		{
			if (g_Vector[i].x <= 0.0f)
			{
				target = i;
			}
		}

		break;

	case -1:
		for (size_t i = 0; i < 3; i++)
		{

			if (g_Vector[i].x >= 0.0f)
			{
				target = i;
			}

		}

		break;

		//ver3の部分
	default:
		//g_Judgeが3もしくは-3の時
		//点と三角形のベクトルのxもしくはyが全て正もしくは全て負の時
		//当たっていない
		return false;
		break;
	}

	//if (g_Judge == 1)
	//{
	//	for (size_t i = 0; i < 3; i++)
	//	{
	//		if (g_Vector[i].x <= 0.0f)
	//		{
	//			target = i;
	//		}
	//	}

	//}
	//else if (g_Judge == -1)
	//{
	//	for (size_t i = 0; i < 3; i++)
	//	{

	//		if (g_Vector[i].x >= 0.0f)
	//		{
	//			target = i;
	//		}

	//	}

	//}
	//for (size_t i = 0; i < 3; i++)
	//{

	//	if (i!=target)
	//	{
	//		g_Tangent[i] *= -1.0f;
	//	}

	//}

	switch (target)
	{
	case 0:
		if (g_Tangent[1] > g_Tangent[2])
		{
			max = 1;
			min = 2;
		}
		else
		{
			max = 2;
			min = 1;
		}
		break;
	case 1:
		if (g_Tangent[2] > g_Tangent[0])
		{
			max = 2;
			min = 0;
		}
		else
		{
			max = 0;
			min = 2;
		}

		break;
	case 2:
		if (g_Tangent[0] > g_Tangent[1])
		{
			max = 0;
			min = 1;
		}
		else
		{
			max = 1;
			min = 0;
		}

		break;
	}
	if (g_Tangent[min] <= g_Tangent[target] && g_Tangent[target] <= g_Tangent[max])
		return true;

	return false;

}
//
void CalCulateVector(XMFLOAT3* _OtherVertex)
{
	for (size_t i = 0; i < 3; i++)
	{
		g_Vector[i].x = g_vertex[i].Pos.x - _OtherVertex->x;
		g_Vector[i].y = g_vertex[i].Pos.y - _OtherVertex->y;
		g_Vector[i].z = g_vertex[i].Pos.z - _OtherVertex->z;

	}
}

//
void CalCulateTangent(XMFLOAT3* _OtherVertex)
{
	int Judge = 0;
	for (size_t i = 0; i < 3; i++)
	{
		if (g_Vector[i].x > 0.0f)
		{
			g_Tangent[i] = g_Vector[i].y / g_Vector[i].x;
			Judge++;
		}
		else if (g_Vector[i].x == 0.0f)
		{
			g_Tangent[i] = g_Vector[i].y / (g_Vector[i].x + 0.001f);
			Judge++;

		}
		else if (g_Vector[i].x < 0.0f)
		{
			g_Tangent[i] = g_Vector[i].y / g_Vector[i].x;
			//g_Tangent[i] = g_Vector[i].y / abs(g_Vector[i].x);
			Judge--;
		}
		g_Judge = Judge;

	}

}
