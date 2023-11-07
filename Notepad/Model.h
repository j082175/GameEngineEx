#pragma once

#include "D3D11Utils.h"
#include "MeshData.h"
#include "ConstantBuffers.h"

class Model
{
public:
	Model() {};

	Model(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& context, const MeshData& meshData)
		: m_device(device), m_context(context), m_indexCount(0)
	{
		//Initialize(device, context);
		Initialize(meshData);
	}

	void Initialize(const MeshData& meshData)
	{
		//m_device = device;
		//m_context = context;


		D3D11Utils::CreateVertexBuffer(m_device, meshData.vertices, m_vertexBuffer);
		D3D11Utils::CreateIndexBuffer(m_device, meshData.indices, m_indexBuffer);
		m_indexCount = (UINT)meshData.indices.size();

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3) * 2 + sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(m_device, L"BasicVS.hlsl", inputElements, m_vertexShader, m_inputLayout);
		D3D11Utils::CreatePixelShader(m_device, L"BasicPS.hlsl", m_pixelShader);
		D3D11Utils::CreateConstBuffer(m_device, m_materialConstsCPU, m_materialConstsGPU);
		D3D11Utils::CreateConstBuffer(m_device, m_meshConstsCPU, m_meshConstsGPU);


		D3D11_SAMPLER_DESC sampDesc{};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

		D3D11Utils::CreateTexture(m_device, m_context, "../../Assets/Textures/blender_uv_grid_2k.png", false, m_texture2D, m_textureResourceView);
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
		m_context->VSSetConstantBuffers(1, (UINT)vertexConstantBuffers.size(), vertexConstantBuffers.data());

		m_context->PSSetShader(m_pixelShader.Get(), NULL, 0);
		std::vector<ID3D11ShaderResourceView*> shaderResources{ m_textureResourceView.Get() };
		m_context->PSSetShaderResources(0, (UINT)shaderResources.size(), shaderResources.data());
		m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

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

	ComPtr<ID3D11Texture2D> m_texture2D;
	ComPtr<ID3D11ShaderResourceView> m_textureResourceView;

	ComPtr<ID3D11SamplerState> m_samplerState;
};

