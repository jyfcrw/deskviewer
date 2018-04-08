#include "screen_capture.hpp"

#pragma comment(lib, "GdiPlus.lib")

using namespace std;


wstring GetAppPathW()
{
	wchar_t szExePath[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szExePath, MAX_PATH);
	wchar_t *pstr = wcsrchr(szExePath, '\\');
	memset(pstr + 1, 0, 2);
	wstring strAppPath(szExePath);
	return strAppPath;
}

ScreenCapture::ScreenCapture()
{
	m_desktop = std::make_shared<CWnd>(CWnd::GetDesktopWindow());
	m_dc = std::make_shared<CDC>(m_desktop->GetDC());
}

ScreenCapture::~ScreenCapture()
{
	// free m_desktop?
}

void ScreenCapture::Capture()
{
	CRect rect;
	CBitmap bmp;

	//获取窗口的大小  
	m_desktop->GetClientRect(&rect);
	bmp.CreateCompatibleBitmap(m_dc.get(), rect.Width(), rect.Height());

	//创建一个兼容的内存画板  
	CDC memory_dc;
	memory_dc.CreateCompatibleDC(m_dc.get());

	CBitmap *p_bmp = memory_dc.SelectObject(&bmp);
	memory_dc.BitBlt(0, 0, rect.Width(), rect.Height(), m_dc.get(), 0, 0, SRCCOPY);

	//获取鼠标位置，然后添加鼠标图像  
	CPoint point;
	GetCursorPos(&point);
	HICON hinco = (HICON)GetCursor();
	memory_dc.DrawIcon(point.x - 10, point.y - 10, hinco);
	//选中原来的画笔  
	memory_dc.SelectObject(p_bmp);

	BITMAP bit;
	bmp.GetBitmap(&bit);
	
	//定义 图像大小（单位：byte）  
	DWORD size = bit.bmWidthBytes * bit.bmHeight;
	LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);

	//后面是创建一个bmp文件的必须文件头  
	BITMAPINFOHEADER pbitinfo;
	pbitinfo.biBitCount = 24;
	pbitinfo.biClrImportant = 0;
	pbitinfo.biCompression = BI_RGB;
	pbitinfo.biHeight = bit.bmHeight;
	pbitinfo.biPlanes = 1;
	pbitinfo.biSize = sizeof(BITMAPINFOHEADER);
	pbitinfo.biSizeImage = size;
	pbitinfo.biWidth = bit.bmWidth;
	pbitinfo.biXPelsPerMeter = 0;
	pbitinfo.biYPelsPerMeter = 0;

	GetDIBits(m_dc->m_hDC, bmp, 0, pbitinfo.biHeight, lpdata, (BITMAPINFO*)
		&pbitinfo, DIB_RGB_COLORS);



	BITMAPFILEHEADER bfh;
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = size + 54;
	bfh.bfOffBits = 54;

	//写入文件  
	CFile file;
	CString strFileName(GetAppPathW().c_str());
	strFileName += _T("ScreenShot\\");
	CreateDirectory((LPCTSTR)strFileName, NULL);
	CTime t = CTime::GetCurrentTime();
	CString tt = t.Format("%Y-%m-%d_%H-%M-%S");
	strFileName += tt;
	strFileName += _T(".bmp");
	if (file.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(&bfh, sizeof(BITMAPFILEHEADER));
		file.Write(&pbitinfo, sizeof(BITMAPINFOHEADER));
		file.Write(lpdata, size);
		file.Close();
	}
	GlobalFree(lpdata);
}
