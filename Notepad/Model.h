#pragma once

#include "D3D11Utils.h"
#include "MeshData.h"
#include "ConstantBuffers.h"

class Model
{
public:
	Model() {};

	Model(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& context)
		: m_device(device), m_context(context)
	{
		//Initialize(device, context);
	}

	void Initialize(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& context, const MeshData& meshData)
	{
		m_device = device;
		m_context = context;


		D3D11Utils::CreateVertexBuffer(m_device, meshData.vertices, m_vertexBuffer);
		D3D11Utils::CreateIndexBuffer(m_device, meshData.indices, m_indexBuffer);
		m_indexCount = meshData.indices.size();

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3) * 2 + sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(m_device, L"BasicVS.hlsl", inputElements, m_vertexShader, m_inputLayout);
		D3D11Utils::CreatePixelShader(m_device, L"BasicPS.hlsl", m_pixelShader);
		D3D11Utils::CreateConstantBuffer(m_device, m_materialConstsCPU, m_materialConstsGPU);
		D3D11Utils::CreateConstantBuffer(m_device, m_meshConstsCPU, m_meshConstsGPU);
	}

	void UpdateModelWorld(const Matrix& modelRow)
	{
		m_meshConstsCPU.world = modelRow.Transpose();

		D3D11Utils::UpdateBuffer(m_context, m_meshConstsCPU, m_meshConstsGPU);
	}

	void Render()
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		m_context->IASetInputLayout(m_inputLayout.Get());
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_context->VSSetShader(m_vertexShader.Get(), NULL, 0);

		std::vector<ID3D11Buffer*> vertexConstantBuffers{ m_meshConstsGPU.Get(), m_materialConstsGPU.Get() };
		m_context->VSSetConstantBuffers(1, vertexConstantBuffers.size(), vertexConstantBuffers.data());

		m_context->PSSetShader(m_pixelShader.Get(), NULL, 0);

		m_context->DrawIndexed(m_indexCount, 0, 0);
	}


private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount;
	ComPtr<ID3D11Buffer> m_constantBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11HullShader> m_hullShader;
	ComPtr<ID3D11DomainShader> m_domainShader;
	ComPtr<ID3D11GeometryShader> m_geometryShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	MaterialConstants m_materialConstsCPU;
	ComPtr<ID3D11Buffer> m_materialConstsGPU;
	MeshConstants m_meshConstsCPU;
	ComPtr<ID3D11Buffer> m_meshConstsGPU;
};

