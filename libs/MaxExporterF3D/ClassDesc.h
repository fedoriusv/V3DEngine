#ifndef _CLASS_DESC_F3D__H_
#define _CLASS_DESC_F3D__H_

#include "ExporterF3D.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IGAMEEXPORTER_CLASS_ID  Class_ID(0x5f1b7931, 0xc7c65e4)

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExporterClassDesc :public ClassDesc2 
{

public:
    int             IsPublic()                      { return TRUE; }
    void*           Create(BOOL loading = FALSE)    { return new ExporterF3D(); }
    const TCHAR*    ClassName()                     { return GetString(IDS_CLASS_NAME); }
    SClass_ID       SuperClassID()                  { return SCENE_EXPORT_CLASS_ID; }
    Class_ID        ClassID()                       { return IGAMEEXPORTER_CLASS_ID; }
    const TCHAR*    Category()                      { return GetString(IDS_CATEGORY); }
    const TCHAR*    InternalName()                  { return _T("ExporterF3D"); }
    HINSTANCE       HInstance()                     { return hInstance; }
};

ClassDesc2* GetExporterDesc();

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_CLASS_DESC_F3D__H_
