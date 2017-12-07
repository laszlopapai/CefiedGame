#include "Renderer.h"
#include <ostream>
#include <iostream>
#include <d3dcompiler.h>
#include "Scene/Material.h"

using namespace Cefied::Game;
// TODO: Refactor the whole Renderer class ISP
// TODO: IViewport location rethink

TypeSetup(Renderer)

Renderer::Renderer(const bool vSyncEnabled, IViewport * viewport) :
	m_device(nullptr),
	m_immediateContext(nullptr),
	m_swapChain(nullptr),
	m_renderTargetView(nullptr),

	m_depthStencilView(nullptr),
	m_depthStencilBuffer(nullptr),

	m_viewportChanged(true),

	m_vSyncEnabled(vSyncEnabled),
	m_windViewport(viewport)
{ }

bool Renderer::initialize()
{
	m_windViewport->setViewportResizedCallback(this);
	setupViewport();

	unsigned int numerator, denominator;
	char desc[128];
	int mem;

	getHWParameters((int)m_viewport.Width, (int)m_viewport.Height, numerator, denominator, desc, mem);

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //DEBUG

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferCount = 1; //double buffered

	swapDesc.BufferDesc.Width = (UINT)m_viewport.Width;
	swapDesc.BufferDesc.Height = (UINT)m_viewport.Height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vSyncEnabled)
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hwnd;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //switch fullscreen

	HRESULT result = 0;
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_swapChain, &m_device,
			&m_featureLevel, &m_immediateContext);

		if (SUCCEEDED(result))
		{
			m_driverType = driverTypes[i];
			break;
		}
	}	

	HRCheck(result,
		"Could not initialize Direct3D.");


	//CREATE RENDER TARGET VIEW
	ID3D11Texture2D *m_pBackBufferTex = 0;
	HRCheck(m_swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTex)),
		"Swap chain error.");

	HRCheck(m_device->CreateRenderTargetView(m_pBackBufferTex, nullptr, &m_renderTargetView),
		"Swap chain error.");

	SafeRelease(m_pBackBufferTex);


	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = (UINT)m_viewport.Width;
	depthBufferDesc.Height = (UINT)m_viewport.Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRCheck(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer),
		"Failed to create the depth stencil view");


	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	HRCheck(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView),
		"DepthStencilState creation failure.");

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	// Create the viewport.
	m_immediateContext->RSSetViewports(1, &m_viewport);
	
	return true;
}

bool Renderer::resize(int width, int height)
{
	if (!m_swapChain)
		return false;

	m_immediateContext->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's buffers.
	SafeRelease(m_renderTargetView);

	// Preserve the existing buffer count and format.
	// Automatically choose the Width and height to match the client rect for HWNDs.
	HRCheck(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0),
		"Swap chain resize error.");


	// Get buffer and create a render-target-view.
	ID3D11Texture2D* pBuffer;
	HRCheck(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&pBuffer)),
		"Swap chain resize error.");

	HRCheck(m_device->CreateRenderTargetView(pBuffer, nullptr,
		&m_renderTargetView),
		"Render target view creation error.");

	SafeRelease(pBuffer);

	// Set up the viewport.
	setupViewport();
	m_immediateContext->RSSetViewports(1, &m_viewport);

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = (UINT)m_viewport.Width;
	depthBufferDesc.Height = (UINT)m_viewport.Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	SafeRelease(m_depthStencilBuffer);
	HRCheck(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer),
		"DepthStencilBuffer creation failure.");

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	SafeRelease(m_depthStencilView);
	// Create the depth stencil view.
	HRCheck(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView),
		"DepthStencilState creation failure.");

	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	m_viewportChanged = true;
	return true;
}

void Renderer::beginScene(float red, float green, float blue, float alpha) const
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::endScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vSyncEnabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	m_viewportChanged = false;
}

bool Renderer::createMaterial(std::wstring shader, IMaterial ** material) const
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	ID3DX11Effect *effect;

	HRCheck(D3DX11CompileEffectFromFile(shader.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, m_device, &effect, &pErrorBlob),
		"Shader initialization failed:",
		{
			if (pErrorBlob) {
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
				std::cerr << (char*)pErrorBlob->GetBufferPointer() << std::endl;
			}

			SafeRelease(pErrorBlob);
		});

	*material = new Material(effect);

	return true;
}

bool Renderer::createInputLayout(VertexLayoutDesc & layoutDesc, IMaterial * material, GeometryBuffer * geometryBuffer) const
{
	InitCheck(material->isInstanceOf<Material>(),
		"The passed material is not a D3D Material");
	auto d3dMaterial = static_cast<Material*>(material);
	auto technique = d3dMaterial->getCurrentTechnique();

	ID3D11InputLayout *inputLayout;
	D3DX11_PASS_DESC passDesc;
	HRCheck(technique->GetPassByIndex(0)->GetDesc(&passDesc), "Technique description query failed.");

	HRCheck(m_device->CreateInputLayout(&layoutDesc[0], layoutDesc.size(), passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &inputLayout), "Input layout creation failed.");

	geometryBuffer->setVertexLayout(inputLayout);

	return true;
}

bool Renderer::createGeometryBuffer(IGeometry * geometry, GeometryBuffer * gb) const
{
	// Create vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	//auto gb = new GeometryBuffer;

	gb->m_stride = geometry->getStride();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = geometry->getStride() * geometry->getVerticesCount();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = geometry->getVertices();
	HRCheck(m_device->CreateBuffer(&bd, &InitData, &gb->m_vertexBuffer),
		"Vertex buffer creation error.");

	// Create index buffer
	gb->m_indexCount = geometry->getIndicesCount();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint16_t) * geometry->getIndicesCount();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = geometry->getIndices();
	HRCheck(m_device->CreateBuffer(&bd, &InitData, &gb->m_indexBuffer),
		"Index buffer creation error.");


	if (geometry->getTopology() == Triangle) {
		gb->m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
	if (geometry->getTopology() == Line) {
		gb->m_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	}

	//createInputLayout(layoutDesc, material, gb);

	//*geometryBuffer = gb;

	return true;
}

bool Renderer::drawGeometry(const GeometryBuffer& geometryBuffer, IMaterial * material) const
{
	InitCheck(material->isInstanceOf<Material>(),
		"The passed material is not a D3D Material");
	auto d3dMaterial = static_cast<Material*>(material);
	auto technique = d3dMaterial->getCurrentTechnique();

	D3DX11_TECHNIQUE_DESC techDesc;
	technique->GetDesc(&techDesc);
	for (uint32_t p = 0; p < techDesc.Passes; ++p)
	{
		HRCheck(technique->GetPassByIndex(p)->Apply(0, m_immediateContext),
			"Failed to apply effect techniques pass.");

		m_immediateContext->IASetVertexBuffers(0, 1, &geometryBuffer.m_vertexBuffer, &geometryBuffer.m_stride, &geometryBuffer.m_offset);
		m_immediateContext->IASetIndexBuffer(geometryBuffer.m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		m_immediateContext->IASetPrimitiveTopology(geometryBuffer.m_topology);

		m_immediateContext->IASetInputLayout(geometryBuffer.m_vertexLayout);
		m_immediateContext->DrawIndexed(geometryBuffer.m_indexCount, 0, 0);
	}

	return true;
}

void Renderer::setBackBufferRenderTarget() const
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	m_immediateContext->RSSetViewports(1, &m_viewport);
}

void Renderer::setupViewport()
{
	RECT client;
	GetClientRect(m_hwnd, &client);


	// Setup the viewport for rendering.
	m_viewport.Width = (float)(client.right - client.left);
	m_viewport.Height = (float)(client.bottom - client.top);
	m_viewport.TopLeftX = (float)client.left;
	m_viewport.TopLeftY = (float)client.top;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}

Renderer::~Renderer()
{
	if (m_immediateContext)
		m_immediateContext->ClearState();

	SafeRelease(m_device);
	SafeRelease(m_immediateContext);
	SafeRelease(m_swapChain);
	SafeRelease(m_renderTargetView);

	SafeRelease(m_depthStencilView);
	SafeRelease(m_depthStencilBuffer);
}

bool Renderer::getHWParameters(int w, int h, unsigned int& numerator, unsigned int& denominator, char* desc, int& mem)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}
	
	// Now go through all the display modes and find the one that matches the screen Width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)w)
		{
			if (displayModeList[i].Height == (unsigned int)h)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
		//std::cout << displayModeList[i].Width << "x" << displayModeList[i].Height << "x" << displayModeList[i].RefreshRate.Numerator / displayModeList[i].RefreshRate.Denominator << std::endl;
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	mem = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	int smem = (int)(adapterDesc.SharedSystemMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, desc, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	std::cout << 
		"------------------- Graphics Card Informations -------------------" << std::endl <<
		" Model: " << desc << std::endl << 
		" Dedicated GPU memory: " << mem << " MiB" << std::endl <<
		" Shared system memory: " << smem << " MiB" << std::endl <<
		"------------------------------------------------------------------" << std::endl;

	// Release the display mode list.
	SafeDeleteArr(displayModeList);

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;
	return true;
}