#include	"triangle2nd.h"


static ID3D11Buffer*			g_VertexBuffer = nullptr;	// ���_�o�b�t�@
static ID3D11VertexShader*		g_VertexShader = nullptr;	// ���_�V�F�[�_�[
static ID3D11PixelShader*		g_PixelShader = nullptr;	// �s�N�Z���V�F�[�_�[
static ID3D11InputLayout*		g_VertexLayout = nullptr;	// ���_���C�A�E�g
static 	ID3D11Device*		 device;
 	// ���_���W
static Vertex2nd	vertex2nd[3] = {
	XMFLOAT3(0.5f, 0.5f, 0.5f),
	XMFLOAT3(0.52f, 0.45f, 0.5f),
	XMFLOAT3(0.48f, 0.45f, 0.5f),
};

// ��������
bool InitTrinagle2nd() {


	// �f�o�C�X���擾����
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	bool sts = CreateVertexShader(			
		device,								// �f�o�C�X�I�u�W�F�N�g						
		"shader/vstriangle.fx",					// ���_�V�F�[�_�[�\�[�X�t�@�C��					
		"main",								// �G���g���֐�
		"vs_5_0",							// ���_�V�F�[�_�[���f���T�D�O
		layout,								// ���_�f�[�^���C�A�E�g
		numElements,						// ���_�f�[�^�̃G�������g��
		&g_VertexShader,					// ���_�V�F�[�_�[�I�u�W�F�N�g
		&g_VertexLayout);					// ���_�f�[�^���C�A�E�g�I�u�W�F�N�g
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
			device,						// �f�o�C�X�I�u�W�F�N�g
			"shader/pstriangle_Black.fx",			// �s�N�Z���V�F�[�_�[�\�[�X�t�@�C��
			"main",						// �G���g���֐�
			"ps_5_0",					// �s�N�Z���V�F�[�_�[���f���T�D�O
			&g_PixelShader);			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	//// ���_���W
	//Vertex	vertex2nd[3] = {
	//	XMFLOAT3(0.5f, 0.5f, 0.5f),//��
	//	XMFLOAT3(0.55f, 0.4f, 0.5f),//��
	//	XMFLOAT3(0.45f, 0.4f, 0.5f),//�E
	//};

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Vertex2nd),				// �P���_������o�C�g��
		3,							// ���_��
		vertex2nd,							// ���_�f�[�^�i�[�������擪�A�h���X
		&g_VertexBuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// �`�揈��
void DrawTriangle2nd() {
	ID3D11DeviceContext*		 devicecontext;				// �f�o�C�X�R���e�L�X�g

	// �f�o�C�X�R���e�L�X�g���擾����
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(Vertex2nd);					// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;									// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devicecontext->IASetVertexBuffers(			
		0,													// �X�^�[�g�X���b�g
		1,													// ���_�o�b�t�@��
		&g_VertexBuffer,									// ���_�o�b�t�@�̐擪�A�h���X
		&stride,											// �X�g���C�h
		&offset);											// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���_�t�H�[�}�b�g���Z�b�g
	devicecontext->IASetInputLayout(g_VertexLayout);

	devicecontext->VSSetShader(g_VertexShader, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
	devicecontext->PSSetShader(g_PixelShader, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

	// �f�o�C�X�R���e�L�X�g�ɒ��_�f�[�^�𗬂�����
	devicecontext->Draw(
		3,									// ���_��
		0);									// �J�n���_�C���f�b�N�X
}

// �I������
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

//���_�f�[�^�X�V
void VertexUpdate(XMFLOAT3* _DeltaPos)
{
	for (size_t i = 0; i < 3; i++)
	{
		vertex2nd[i].Pos.x += _DeltaPos->x;
		vertex2nd[i].Pos.y += _DeltaPos->y;
		vertex2nd[i].Pos.z += _DeltaPos->z;
	}
	// ���_�o�b�t�@�𐶐�
	CreateVertexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Vertex2nd),				// �P���_������o�C�g��
		3,							// ���_��
		vertex2nd,							// ���_�f�[�^�i�[�������擪�A�h���X
		&g_VertexBuffer				// ���_�o�b�t�@
	);

}

//���_���W��n��
Vertex2nd* GetVertex2nd()
{
	return vertex2nd;
}
