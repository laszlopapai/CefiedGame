#pragma once

#include "Interfaces/IViewport.h"

#include <Windows.h>
#include <unordered_map>

namespace Cefied
{
	namespace Game
	{
		class Window : public IViewport
		{
		public:
			Window(std::string windowTitle, int w, int h);
			
			bool initialize() override;
			
			void getPosition(int& left, int& top) const override;

			void getSize(int& width, int& height) const override;
			
			void reCalculateRect();
			
			~Window();
			
			void setViewportResizedCallback(IResizeable* resizeable) override
			{
				m_resizedNotify = resizeable;
			}

			// Getters
			HWND getHWND() const {
				return m_hAppWnd;
			}


		protected:
			static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

		private:
			std::string m_title;
			HWND		m_hAppWnd { nullptr };
			RECT		m_clientRect;
			SIZE		m_clientSize;

		protected:
			static std::unordered_map<HWND, Window&> *s_openedWindowsMap;
			IResizeable                    *m_resizedNotify { nullptr } ;

			TypeInherit1(IViewport)
		};
	}
}
