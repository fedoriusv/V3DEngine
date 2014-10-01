/**********************************************************************
   ExporterF3D
   Copyright (c) 2014, Fred Nekrasov. All Rights Reserved.
 **********************************************************************/

#include "ClassDesc.h"

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
   if(fdwReason == DLL_PROCESS_ATTACH)
   {
      MaxSDK::Util::UseLanguagePackLocale();
      hInstance = hinstDLL;
      DisableThreadLibraryCalls(hInstance);
   }

   return (TRUE);
}

__declspec(dllexport) const TCHAR* LibDescription()
{
   return GetString(IDS_LIBDESCRIPTION);
}

//TODO: Must change this number when adding a new class
__declspec(dllexport) int LibNumberClasses()
{
   return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
   switch(i) 
   {
      case 0: 
          return GetExporterDesc();

      default: 
          return 0;
   }
}

__declspec(dllexport) ULONG LibVersion()
{
   return VERSION_3DSMAX;
}

// Let the plug-in register itself for deferred loading
__declspec(dllexport) ULONG CanAutoDefer()
{
   return 1;
}
