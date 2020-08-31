#include "triangle.h"
#include "triangle2nd.h"



static ID3D11Buffer*			g_VertexBuffer = nullptr;	// ���_�o�b�t�@
static ID3D11VertexShader*		g_VertexShader = nullptr;	// ���_�V�F�[�_�[
static ID3D11PixelShader*		g_PixelShader[2] = { nullptr,nullptr };	// �s�N�Z���V�F�[�_�[
static ID3D11InputLayout*		g_VertexLayout = nullptr;	// ���_���C�A�E�g
	// ���_���W
static Vertex	g_vertex[3] = {
	XMFLOAT3(0.0f, 0.0f, 0.5f),
	XMFLOAT3(0.5f, -0.75f, 0.5f),
	XMFLOAT3(-0.5f, -0.75f, 0.5f),
};
static XMFLOAT3	g_Vector[3] = {};
static float	g_Tangent[3] = {};

// ���_�V�F�[�_�[
const char* vsfilename[] = {
	"shader/vstriangle.fx",
};
const int vsfile_num = 1;

// �s�N�Z���V�F�[�_�[
const char* psfilename[] = {
	"shader/pstriangle.fx",
	"shader/pstriangle_Red.fx",
};
const int psfile_num = 2;

static bool g_CollisionFlag = false;
static int g_Judge = 0;

// ��������
bool InitTrinagle() {

	ID3D11Device*		 device;
	bool sts;
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
	for (int i = 0; i < vsfile_num; i++) {
		sts = CreateVertexShader(
			device,								// �f�o�C�X�I�u�W�F�N�g						
			vsfilename[i],					// ���_�V�F�[�_�[�\�[�X�t�@�C��					
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

	}
	// �s�N�Z���V�F�[�_�[�𐶐�
	for (int i = 0; i < psfile_num; i++) {
		sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
			device,						// �f�o�C�X�I�u�W�F�N�g
			psfilename[i],			// �s�N�Z���V�F�[�_�[�\�[�X�t�@�C��
			"main",						// �G���g���֐�
			"ps_5_0",					// �s�N�Z���V�F�[�_�[���f���T�D�O
			&g_PixelShader[i]);			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}
	}


	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Vertex),				// �P���_������o�C�g��
		3,							// ���_��
		g_vertex,							// ���_�f�[�^�i�[�������擪�A�h���X
		&g_VertexBuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// �`�揈��
void DrawTriangle() {

	ID3D11DeviceContext*		 devicecontext;				// �f�o�C�X�R���e�L�X�g

	// �f�o�C�X�R���e�L�X�g���擾����
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(Vertex);					// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
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


	if (g_CollisionFlag)
	{
		devicecontext->PSSetShader(g_PixelShader[1], nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

	}
	else
	{
		devicecontext->PSSetShader(g_PixelShader[0], nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

	}


	// �f�o�C�X�R���e�L�X�g�ɒ��_�f�[�^�𗬂�����
	devicecontext->Draw(
		3,									// ���_��
		0);									// �J�n���_�C���f�b�N�X
}

// �I������
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

//���_���W��n��
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
	//�x�N�g���쐬
	CalCulateVector(_OtherVertex);

	//�_�ƎO�p�`�̃x�N�g����x��������y���S�Đ��������͑S�ĕ��̎�
	//�������Ă��Ȃ�

		//ver1�̕���
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


		//ver2�̕���
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


		//�^���W�F���g�쐬
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

		//ver3�̕���
	default:
		//g_Judge��3��������-3�̎�
		//�_�ƎO�p�`�̃x�N�g����x��������y���S�Đ��������͑S�ĕ��̎�
		//�������Ă��Ȃ�
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
