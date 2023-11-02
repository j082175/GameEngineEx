// ����: ��� include ����
// https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes

#include "AppBase.h"

#include <dxgi.h>    // DXGIFactory
#include <dxgi1_4.h> // DXGIFactory4

// imgui_impl_win32.cpp�� ���ǵ� �޽��� ó�� �Լ��� ���� ���� ����
// VCPKG�� ���� IMGUI�� ����� ��� �����ٷ� ����� �� �� ����
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);

namespace hlab {

	using namespace std;

	// RegisterClassEx()���� ��� �Լ��� ���� ����� ���� ���� ������
	// Ŭ������ ��� �Լ����� ���������� �޽����� ó���� �� �ֵ��� �����ݴϴ�.
	AppBase* g_appBase = nullptr;

	// RegisterClassEx()���� ������ ��ϵ� �ݹ� �Լ�
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		// g_appBase�� �̿��ؼ� ���������� ��� �Լ� ȣ��
		return g_appBase->MsgProc(hWnd, msg, wParam, lParam);
	}

	// ������
	AppBase::AppBase()
		: m_screenWidth(1280), m_screenHeight(720), m_mainWindow(0),
		m_screenViewport(D3D11_VIEWPORT()) {

		g_appBase = this;
	}

	AppBase::~AppBase() {
		g_appBase = nullptr;

		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DestroyWindow(m_mainWindow);
		// UnregisterClass(wc.lpszClassName, wc.hInstance);//����

		// COMPtr���� �˾Ƽ� release
		// ComPtr automatically maintains a reference count for the underlying
		// interface pointer and releases the interface when the reference count
		// goes to zero.
		// https:learn.microsoft.com/en-us/cpp/cppcx/wrl/comptr-class?view=msvc-170
		// ����: m_d3dDevice.Reset(); ����
	}

	float AppBase::GetAspectRatio() const { return float(m_screenWidth) / m_screenHeight; }

	int AppBase::Run() {

		// Main message loop
		MSG msg = { 0 };
		while (WM_QUIT != msg.message) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				ImGui_ImplDX11_NewFrame(); // GUI ������ ����
				ImGui_ImplWin32_NewFrame();

				ImGui::NewFrame(); // � �͵��� ������ ���� ��� ����
				ImGui::Begin("Scene Control");

				// ImGui�� �������ִ� Framerate ���
				ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
					ImGui::GetIO().Framerate);

				UpdateGUI(); // �߰������� ����� GUI

				ImGui::End();
				ImGui::Render(); // �������� �͵� ��� ��

				Update(ImGui::GetIO().DeltaTime); // �ִϸ��̼� ���� ��ȭ

				Render(); // �츮�� ������ ������

				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI ������

				// Switch the back buffer and the front buffer
				// ����: ImGui RenderDrawData() ������ Present() ȣ��
				m_swapChain->Present(1, 0);
			}
		}

		return 0;
	}

	bool AppBase::Initialize() {

		if (!InitMainWindow())
			return false;

		if (!InitDirect3D())
			return false;

		if (!InitGUI())
			return false;

		return true;
	}

	LRESULT AppBase::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

		switch (msg) {
		case WM_SIZE:
			// Reset and resize swapchain
			break;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_MOUSEMOVE:
			// cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << endl;
			break;
		case WM_LBUTTONUP:
			// cout << "WM_LBUTTONUP Left mouse button" << endl;
			break;
		case WM_RBUTTONUP:
			// cout << "WM_RBUTTONUP Right mouse button" << endl;
			break;
		case WM_KEYDOWN:
			// cout << "WM_KEYDOWN " << (int)wParam << endl;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}

		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool AppBase::InitMainWindow() {

		WNDCLASSEX wc = { sizeof(WNDCLASSEX),
						 CS_CLASSDC,
						 WndProc,
						 0L,
						 0L,
						 GetModuleHandle(NULL),
						 NULL,
						 NULL,
						 NULL,
						 NULL,
						 L"HongLabGraphics", // lpszClassName, L-string
						 NULL };

		// The RegisterClass function has been superseded by the RegisterClassEx function.
		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
		if (!RegisterClassEx(&wc)) {
			cout << "RegisterClassEx() failed." << endl;
			return false;
		}

		// ���ٱ��� ������ ������ ��ü �ػ󵵰� �ƴ϶�
		// �츮�� ������ �׸��� �ػ󵵰� width x height�� �ǵ���
		// �����츦 ���� �ػ󵵸� �ٽ� ����ؼ� CreateWindow()���� ���

		// �츮�� ���ϴ� �׸��� �׷��� �κ��� �ػ�
		RECT wr = { 0, 0, m_screenWidth, m_screenHeight };

		// �ʿ��� ������ ũ��(�ػ�) ���
		// wr�� ���� �ٲ�
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

		// �����츦 ���鶧 ������ ����� wr ���
		m_mainWindow = CreateWindow(wc.lpszClassName, L"HongLabGraphics Example", WS_OVERLAPPEDWINDOW,
			100,                // ������ ���� ����� x ��ǥ
			100,                // ������ ���� ����� y ��ǥ
			wr.right - wr.left, // ������ ���� ���� �ػ�
			wr.bottom - wr.top, // ������ ���� ���� �ػ�
			NULL, NULL, wc.hInstance, NULL);

		if (!m_mainWindow) {
			cout << "CreateWindow() failed." << endl;
			return false;
		}

		ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
		UpdateWindow(m_mainWindow);

		return true;
	}

	bool AppBase::InitDirect3D() {

		// �� ������ Intel ���� �׷��Ƚ� Ĩ���� ������ Ȯ���Ͽ����ϴ�.
		// (LG �׷�, 17Z90n, Intel Iris Plus Graphics)
		// ���� �׷��Ƚ� ī�� ȣȯ�� ������ D3D11CreateDevice()�� �����ϴ� ��쿡��
		// D3D_DRIVER_TYPE_HARDWARE ��� D3D_DRIVER_TYPE_WARP ����غ�����
		// const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_WARP;
		const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

		// ���⼭ �����ϴ� �͵�
		// m_device, m_context, m_swapChain,
		// m_renderTargetView, m_screenViewport, m_rasterizerSate

		// m_device�� m_context ����

		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		const D3D_FEATURE_LEVEL featureLevels[2] = {
			D3D_FEATURE_LEVEL_11_0, // �� ���� ������ ���� ������ ����
			D3D_FEATURE_LEVEL_9_3 };
		D3D_FEATURE_LEVEL featureLevel;

		if (FAILED(D3D11CreateDevice(
			nullptr,                  // Specify nullptr to use the default adapter.
			driverType,               // Create a device using the hardware graphics driver.
			0,                        // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
			createDeviceFlags,        // Set debug and Direct2D compatibility flags.
			featureLevels,            // List of feature levels this app can support.
			ARRAYSIZE(featureLevels), // Size of the list above.
			D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
			&device,           // Returns the Direct3D device created.
			&featureLevel,     // Returns feature level of device created.
			&context           // Returns the device immediate context.
		))) {
			cout << "D3D11CreateDevice() failed." << endl;
			return false;
		}

		/* ����: ������ ���� ��� ���� �߻� ���

		// MS ����
		inline void ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				// Set a breakpoint on this line to catch Win32 API errors.
				throw Platform::Exception::CreateException(hr);
			}
		}

		// Luna DX12 ����
		#ifndef ThrowIfFailed
		#define ThrowIfFailed(x)                                              \
		{                                                                     \
			HRESULT hr__ = (x);                                               \
			std::wstring wfn = AnsiToWString(__FILE__);                       \
			if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
		}
		#endif
		*/

		if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
			cout << "D3D Feature Level 11 unsupported." << endl;
			return false;
		}

		// ����: Immediate vs deferred context
		// A deferred context is primarily used for multithreading and is not necessary for a
		// single-threaded application.
		// https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context

		// 4X MSAA �����ϴ��� Ȯ��
		UINT numQualityLevels;
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
		if (numQualityLevels <= 0) {
			cout << "MSAA not supported." << endl;
		}

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = m_screenWidth;               // set the back buffer width
		sd.BufferDesc.Height = m_screenHeight;             // set the back buffer height
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
		sd.BufferCount = 2;                                // Double-buffering
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
		sd.OutputWindow = m_mainWindow;                    // the window to be used
		sd.Windowed = TRUE;                                // windowed/full-screen mode
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		if (numQualityLevels > 0) {
			sd.SampleDesc.Count = 4; // how many multisamples
			sd.SampleDesc.Quality = numQualityLevels - 1;
		}
		else {
			sd.SampleDesc.Count = 1; // how many multisamples
			sd.SampleDesc.Quality = 0;
		}

		if (FAILED(device.As(&m_device))) {
			cout << "device.AS() failed." << endl;
			return false;
		}

		if (FAILED(context.As(&m_context))) {
			cout << "context.As() failed." << endl;
			return false;
		}

		// ����: IDXGIFactory�� �̿��� CreateSwapChain()
		/*
		ComPtr<IDXGIDevice3> dxgiDevice;
		m_device.As(&dxgiDevice);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		 ComPtr<IDXGIFactory> dxgiFactory;
		 dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

		 ComPtr<IDXGISwapChain> swapChain;
		 dxgiFactory->CreateSwapChain(m_device.Get(), &sd, &swapChain);

		 swapChain.As(&m_swapChain);
		 */

		 // ����: IDXGIFactory4�� �̿��� CreateSwapChainForHwnd()
		 /*
		 ComPtr<IDXGIFactory4> dxgiFactory;
		 dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

		 DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
		 swapChainDesc.Width = lround(m_screenWidth); // Match the size of the window.
		 swapChainDesc.Height = lround(m_screenHeight);
		 swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
		 swapChainDesc.Stereo = false;
		 swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
		 swapChainDesc.SampleDesc.Quality = 0;
		 swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		 swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
		 swapChainDesc.SwapEffect =
			 DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Microsoft Store apps must use this SwapEffect.
		 swapChainDesc.Flags = 0;
		 swapChainDesc.Scaling = DXGI_SCALING_NONE;
		 swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		 ComPtr<IDXGISwapChain1> swapChain;
		 dxgiFactory->CreateSwapChainForHwnd(m_device.Get(), m_mainWindow, &swapChainDesc, nullptr,
		 nullptr, swapChain.GetAddressOf());
		 */

		if (FAILED(D3D11CreateDeviceAndSwapChain(0, // Default adapter
			driverType,
			0, // No software device
			createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION,
			&sd, &m_swapChain, &m_device, &featureLevel,
			&m_context))) {
			cout << "D3D11CreateDeviceAndSwapChain() failed." << endl;
			return false;
		}

		// CreateRenderTarget
		ID3D11Texture2D* pBackBuffer;
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (pBackBuffer) {
			m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
			pBackBuffer->Release();
		}
		else {
			cout << "CreateRenderTargetView() failed." << endl;
			return false;
		}

		// Set the viewport
		ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
		m_screenViewport.TopLeftX = 0;
		m_screenViewport.TopLeftY = 0;
		m_screenViewport.Width = float(m_screenWidth);
		m_screenViewport.Height = float(m_screenHeight);
		// m_screenViewport.Width = static_cast<float>(m_screenHeight);
		m_screenViewport.MinDepth = 0.0f;
		m_screenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

		m_context->RSSetViewports(1, &m_screenViewport);

		// Create a rasterizer state
		D3D11_RASTERIZER_DESC rastDesc;
		ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
		rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		// rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rastDesc.FrontCounterClockwise = false;

		m_device->CreateRasterizerState(&rastDesc, &m_rasterizerSate);

		// Create depth buffer

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = m_screenWidth;
		depthStencilBufferDesc.Height = m_screenHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (numQualityLevels > 0) {
			depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
			depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
		}
		else {
			depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
			depthStencilBufferDesc.SampleDesc.Quality = 0;
		}
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		if (FAILED(m_device->CreateTexture2D(&depthStencilBufferDesc, 0,
			m_depthStencilBuffer.GetAddressOf()))) {
			cout << "CreateTexture2D() failed." << endl;
		}
		if (FAILED(
			m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView))) {
			cout << "CreateDepthStencilView() failed." << endl;
		}

		// Create depth stencil state

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = true; // false
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc,
			m_depthStencilState.GetAddressOf()))) {
			cout << "CreateDepthStencilState() failed." << endl;
		}

		return true;
	}

	bool AppBase::InitGUI() {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.DisplaySize = ImVec2(float(m_screenWidth), float(m_screenHeight));
		ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) {
			return false;
		}

		if (!ImGui_ImplWin32_Init(m_mainWindow)) {
			return false;
		}

		return true;
	}

	// ����: How To: Compile a Shader
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader

	// ����: �տ� L�� ���� ���ڿ��� wide character�� �̷���� ���ڿ���
	// �ǹ��մϴ�.
	// String and character literals (C++)
	// https://learn.microsoft.com/en-us/cpp/cpp/string-and-character-literals-cpp?view=msvc-170

	// ����: ���̴��� �̸� �������ص� .cso ���Ϸκ��� ���� ���� �ֽ��ϴ�.
	// Ȯ���� cso�� Compiled Shader Object�� �ǹ��մϴ�.
	// ���⼭�� ���̴� ������ �о�鿩�� �������մϴ�.

	void CheckResult(HRESULT hr, ID3DBlob* errorBlob) {
		if (FAILED(hr)) {
			// ������ ���� ���
			if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
				cout << "File not found." << endl;
			}

			// ���� �޽����� ������ ���
			if (errorBlob) {
				cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
			}
		}
	}

	void AppBase::CreateVertexShaderAndInputLayout(
		const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout) {

		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		// ����: ���̴��� �������� �̸��� "main"�� �Լ��� ����
		HRESULT hr =
			D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob.Get());

		m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
			&vertexShader);

		m_device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			&inputLayout);
	}

	void AppBase::CreatePixelShader(const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader) {
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		// ����: ���̴��� �������� �̸��� "main"�� �Լ��� ����
		HRESULT hr =
			D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob.Get());

		m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
			&pixelShader);
	}

	void AppBase::CreateIndexBuffer(const std::vector<uint32_t>& indices,
		ComPtr<ID3D11Buffer>& m_indexBuffer) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
		bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
		bufferDesc.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		m_device->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
	}

} // namespace hlab