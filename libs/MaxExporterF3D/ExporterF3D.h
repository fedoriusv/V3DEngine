#ifndef _EXPORTER_F3D_H_
#define _EXPORTER_F3D_H_

#include "common.h"
#include "ModelMetadata.h"

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "IGameError.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define USE_CONSOLE 1

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class IGameScene;
class IGameNode;

namespace v3d
{
namespace stream
{
    class MemoryStream;
}
}

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

    enum EExportError
    {
        eNoError = 0,
        eUnknownError,
        eSceneEmptyError,
        eNodeMeshError,
        eNodeLightError,
        eNodeCameraError,
        eGeometryError,
    };

    ExporterF3D();
    virtual                     ~ExporterF3D();

    int                         ExtCount();                 // Number of extensions supported
    const TCHAR*                Ext(int n);                 // Extension #n (i.e. "3DS")
    const TCHAR*                LongDesc();                 // Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR*                ShortDesc();                // Short ASCII description (i.e. "3D Studio")
    const TCHAR*                AuthorName();               // ASCII Author name
    const TCHAR*                CopyrightMessage();         // ASCII Copyright message
    const TCHAR*                OtherMessage1();            // Other message #1
    const TCHAR*                OtherMessage2();            // Other message #2
    unsigned int                Version();                  // Version number * 100 (i.e. v3.01 = 301)
    void                        ShowAbout(HWND hWnd);       // Show DLL's "About..." box

    BOOL                        SupportsOptions(int ext, DWORD options);
    int                         DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

    static INT_PTR CALLBACK     ExporterF3DOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK     TestOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    EExportError                CreateModel();
    EExportError                ExportNode(IGameNode* node, int index);

    bool                        ExportMesh(IGameNode* node, renderer::MeshPtr& mesh);
    bool                        ExportLight(IGameNode* node, scene::LightPtr& light);
    bool                        ExportCamera(IGameNode* node, scene::CameraPtr& camera);

    bool                        ExportMaterial(IGameMaterial* gameMaterial, renderer::MaterialPtr& material);

    core::Vector3D              convertPointToVector3(Point3& point);
    core::Vector2D              convertPointToVector2(Point2& point);

private:

    const float                 m_exporterVersion = 1.01f;

    IGameScene*                 m_iGameScene;
    float                       m_iGameVersion;

    bool                        m_exportSelected;
    bool                        m_exportAsGeometry;
    bool                        m_exportObjectSpace;
    bool                        m_exportMaterials;
    bool                        m_exportLights;
    bool                        m_exportCamera;

    v3d::stream::MemoryStream*  m_stream;

    v3d::CModelMetadata*        m_scene;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_EXPORTER_F3D_H_
