#include "ExampleApp.h"

#include <tuple>
#include <vector>
#include "GeometryGenerator.h"
#include "D3D11Utils.h"

namespace hlab
{

	using namespace std;

	ExampleApp::ExampleApp() : AppBase(), m_indexCount(0)
	{
	}

	bool ExampleApp::Initialize()
	{

		if (!AppBase::Initialize())
			return false;

		MeshData meshData = GeometryGenerator::MakeBox();
		//MeshData meshData = GeometryGenerator::MakeSquare(1.f, Vector2(1.f));

		//// 버텍스 버퍼 만들기
		//D3D11Utils::CreateVertexBuffer(m_device, meshData.vertices, m_vertexBuffer);

		//// 인덱스 버퍼 만들기
		//m_indexCount = UINT(meshData.indices.size());

		//D3D11Utils::CreateIndexBuffer(m_device, meshData.indices, m_indexBuffer);

		//// ConstantBuffer 만들기
		////m_constantBufferData.model = Matrix();
		////m_constantBufferData.view = Matrix();
		////m_constantBufferData.proj = Matrix();

		D3D11Utils::CreateConstantBuffer(m_device, m_globalConstsCPU, m_globalConstsGPU);

		//// 쉐이더 만들기

		//// Input-layout objects describe how vertex buffer data is streamed into the
		//// IA(Input-Assembler) pipeline stage.
		//// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetinputlayout

		//// Input-Assembler Stage
		//// The purpose of the input-assembler stage is to read primitive data
		//// (points, lines and/or triangles) from user-filled buffers and assemble
		//// the data into primitives that will be used by the other pipeline stages.
		//// https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-input-assembler-stage

		//vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
		//	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//	{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3) * 2 + sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0},
		//};

		//D3D11Utils::CreateVertexShaderAndInputLayout(m_device, L"BasicVS.hlsl", inputElements,
		//											 m_colorVertexShader, m_colorInputLayout);

		//D3D11Utils::CreatePixelShader(m_device, L"BasicPS.hlsl", m_colorPixelShader);

		m_model.Initialize(m_device, m_context, meshData);
		return true;
	}

	void ExampleApp::Update(float dt)
	{

		static float rot = 0;
		rot += dt;


		//m_constantBufferData.model = Matrix::CreateScale(0.5f) * Matrix::CreateRotationY(rot) * Matrix::CreateTranslation(0.f, -1.f, 0.f) * Matrix::CreateRotationZ(rot);
		//m_constantBufferData.model = m_constantBufferData.model.Transpose();

		//m_constantBufferData.view = DirectX::XMMatrixLookAtLH(Vector3(0.f, 0.f, -2.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 1.f, 0.f));
		//m_constantBufferData.view = m_constantBufferData.view.Transpose();

		//const float aspect = AppBase::GetAspectRatio();
		//if (m_usePerspectiveProjection)
		//{
		//	const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
		//	m_constantBufferData.proj =
		//		DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspect, 0.01f, 100.0f);
		//}
		//else
		//{
		//	m_constantBufferData.proj =
		//		DirectX::XMMatrixOrthographicOffCenterLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
		//}
		//m_constantBufferData.proj = m_constantBufferData.proj.Transpose();

		m_model.UpdateModelWorld(Matrix::CreateScale(0.5f) * Matrix::CreateRotationY(rot) * Matrix::CreateTranslation(0.f, -1.f, 0.f) * Matrix::CreateRotationZ(rot));

		m_camera.GetViewProj(m_globalConstsCPU.viewProj);

		D3D11Utils::UpdateBuffer(m_context, m_globalConstsCPU, m_globalConstsGPU);

	}

	void ExampleApp::Render()
	{

		// IA: Input-Assembler stage
		// VS: Vertex Shader
		// PS: Pixel Shader
		// RS: Rasterizer stage
		// OM: Output-Merger stage

		m_context->RSSetViewports(1, &m_screenViewport);

		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
		m_context->ClearDepthStencilView(m_depthStencilView.Get(),
										 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
		m_context->RSSetState(m_rasterizerSate.Get());

		//m_context->IASetInputLayout(m_colorInputLayout.Get());
		//UINT stride = sizeof(Vertex);
		//UINT offset = 0;
		//m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		//m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//m_context->VSSetShader(m_colorVertexShader.Get(), NULL, 0);
		////m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
		m_context->VSSetConstantBuffers(3, 1, m_globalConstsGPU.GetAddressOf());

		//m_context->PSSetShader(m_colorPixelShader.Get(), NULL, 0);

		//m_context->DrawIndexed(m_indexCount, 0, 0);

		m_model.Render();
	}

	void ExampleApp::UpdateGUI()
	{
		ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
	}

} // namespace hlab
