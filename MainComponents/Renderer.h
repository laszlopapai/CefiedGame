#pragma once
#pragma warning (disable : 4005)

#include <d3d11.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include "Tools/Macros.h"
#include <vector>
#include "Interfaces/IViewport.h"
#include "Interfaces/IGeometry.h"
#include "Interfaces/IMaterial.h"

using namespace DirectX;

namespace Cefied
{
	namespace Game
	{

		typedef std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayoutDesc;
		class GeometryBuffer
		{
		public:
			ID3D11Buffer      *m_vertexBuffer { nullptr };
			ID3D11Buffer      *m_indexBuffer  { nullptr };
			ID3D11InputLayout *m_vertexLayout { nullptr };
			uint32_t           m_indexCount   { 0 };
			uint32_t           m_stride       { 0 };
			uint32_t           m_offset       { 0 };
			D3D_PRIMITIVE_TOPOLOGY m_topology { D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED };

			void setVertexLayout(ID3D11InputLayout * vertexlayout)
			{
				m_vertexLayout = vertexlayout;
			}

			~GeometryBuffer()
			{
				SafeRelease(m_vertexBuffer);
				SafeRelease(m_indexBuffer);
				SafeRelease(m_vertexLayout);
				m_indexCount   = 0;
				m_stride       = 0;
				m_offset       = 0;
			}
		};

		class Renderer : public IResizeable
		{
		public:
			Renderer(bool vSyncEnabled, IViewport * viewporth);
			
			bool initialize() override;

			bool resize(int width, int height) override;
			
			void beginScene(float red, float green, float blue, float alpha) const;
			
			void endScene();

			bool createMaterial(std::wstring shader, IMaterial ** material) const;

			bool createInputLayout(VertexLayoutDesc & layoutDesc, IMaterial * material, GeometryBuffer * geometryBuffer) const ;

			bool createGeometryBuffer(IGeometry * geometry, GeometryBuffer * geometryBuffer) const;

			bool drawGeometry(const GeometryBuffer & geometryBuffer, IMaterial * material) const;
			
			void setBackBufferRenderTarget() const;
			void setupViewport();
			~Renderer();

			void setHWND(HWND hwnd) {
				m_hwnd = hwnd;
			}

			// Getters
			bool ssViewportChanged() const {
				return m_viewportChanged;
			}

			ID3D11Device* getDevice() const {
				return m_device;
			}
			ID3D11DeviceContext* getImmediateContext() const {
				return m_immediateContext;
			}

			ID3D11DepthStencilView* getDepthStencilView() const {
				return m_depthStencilView;
			}
			const D3D11_VIEWPORT& getViewport() const {
				return m_viewport;
			}

			// Static
			static bool getHWParameters(int w, int h, unsigned int& numerator, unsigned int& denominator, char* desc, int& mem);

		private:
			bool                        m_viewportChanged;
			const bool                  m_vSyncEnabled;
			IViewport                  *m_windViewport;


			HWND                        m_hwnd                 { nullptr } ;

			//DIRECTX ATTRIBUTES
			ID3D11Device               *m_device;
			ID3D11DeviceContext        *m_immediateContext;
			IDXGISwapChain             *m_swapChain;
			ID3D11RenderTargetView     *m_renderTargetView;
			D3D_DRIVER_TYPE				m_driverType           { D3D_DRIVER_TYPE_UNKNOWN } ;
			D3D_FEATURE_LEVEL			m_featureLevel         { D3D_FEATURE_LEVEL_11_0 };
			D3D11_VIEWPORT				m_viewport;

			ID3D11DepthStencilView     *m_depthStencilView;
			ID3D11Texture2D            *m_depthStencilBuffer;

			TypeInherit1(IResizeable)
		};
	}
}