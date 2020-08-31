#include "TriangleClass.h"


// ��������
bool TriangleClass::InitTrinagle()
{
	ID3D11Device*		 device;

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
		"vstriangle.fx",					// ���_�V�F�[�_�[�\�[�X�t�@�C��					
		"main",								// �G���g���֐�
		"vs_5_0",							// ���_�V�F�[�_�[���f���T�D�O
		layout,								// ���_�f�[�^���C�A�E�g
		numElements,						// ���_�f�[�^�̃G�������g��
		&m_VertexShader,					// ���_�V�F�[�_�[�I�u�W�F�N�g
		&m_VertexLayout);					// ���_�f�[�^���C�A�E�g�I�u�W�F�N�g
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		device,						// �f�o�C�X�I�u�W�F�N�g
		"pstriangle.fx",			// �s�N�Z���V�F�[�_�[�\�[�X�t�@�C��
		"main",						// �G���g���֐�
		"ps_5_0",					// �s�N�Z���V�F�[�_�[���f���T�D�O
		&m_PixelShader);			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	// ���_���W
	Vertex	v[3] = {
		XMFLOAT3(0.0f, 0.25f, 0.5f),
		XMFLOAT3(0.25f, -0.25f, 0.5f),
		XMFLOAT3(-0.25f, -0.25f, 0.5f),
	};

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Vertex),				// �P���_������o�C�g��
		3,							// ���_��
		v,							// ���_�f�[�^�i�[�������擪�A�h���X
		&m_VertexBuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// �`�揈��
void TriangleClass::DrawTriangle()
{
	ID3D11DeviceContext*		 devicecontext;				// �f�o�C�X�R���e�L�X�g

// �f�o�C�X�R���e�L�X�g���擾����
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(Vertex);					// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;									// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devicecontext->IASetVertexBuffers(
		0,													// �X�^�[�g�X���b�g
		1,													// ���_�o�b�t�@��
		&m_VertexBuffer,									// ���_�o�b�t�@�̐擪�A�h���X
		&stride,											// �X�g���C�h
		&offset);											// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���_�t�H�[�}�b�g���Z�b�g
	devicecontext->IASetInputLayout(m_VertexLayout);

	devicecontext->VSSetShader(m_VertexShader, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
	devicecontext->PSSetShader(m_PixelShader, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

	// �f�o�C�X�R���e�L�X�g�ɒ��_�f�[�^�𗬂�����
	devicecontext->Draw(
		3,									// ���_��
		0);									// �J�n���_�C���f�b�N�X

}

// �I������
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

//�R���X�g���N�^
TriangleClass::TriangleClass()
{
}

//�f�X�g���N�^
TriangleClass::~TriangleClass()
{
}
