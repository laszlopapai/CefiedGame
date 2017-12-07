#pragma once

// Dx Input
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <basetsd.h>
#include <Interfaces\IAny.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

namespace Cefied
{
	namespace Game
	{
		struct XINPUT_DEVICE_NODE
		{
			DWORD dwVidPid;
			XINPUT_DEVICE_NODE* pNext;
		};

		struct DI_ENUM_CONTEXT
		{
			DIJOYCONFIG* pPreferredJoyCfg;
			bool bPreferredJoyCfgValid;
		};

		class IOHandler : public IAny
		{
		public:
			IOHandler();
			bool initialize();

			static BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext);

			// Mouse
			DIMOUSESTATE& getMouseState();
			bool isMButtonDown(UINT button) const;
			bool isMButtonPressed(UINT button) const;
			bool isMButtonReleased(UINT button) const;

			LONG getMousePosX() const;
			LONG getMousePosY() const;
			LONG getMousePosZ() const;

			LONG getMouseDifPosX() const;
			LONG getMouseDifPosY() const;
			LONG getMouseDifPosZ() const;


			// Keyboard
			bool isKeyDown(UINT key) const;
			bool isKeyPressed(UINT key) const;
			bool isKeyReleased(UINT key) const;

			// Joystick
			bool isButtonDown(UINT key) const;
			bool isButtonPressed(UINT key) const;
			bool isButtonReleased(UINT key) const;

			float getAxisPosX() const;
			float getAxisPosY() const;
			float getAxisPosZ() const;

			float getAxisRotX() const;
			float getAxisRotY() const;
			float getAxisRotZ() const;

			void setHWND(const HWND hwnd) {
				m_hwnd = hwnd;
			}
			void getState();
			~IOHandler();

		private:
			HWND m_hwnd;

			BYTE m_keyboardCurr[256];
			BYTE m_keyboardPrev[256];
			DIJOYSTATE2 m_joystickCurr;
			DIJOYSTATE2 m_joystickPrev;
			DIMOUSESTATE m_mouseCurr;
			DIMOUSESTATE m_mousePrev;

			//DIRECTX INPUT
			IDirectInputDevice8* m_diJoystick { nullptr };
			IDirectInputDevice8* m_diKeyboard { nullptr };
			IDirectInputDevice8* m_diMouse    { nullptr };
			LPDIRECTINPUT8 m_directInput;

			TypeInherit1(IAny)
		};
	}
}