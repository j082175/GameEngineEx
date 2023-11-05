#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <wrl.h> // ComPtr
#include "Camera.h"

namespace hlab
{

	using Microsoft::WRL::ComPtr;
	using std::vector;
	using std::wstring;

	// 모든 예제들이 공통적으로 사용할 기능들을 가지고 있는
	// 부모 클래스
	class AppBase
	{
	public:
		AppBase();
		virtual ~AppBase();
		int Run();
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	protected: // 상속 받은 클래스에서도 접근 가능
		virtual bool Initialize();
		virtual void UpdateGUI() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

		bool InitMainWindow();
		bool InitDirect3D();
		bool InitGUI();

		float GetAspectRatio() const;
		// Convenience overrides for handling mouse input.
		virtual void OnMouseDown(WPARAM btnState, int x, int y) {};
		virtual void OnMouseUp(WPARAM btnState, int x, int y) {};
		virtual void OnMouseMove(WPARAM btnState, int x, int y) {};
	private:


	public:
		// 변수 이름 붙이는 규칙은 VS DX11/12 기본 템플릿을 따릅니다.
		// 다만 변수 이름을 줄이기 위해 d3d는 생략했습니다.
		// 예: m_d3dDevice -> m_device
		int m_screenWidth; // 렌더링할 최종 화면의 해상도
		int m_screenHeight;
		HWND m_mainWindow;
		Camera m_camera;

		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;

		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11RasterizerState> m_rasterizerSate;

		// Depth buffer 관련
		ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;

		D3D11_VIEWPORT m_screenViewport;
	};
} // namespace hlab