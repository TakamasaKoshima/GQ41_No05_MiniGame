#include	"triangle2nd.h"


static ID3D11Buffer*			g_VertexBuffer = nullptr;	// 頂点バッファ
static ID3D11VertexShader*		g_VertexShader = nullptr;	// 頂点シェーダー
static ID3D11PixelShader*		g_PixelShader = nullptr;	// ピクセルシェーダー
static ID3D11InputLayout*		g_VertexLayout = nullptr;	// 頂点レイアウト
static 	ID3D11Device*		 device;
 	// 頂点座標
static Vertex2nd	vertex2nd[3] = {
	XMFLOAT3(0.5f, 0.5f, 0.5f),
	XMFLOAT3(0.52f, 0.45f, 0.5f),
	XMFLOAT3(0.48f, 0.45f, 0.5f),
};

// 初期処理
bool InitTrinagle2nd() {


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
	bool sts = CreateVertexShader(			
		device,								// デバイスオブジェクト						
		"shader/vstriangle.fx",					// 頂点シェーダーソースファイル					
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

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
			device,						// デバイスオブジェクト
			"shader/pstriangle_Black.fx",			// ピクセルシェーダーソースファイル
			"main",						// エントリ関数
			"ps_5_0",					// ピクセルシェーダーモデル５．０
			&g_PixelShader);			// ピクセルシェーダーオブジェクト
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	//// 頂点座標
	//Vertex	vertex2nd[3] = {
	//	XMFLOAT3(0.5f, 0.5f, 0.5f),//上
	//	XMFLOAT3(0.55f, 0.4f, 0.5f),//左
	//	XMFLOAT3(0.45f, 0.4f, 0.5f),//右
	//};

	// 頂点バッファを生成
	sts = CreateVertexBuffer(
		device,						// デバイスオブジェクト
		sizeof(Vertex2nd),				// １頂点当たりバイト数
		3,							// 頂点数
		vertex2nd,							// 頂点データ格納メモリ先頭アドレス
		&g_VertexBuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// 描画処理
void DrawTriangle2nd() {
	ID3D11DeviceContext*		 devicecontext;				// デバイスコンテキスト

	// デバイスコンテキストを取得する
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(Vertex2nd);					// ストライドをセット（１頂点当たりのバイト数）
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
	devicecontext->PSSetShader(g_PixelShader, nullptr, 0);		// ピクセルシェーダーをセット

	// デバイスコンテキストに頂点データを流し込む
	devicecontext->Draw(
		3,									// 頂点数
		0);									// 開始頂点インデックス
}

// 終了処理
void UninitTriangle2nd() {

	if (g_VertexLayout != nullptr) {
		g_VertexLayout->Release();
		g_VertexLayout = nullptr;
	}

	if (g_VertexShader != nullptr) {
		g_VertexShader->Release();
		g_VertexShader = nullptr;
	}

	if (g_PixelShader != nullptr) {
		g_PixelShader->Release();
		g_PixelShader = nullptr;
	}

	if (g_VertexBuffer != nullptr) {
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

//頂点データ更新
void VertexUpdate(XMFLOAT3* _DeltaPos)
{
	for (size_t i = 0; i < 3; i++)
	{
		vertex2nd[i].Pos.x += _DeltaPos->x;
		vertex2nd[i].Pos.y += _DeltaPos->y;
		vertex2nd[i].Pos.z += _DeltaPos->z;
	}
	// 頂点バッファを生成
	CreateVertexBuffer(
		device,						// デバイスオブジェクト
		sizeof(Vertex2nd),				// １頂点当たりバイト数
		3,							// 頂点数
		vertex2nd,							// 頂点データ格納メモリ先頭アドレス
		&g_VertexBuffer				// 頂点バッファ
	);

}

//頂点座標を渡す
Vertex2nd* GetVertex2nd()
{
	return vertex2nd;
}
