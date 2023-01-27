#if RELEASE && defined(_MSC_VER)
extern int mainFunction();

#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return mainFunction();
}
#endif
