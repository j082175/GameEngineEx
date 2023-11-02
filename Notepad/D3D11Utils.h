#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <wrl.h>
#include <iostream>

using namespace Microsoft::WRL;

class D3D11Utils
{
public:
	static void CreateIndexBuffer(const ComPtr<ID3D11Device>& device, const std::vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer) {

		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(uint32_t) * indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = indices.data();

		HRESULT hr = device->CreateBuffer(&desc, &subData, indexBuffer.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Index Buffer Failed!!" << '\n';
		}
	}

	template<typename T_VERTEX>
	static void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const std::vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer) {

		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(T_VERTEX) * vertices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = vertices.data();

		HRESULT hr = device->CreateBuffer(&desc, &subData, vertexBuffer.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Vertex Buffer Failed!!" << '\n';
		}
	}

	template<typename T_CONSTANT>
	static void CreateConstantBuffer(const ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer) {

		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(T_CONSTANT);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = &constantBufferData;

		HRESULT hr = device->CreateBuffer(&desc, &subData, constantBuffer.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Constant Buffer Failed!!" << '\n';
		}
	}

	static void CreateVertexShaderAndInputLayout(const ComPtr<ID3D11Device>& device, const std::wstring& vertexShaderFileName, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout) {

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(vertexShaderFileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		if (FAILED(hr))
		{
			std::cout << "Compile Vertex Shader Failed!!" << '\n';
		}

		hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, vertexShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Vertex Shader Failed!!" << '\n';
		}

		hr = device->CreateInputLayout(inputElements.data(), inputElements.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Input Layout Failed!!" << '\n';
		}
	}

	static void CreateHullShader(const ComPtr<ID3D11Device>& device, const std::wstring& fileName, ComPtr<ID3D11HullShader>& hullShader) {

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "hs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, hullShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Hull Shader Failed!!" << '\n';
		}
	}

	static void CreateDomainShader(const ComPtr<ID3D11Device>& device, const std::wstring& fileName, ComPtr<ID3D11DomainShader>& domainShader) {

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ds_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, domainShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Domain Shader Failed!!" << '\n';
		}
	}

	static void CreateGeometryShader(const ComPtr<ID3D11Device>& device, const std::wstring& fileName, ComPtr<ID3D11GeometryShader>& geometryShader) {

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, geometryShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Geometry Shader Failed!!" << '\n';
		}
	}

	static void CreatePixelShader(const ComPtr<ID3D11Device>& device, const std::wstring& fileName, ComPtr<ID3D11PixelShader>& pixleShader) {

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, pixleShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Pixel Shader Failed!!" << '\n';
		}
	}

	//void CreateTexture2D(const ComPtr<ID3D11Device>& device, )

private:
	static void CheckResult(HRESULT hr, ID3DBlob* errorBlob) {
		if (FAILED(hr)) {
			// ������ ���� ���
			if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
				std::cout << "File not found." << std::endl;
			}

			// ���� �޽����� ������ ���
			if (errorBlob) {
				std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
			}
		}
	}
};
