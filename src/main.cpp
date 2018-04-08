#include <iostream>
#include <memory>

#define WINVER 0x0501  
#define _WIN32_WINNT 0x0501

#define _AFXDLL
#include <afxwin.h>

using namespace std;

void ScreenShot(void);


int main(int argc, char** argv)
{
	cout << "hello world!" << endl;

	ScreenShot();
	return 0;
}



