#ifndef _EXPORTER_F3D_H_
#define _EXPORTER_F3D_H_

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "IGameError.h"

#include "SceneData.h"
#include "ExportSettings.h"

#define USE_CONSOLE 1

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class IGameScene;
class IGameNode;

class ExporterF3DErrorCallback : public IGameErrorCallBack
{
public:
    ExporterF3DErrorCallback() {}
    virtual ~ExporterF3DErrorCallback() {}

    void    ErrorProc(IGameError error);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace v3d
{

    class ExporterF3D : public SceneExport
    {
    public:

        enum EExportError
        {
            eNoError = 0,
            eUnknownError,
            eSaveError,
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

        CSettings*                  getExportSettings() const;

    private:

        EExportError                CreateModel();
        EExportError                ExportNode(IGameNode* node, u32 index, scene::CNode* parent = nullptr);

        bool                        ExportMesh(IGameNode* node, scene::CMesh* mesh);
        bool                        ExportLight(IGameNode* node, scene::CLight* light);
        bool                        ExportCamera(IGameNode* node, scene::CCamera* camera);
        bool                        ExportMaterial(IGameMaterial* gameMaterial, renderer::MaterialPtr& material, STileUV& tile);

        core::Vector4D              convertPointToVector4(const Point4& point);
        core::Vector3D              convertPointToVector3(const Point3& point);
        core::Vector2D              convertPointToVector2(const Point2& point);
        s32                         findVertex(const renderer::GeometryPtr& geometry, const Point3& vertex);

        std::string                 getBitmapNameWithoutPath(const std::string& name);

    private:

        const f32                   k_exporterVersion = 1.0f;
        const s32                   k_decimalRound = 6;

        IGameScene*                 m_iGameScene;
        f32                         m_iGameVersion;

        CSceneData*                 m_scene;
        CSettings*                  m_settings;
    };
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_EXPORTER_F3D_H_
