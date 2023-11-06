#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <wrl.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace Microsoft::WRL;

class D3D11Utils
{
public:
	static void CreateIndexBuffer(const ComPtr<ID3D11Device>& device, const std::vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
	{

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
	static void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const std::vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
	{

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
	static void CreateConstantBuffer(const ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
	{

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

	static void CreateVertexShaderAndInputLayout(const ComPtr<ID3D11Device>& device, const std::wstring& vertexShaderfilename, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
	{

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(vertexShaderfilename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

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

	static void CreateHullShader(const ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11HullShader>& hullShader)
	{

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "hs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, hullShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Hull Shader Failed!!" << '\n';
		}
	}

	static void CreateDomainShader(const ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11DomainShader>& domainShader)
	{

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ds_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, domainShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Domain Shader Failed!!" << '\n';
		}
	}

	static void CreateGeometryShader(const ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11GeometryShader>& geometryShader)
	{

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, geometryShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Geometry Shader Failed!!" << '\n';
		}
	}

	static void CreatePixelShader(const ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11PixelShader>& pixleShader)
	{

		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(filename.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob);

		hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, pixleShader.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Create Pixel Shader Failed!!" << '\n';
		}
	}

	template<typename T_CONSTANT>
	static void UpdateBuffer(const ComPtr<ID3D11DeviceContext>& context, const T_CONSTANT& source, ComPtr<ID3D11Buffer>& dest)
	{
		D3D11_MAPPED_SUBRESOURCE ms{};
		context->Map(dest.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, &source, sizeof(T_CONSTANT));
		context->Unmap(dest.Get(), NULL);
	}

	void CreateTexture2D(const std::string& device, const std::string& filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
	{
		int width;
		int height;
		std::vector<uint8_t> image;

		ReadImage(filename, image, width, height);


	}

	ComPtr<ID3D11Texture2D> CreateStagingTexture(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& context, const std::vector<uint8_t>& image, int width, int height, const DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM, const int mipLevels = 1, const int arraySize = 1)
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = mipLevels;
		desc.ArraySize = arraySize;
		desc.Format = pixelFormat;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		ComPtr<ID3D11Texture2D> stagingTexture;
		
		HRESULT hr = device->CreateTexture2D(&desc, NULL, stagingTexture.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "CreateStagintTexture2D Failed!!" << '\n';
		}

		D3D11_MAPPED_SUBRESOURCE ms{};
		context->Map()
	}

private:
	static void ReadImage(const std::string& filename, std::vector<uint8_t>& image, int& width, int& height)
	{
		int width;
		int height;
		int channel;

		unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channel, 0);

		std::cout << "ReadImage() : " << filename << ' ' << width << ' ' << height << ' ' << channel << '\n';

		image.resize(width * height * 4);

		if (channel == 1 || channel == 2)
		{
			std::cout << "ReadImage channel is 1 or 2!!!" << '\n';
		}
		else if (channel == 3)
		{
			for (size_t i = 0; i < width * height; i++)
			{
				for (size_t c = 0; c < 3; c++)
				{
					image[4 * i + c] = img[channel * i + c];
				}

				image[4 * i + 3] = 255;
			}
		}
		else if (channel == 4)
		{
			for (size_t i = 0; i < width * height; i++)
			{
				for (size_t c = 0; c < 4; c++)
				{
					image[4 * i + c] = img[i * channel + c];
				}
			}
		}
	}

	static void CheckResult(HRESULT hr, ID3DBlob* errorBlob)
	{
		if (FAILED(hr))
		{
			// 파일이 없을 경우
			if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
			{
				std::cout << "File not found." << std::endl;
			}

			// 에러 메시지가 있으면 출력
			if (errorBlob)
			{
				std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
			}
		}
	}
};

