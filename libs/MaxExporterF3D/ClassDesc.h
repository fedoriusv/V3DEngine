#ifndef _CLASS_DESC_F3D__H_
#define _CLASS_DESC_F3D__H_

#include "ExporterF3D.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////

TCHAR *GetString(int id)
{
    static TCHAR buf[256];

    if (hInstance)
    {
        return LoadString(hInstance, id, buf, _countof(buf)) ? buf : NULL;
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IGAMEEXPORTER_CLASS_ID  Class_ID(0x79d613a4, 0x4f21c3ad)

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////

static ExporterClassDesc ExporterDesc;
ClassDesc2* GetExporterDesc() { return &ExporterDesc; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_CLASS_DESC_F3D__H_
