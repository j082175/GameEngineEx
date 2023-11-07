#include "GeometryGenerator.h"

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

MeshData GeometryGenerator::MakeSquare(const float scale,
	const Vector2 texScale) {
	using namespace std;

	vector<Vector3> positions;
	vector<Vector3> colors;
	vector<Vector3> normals;
	vector<Vector2> texcoords; // �ؽ��� ��ǥ

	// �ո�
	positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

	// Texture Coordinates (Direct3D 9)
	// https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	MeshData meshData;

	for (size_t i = 0; i < positions.size(); i++) {
		Vertex v;
		v.position = positions[i];
		v.normalModel = normals[i];
		v.texcoord = texcoords[i] * texScale;
		v.tangentModel = Vector3(1.0f, 0.0f, 0.0f);

		 //v.color = colors[i];

		meshData.vertices.push_back(v);
	}
	meshData.indices = {
		0, 1, 2, 0, 2, 3, // �ո�
	};

	return meshData;
}

MeshData GeometryGenerator::MakeBox()
{
	using namespace std;

	vector<Vector3> positions;
	vector<Vector3> colors;
	vector<Vector3> normals;
	vector<Vector2> texcoords;

	const float scale = 1.f;

	// ����
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// �Ʒ���
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// �ո�
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// �޸�
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// ����
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// ������
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));


	vector<Vertex> vertices;
	for (size_t i = 0; i < positions.size(); i++) {
		Vertex v;
		v.position = positions[i];
		v.normalModel = normals[i];
		//v.color = colors[i];
		v.texcoord = texcoords[i];
		vertices.push_back(v);
	}

	vector<uint32_t> indices = {
		0,  1,  2,  0,  2,  3,  // ����
		4, 7, 6, 4, 6, 5,
		8, 9, 10, 8, 10, 11,
		12, 15, 14, 12, 14, 13,
		16, 17, 18, 16, 18, 19,
		20, 23, 22, 20, 22, 21
	};

	MeshData meshData;
	meshData.vertices = vertices;
	meshData.indices = indices;

	return meshData;
}

