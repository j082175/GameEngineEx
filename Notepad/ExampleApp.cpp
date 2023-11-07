#include "ExampleApp.h"
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

		D3D11Utils::CreateConstBuffer(m_device, m_globalConstsCPU, m_globalConstsGPU);

		//Model model1(m_device, m_context, meshData);
		//m_model.Initialize(m_device, m_context, meshData);
		std::shared_ptr<Model> model1 = std::make_shared<Model>(m_device, m_context, meshData);

		m_basicList.push_back(model1);

		meshData = GeometryGenerator::MakeBox();
		std::shared_ptr<Model> model2 = std::make_shared<Model>(m_device, m_context, meshData);

		m_basicList.push_back(model2);
		return true;
	}

	void ExampleApp::Update(float dt)
	{

		static float rot = 0;
		rot += dt;

		m_basicList[0]->UpdateModelWorld(Matrix::CreateScale(0.5f) * Matrix::CreateRotationY(rot * 2.f) * Matrix::CreateTranslation(0.f, -1.f, 0.f) * Matrix::CreateRotationZ(rot));

		m_basicList[1]->UpdateModelWorld(Matrix::CreateScale(0.2f) * Matrix::CreateRotationY(rot * 2.f) * Matrix::CreateTranslation(0.f, -1.f, 0.f));

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

		m_context->VSSetConstantBuffers(3, 1, m_globalConstsGPU.GetAddressOf());

		for (auto& i : m_basicList)
		{
			i->Render();
		}
	}

	void ExampleApp::UpdateGUI()
	{
		ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
	}

} // namespace hlab
