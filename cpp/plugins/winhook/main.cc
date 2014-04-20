// main.cc
// 1/27/2013
// Restrict from accessing QObject in this file, and make sure the instance to start with wintimer.

#include "winhook/main.h"
#include "winhook/util/wingui.h"
#include "wintimer/wintimer.h"
#include "singleapp/singleapp.h"
#include "cc/ccmacro.h"

//#include "hijack/mygdi.h"

// - Main -

BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
  CC_UNUSED(lpvReserved);
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    if (!::single_app())
      return FALSE;

    //while(!::GetModuleHandleA("gdi32.dll"))
    //  ::Sleep(200);
    //while(!::GetModuleHandleA("d3d9.dll"))
    //  ::Sleep(200);

    ::DisableThreadLibraryCalls(hInstance); // Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications

    //::CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(d3dhook), nullptr, 0, nullptr);

    WinTimer::setGlobalWindow(
      wingui::GetAnyWindowWithProcessId(
        ::GetCurrentProcessId()));

    WinTimer::singleShot(1000, boost::bind(Main::initWithInstance, hInstance));

    //My::OverrideGDIModules();
    break;

  case DLL_PROCESS_DETACH:
    Main::destroy();
    WinTimer::singleShot(5000, []() { // If hang, terminate the process in 5 seconds.
      ::TerminateProcess(::GetCurrentProcess(), EXIT_SUCCESS);
    });
    break;
  }
  //SK_ASSERT(0);
  return TRUE;
}

// EOF
