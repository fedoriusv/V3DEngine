#ifndef _EXPORTER_F3D_H_
#define _EXPORTER_F3D_H_

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "IGameError.h"

class IGameScene;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExporterF3DErrorCallback : public IGameErrorCallBack
{
public:
    ExporterF3DErrorCallback() {}
    virtual ~ExporterF3DErrorCallback() {}

    void    ErrorProc(IGameError error);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExporterF3D : public SceneExport 
{
public:

    ExporterF3D();
    virtual ~ExporterF3D();

    int             ExtCount();                 // Number of extensions supported
    const TCHAR*    Ext(int n);                 // Extension #n (i.e. "3DS")
    const TCHAR*    LongDesc();                 // Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR*    ShortDesc();                // Short ASCII description (i.e. "3D Studio")
    const TCHAR*    AuthorName();               // ASCII Author name
    const TCHAR*    CopyrightMessage();         // ASCII Copyright message
    const TCHAR*    OtherMessage1();            // Other message #1
    const TCHAR*    OtherMessage2();            // Other message #2
    unsigned int    Version();                  // Version number * 100 (i.e. v3.01 = 301)
    void            ShowAbout(HWND hWnd);       // Show DLL's "About..." box

    BOOL            SupportsOptions(int ext, DWORD options);
    int             DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

private:

    bool            CreateModel();

private:

    const float     m_exporterVersion = 1.01f;

    IGameScene*     m_iGameScene;
    float           m_iGameVersion;

    bool            m_exportSelected;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_EXPORTER_F3D_H_
