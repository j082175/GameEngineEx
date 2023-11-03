#pragma once

#include <algorithm>
#include <directxtk/SimpleMath.h>
#include <iostream>
#include <memory>
#include <vector>

#include "AppBase.h"
#include "Buffers.h"
#include "Model.h"

namespace hlab {

	using DirectX::SimpleMath::Matrix;
	using DirectX::SimpleMath::Vector3;

	//// �� �������� ����ϴ� Vertex ����
	//struct Vertex {
	//	Vector3 position;
	//	Vector3 color;
	//};

	//// �� �������� ConstantBuffer�� ���� ������
	//struct ModelViewProjectionConstantBuffer {
	//	Matrix model;
	//	Matrix view;
	//	Matrix projection;
	//};

	class ExampleApp : public AppBase {
	public:
		ExampleApp();

		virtual bool Initialize() override;
		virtual void UpdateGUI() override;
		virtual void Update(float dt) override;
		virtual void Render() override;

	protected:
		ComPtr<ID3D11VertexShader> m_colorVertexShader;
		ComPtr<ID3D11PixelShader> m_colorPixelShader;
		ComPtr<ID3D11InputLayout> m_colorInputLayout;
		ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		ComPtr<ID3D11SamplerState> m_SamplerState;

		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_constantBuffer;
		UINT m_indexCount;

		//MVP m_constantBufferData;
		GlobalConstants m_globalConstsCPU;
		ComPtr<ID3D11Buffer> m_globalConstsGPU;
		Model m_model;

		bool m_usePerspectiveProjection = true;
	};
} // namespace hlab