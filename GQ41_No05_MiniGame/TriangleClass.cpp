#include "TriangleClass.h"


// 初期処理
bool TriangleClass::InitTrinagle()
{
	ID3D11Device*		 device;

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
		"vstriangle.fx",					// 頂点シェーダーソースファイル					
		"main",								// エントリ関数
		"vs_5_0",							// 頂点シェーダーモデル５．０
		layout,								// 頂点データレイアウト
		numElements,						// 頂点データのエレメント数
		&m_VertexShader,					// 頂点シェーダーオブジェクト
		&m_VertexLayout);					// 頂点データレイアウトオブジェクト
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,						// デバイスオブジェクト
		"pstriangle.fx",			// ピクセルシェーダーソースファイル
		"main",						// エントリ関数
		"ps_5_0",					// ピクセルシェーダーモデル５．０
		&m_PixelShader);			// ピクセルシェーダーオブジェクト
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	// 頂点座標
	Vertex	v[3] = {
		XMFLOAT3(0.0f, 0.25f, 0.5f),
		XMFLOAT3(0.25f, -0.25f, 0.5f),
		XMFLOAT3(-0.25f, -0.25f, 0.5f),
	};

	// 頂点バッファを生成
	sts = CreateVertexBuffer(
		device,						// デバイスオブジェクト
		sizeof(Vertex),				// １頂点当たりバイト数
		3,							// 頂点数
		v,							// 頂点データ格納メモリ先頭アドレス
		&m_VertexBuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// 描画処理
void TriangleClass::DrawTriangle()
{
	ID3D11DeviceContext*		 devicecontext;				// デバイスコンテキスト

// デバイスコンテキストを取得する
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(Vertex);					// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;									// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devicecontext->IASetVertexBuffers(
		0,													// スタートスロット
		1,													// 頂点バッファ個数
		&m_VertexBuffer,									// 頂点バッファの先頭アドレス
		&stride,											// ストライド
		&offset);											// オフセット

	// トポロジーをセット
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点フォーマットをセット
	devicecontext->IASetInputLayout(m_VertexLayout);

	devicecontext->VSSetShader(m_VertexShader, nullptr, 0);		// 頂点シェーダーをセット
	devicecontext->PSSetShader(m_PixelShader, nullptr, 0);		// ピクセルシェーダーをセット

	// デバイスコンテキストに頂点データを流し込む
	devicecontext->Draw(
		3,									// 頂点数
		0);									// 開始頂点インデックス

}

// 終了処理
void TriangleClass::UninitTriangle()
{
	if (m_VertexLayout != nullptr) {
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}

	if (m_VertexShader != nullptr) {
		m_VertexShader->Release();
		m_VertexShader = nullptr;
	}

	if (m_PixelShader != nullptr) {
		m_PixelShader->Release();
		m_PixelShader = nullptr;
	}

	if (m_VertexBuffer != nullptr) {
		m_VertexBuffer->Release();
		m_VertexBuffer = nullptr;
	}

}

//コンストラクタ
TriangleClass::TriangleClass()
{
}

//デストラクタ
TriangleClass::~TriangleClass()
{
}
