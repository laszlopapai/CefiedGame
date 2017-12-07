#include "IOHandler.h"
#include <iostream>
#include "Tools/Macros.h"

using namespace Cefied::Game;
TypeSetup(IOHandler)

IOHandler::IOHandler()
{
}

bool IOHandler::initialize()
{
	HRCheck(DirectInput8Create(GetModuleHandle(0),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_directInput,
		NULL), "Direct input initialize error!");


	// Zero joystick state memory
	ZeroMemory(&m_joystickCurr, sizeof(m_joystickCurr));
	ZeroMemory(&m_joystickPrev, sizeof(m_joystickPrev));
	m_joystickCurr.lX = 32768;
	m_joystickCurr.lY = 32768;
	m_joystickCurr.lZ = 32768;
	m_joystickCurr.lRx = 32768;
	m_joystickCurr.lRy = 32768;
	m_joystickCurr.lRz = 32768;
	
	// Create Joystick if the computer has any
	DIJOYCONFIG PreferredJoyCfg = { 0 };
	DI_ENUM_CONTEXT enumContext;
	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	IDirectInputJoyConfig8* pJoyConfig = nullptr;
	m_directInput->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&pJoyConfig);

	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE))) // This function is expected to fail if no joystick is attached
		enumContext.bPreferredJoyCfgValid = true;
	pJoyConfig->Release();

	void* contexts[2] = { this, &enumContext };

	// Look for a simple joystick we can use
	m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		IOHandler::enumJoysticksCallback,
		contexts, DIEDFL_ATTACHEDONLY);

/*	STOPLOG(m_directInput->CreateDevice(GUID_Joystick,
		&m_diJoystick,
		NULL), "Joystick initialize error!");*/

	// Create keyboard
	HRCheck(m_directInput->CreateDevice(GUID_SysKeyboard,
		&m_diKeyboard,
		NULL), "Keyboard initialize error!");

	// Create mouse
	HRCheck(m_directInput->CreateDevice(GUID_SysMouse,
		&m_diMouse,
		NULL), "Mouse initialize error!");

/*	// Configure joystick
	if (m_diJoystick) {
		STOPLOG(m_diJoystick->SetDataFormat(&c_dfDIJoystick2), "Joystick data format setting error!");
		STOPLOG(m_diJoystick->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE),
			"Joystick cooperativ level setting error!");
	}*/

	// Configure keyboard
	HRCheck(m_diKeyboard->SetDataFormat(&c_dfDIKeyboard), "Keyboard data format setting error!");
	HRCheck(m_diKeyboard->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_FOREGROUND*/),
		"Keyboard cooperativ level setting error!");

	// Configure mouse
	HRCheck(m_diMouse->SetDataFormat(&c_dfDIMouse), "Mouse data format setting error!");
	HRCheck(m_diMouse->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY | DISCL_BACKGROUND*/),
		"Mouse cooperativ level setting error!");

	return true;
}

BOOL CALLBACK IOHandler::enumJoysticksCallback(const DIDEVICEINSTANCE * pdidInstance,
	VOID* pContext)
{
	auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>(((VOID**)pContext)[1]);
	IOHandler* instance = (IOHandler*)(((VOID**)pContext)[0]);
	HRESULT hr;

	// Skip anything other than the perferred joystick device as defined by the control panel.  
	// Instead you could store all the enumerated joysticks and let the user pick.
	if (pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
		return DIENUM_CONTINUE;

	// Obtain an interface to the enumerated joystick.
	hr = instance->m_directInput->CreateDevice(pdidInstance->guidInstance, &instance->m_diJoystick, nullptr);

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if (FAILED(hr))
		return DIENUM_CONTINUE;

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}

// Mouse
DIMOUSESTATE & IOHandler::getMouseState()
{
	return m_mouseCurr;
}

bool IOHandler::isMButtonDown(UINT button) const
{
	return (m_mouseCurr.rgbButtons[button] & 0x80) != 0;
}

bool IOHandler::isMButtonPressed(UINT button) const
{
	return (m_mouseCurr.rgbButtons[button] & 0x80) != 0 && (m_mousePrev.rgbButtons[button] & 0x80) == 0;
}

bool IOHandler::isMButtonReleased(UINT button) const
{
	return (m_mouseCurr.rgbButtons[button] & 0x80) == 0 && (m_mousePrev.rgbButtons[button] & 0x80) != 0;
}

LONG IOHandler::getMousePosX() const
{
	return m_mouseCurr.lX;
}

LONG IOHandler::getMousePosY() const
{
	return m_mouseCurr.lY;
}

LONG IOHandler::getMousePosZ() const
{
	return m_mouseCurr.lZ;
}

LONG IOHandler::getMouseDifPosX() const
{
	return m_mousePrev.lX - m_mouseCurr.lX;
}

LONG IOHandler::getMouseDifPosY() const
{
	return m_mousePrev.lY - m_mouseCurr.lY;
}

LONG IOHandler::getMouseDifPosZ() const
{
	return m_mousePrev.lZ - m_mouseCurr.lZ;
}

// Keyboard
bool IOHandler::isKeyDown(UINT key) const
{
	return (m_keyboardCurr[key] & 0x80) != 0;
}

bool IOHandler::isKeyPressed(UINT key) const
{
	return (m_keyboardCurr[key] & 0x80) != 0 && (m_keyboardPrev[key] & 0x80) == 0;
}

bool IOHandler::isKeyReleased(UINT key) const
{
	return (m_keyboardCurr[key] & 0x80) == 0 && (m_keyboardPrev[key] & 0x80) != 0;
}


// Joystick
bool IOHandler::isButtonDown(UINT button) const
{
	return (m_joystickCurr.rgbButtons[button] & 0x80) != 0;
}

bool IOHandler::isButtonPressed(UINT button) const
{
	return (m_joystickCurr.rgbButtons[button] & 0x80) != 0 && (m_joystickPrev.rgbButtons[button] & 0x80) == 0;
}

bool IOHandler::isButtonReleased(UINT button) const
{
	return (m_joystickCurr.rgbButtons[button] & 0x80) == 0 && (m_joystickPrev.rgbButtons[button] & 0x80) != 0;
}

float IOHandler::getAxisPosX() const {
	return (m_joystickCurr.lX - 32768) / 32768.0f;
}

float IOHandler::getAxisPosY() const {
	return (m_joystickCurr.lY - 32768) / 32768.0f;
}

float IOHandler::getAxisPosZ() const {
	return (m_joystickCurr.lZ - 32768) / 32768.0f;
}

float IOHandler::getAxisRotX() const {
	return (m_joystickCurr.lRx - 32768) / 32768.0f;
}

float IOHandler::getAxisRotY() const {
	return (m_joystickCurr.lRy - 32768) / 32768.0f;
}

float IOHandler::getAxisRotZ() const {
	return (m_joystickCurr.lRz - 32768) / 32768.0f;
}


// Update
void IOHandler::getState()
{
	memcpy_s(&m_joystickPrev, sizeof(m_joystickPrev), &m_joystickCurr, sizeof(m_joystickCurr));
	memcpy_s(&m_mousePrev, sizeof(m_mousePrev), &m_mouseCurr, sizeof(m_mouseCurr));
	memcpy_s(m_keyboardPrev, sizeof(m_keyboardPrev), m_keyboardCurr, sizeof(m_keyboardCurr));

	if (m_diJoystick) {
		m_diJoystick->Acquire();
		m_diJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &m_joystickCurr);
	}

	m_diMouse->Acquire();
	m_diMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseCurr);

	m_diKeyboard->Acquire();
	m_diKeyboard->GetDeviceState(sizeof(m_keyboardCurr), (LPVOID)&m_keyboardCurr);
}

IOHandler::~IOHandler()
{
	/*if (m_diJoystick)
		m_diJoystick->Unacquire();*/

	if (m_diMouse)
		m_diMouse->Unacquire();

	if (m_diKeyboard)
		m_diKeyboard->Unacquire();
}
