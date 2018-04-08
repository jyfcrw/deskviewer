#pragma once

#define WINVER 0x0501  
#define _WIN32_WINNT 0x0501
#define _AFXDLL

#include <string>
#include <memory>
#include <afxwin.h>

class ScreenCapture {
public:
	explicit ScreenCapture();

	~ScreenCapture();

	void Capture();

private:


	std::shared_ptr<CWnd> m_desktop;
	std::shared_ptr<CDC> m_dc;
};
