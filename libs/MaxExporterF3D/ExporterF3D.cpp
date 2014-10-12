#include "ExporterF3D.h"
#include "ClassDesc.h"
#include "Logger.h"

#include "IGame.h"
#include "IGameObject.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "3dsmaxport.h"

#define BEZIER	0
#define TCB		1
#define LINEAR	2
#define SAMPLE	3

using namespace v3d;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassDesc2* GetExporterDesc() 
{
    static ExporterClassDesc ExporterDesc;
    return &ExporterDesc; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ExporterF3DErrorCallback errorCallback;

void ExporterF3DErrorCallback::ErrorProc(IGameError error)
{
    const TCHAR * buf = GetLastIGameErrorText();
    LOG_INFO("ErrorCode = %d ErrorText = %s", error, buf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ExporterF3D::ExporterF3D()
    : m_iGameVersion(0.0f)
    , m_iGameScene(nullptr)

    , m_exportSelected(false)
    , m_exportAsGeometry(false)
    , m_exportObjectSpace(false)
    , m_exportMaterials(false)
    , m_exportLights(false)
{
}

ExporterF3D::~ExporterF3D()
{
}

int ExporterF3D::ExtCount()
{
    //TODO: Returns the number of file name extensions supported by the plug-in.
    return 1;
}

const TCHAR* ExporterF3D::Ext(int n)
{
    //TODO: Return the 'i-th' file name extension (i.e. "3DS").
    return _T("f3d");
}

const TCHAR* ExporterF3D::LongDesc()
{
    //TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
    return _T("F3D Exporter Model");
}

const TCHAR* ExporterF3D::ShortDesc()
{
    //TODO: Return short ASCII description (i.e. "Targa")
    return _T("ExporterF3D");
}

const TCHAR* ExporterF3D::AuthorName()
{
    //TODO: Return ASCII Author name
    return _T("Fedor Nekrasov");
}

const TCHAR* ExporterF3D::CopyrightMessage()
{
    // Return ASCII Copyright message
    return _T("Copyright 2014");
}

const TCHAR* ExporterF3D::OtherMessage1()
{
    //TODO: Return Other message #1 if any
    return _T("");
}

const TCHAR* ExporterF3D::OtherMessage2()
{
    //TODO: Return other message #2 in any
    return _T("");
}

unsigned int ExporterF3D::Version()
{
    //TODO: Return Version number * 100 (i.e. v3.01 = 301)
    return m_exporterVersion * 100;
}

void ExporterF3D::ShowAbout(HWND hWnd)
{
    // Optional
}

BOOL ExporterF3D::SupportsOptions(int ext, DWORD options)
{
    // TODO Decide which options to support.  Simply return
    // true for each option supported by each Extension 
    // the exporter supports.

    return TRUE;
}

int ExporterF3D::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
#if USE_CONSOLE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    char mbstr[128];
    std::wcstombs(mbstr, name, 128);
    
    LOG_INFO("Start Export file %s", mbstr);

    Interface* ip = GetCOREInterface();
    SetErrorCallBack(&errorCallback);

    m_iGameVersion = GetIGameVersion();
    LOG_INFO("IGameVersion %f", m_iGameVersion);

    if (DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), i->GetMAXHWnd(), &ExporterF3DOptionsDlgProc, (LPARAM)this) != IDOK)
    {
        LOG_ERROR("Error Create DialogBox. Exit");
#if USE_CONSOLE
        fclose(stdout);
        FreeConsole();
#endif

        return 1;
    }

    m_iGameScene = GetIGameInterface();
    m_iGameScene->InitialiseIGame(m_exportSelected);

    IGameConversionManager* cm = GetConversionManager();
    cm->SetCoordSystem(IGameConversionManager::IGAME_USER); // IGAME_OGL); // IGAME_D3D);

    UserCoord userCoordSystem;
    userCoordSystem.rotation = 1;
    userCoordSystem.xAxis = 0; // 1
    userCoordSystem.yAxis = 3; // 2
    userCoordSystem.zAxis = 4; // 4
    userCoordSystem.uAxis = 1;
    userCoordSystem.vAxis = 0;
    cm->SetUserCoordSystem(userCoordSystem);
    cm->SetCoordSystem(IGameConversionManager::IGAME_USER);

    m_iGameScene->SetStaticFrame(0);

    EExportError success = ExporterF3D::CreateModel();

    m_iGameScene->ReleaseIGame();
    ip->ProgressEnd();

    if (success == eNoError)
    {
        MessageBox(NULL, _T("Finished Success"), _T("OK"), MB_OK);
    }
    else
    {
        std::wstring steError(L"", 256);
        wsprintf(&steError[0], L"Error Create Model: %d", success);
        MessageBox(NULL, steError.c_str(), _T("CLOSE"), MB_OK | MB_ICONERROR);
    }

#if USE_CONSOLE
    fclose(stdout);
    FreeConsole();
#endif

    return (success == eNoError) ? 1 : 0;
}

ExporterF3D::EExportError ExporterF3D::CreateModel()
{
    return eNoError;
}

INT_PTR CALLBACK ExporterF3D::ExporterF3DOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ExporterF3D* pExporter = (ExporterF3D*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {
        case WM_INITDIALOG:
        {
            pExporter = (ExporterF3D*)lParam;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);
            CenterWindow(hWnd, GetParent(hWnd));

            // set check box to default values
            CheckDlgButton(hWnd, IDC_EXPORT_GEOMETRY, pExporter->m_exportAsGeometry);
            CheckDlgButton(hWnd, IDC_EXPORT_IN_OBJECT_SPACE, pExporter->m_exportObjectSpace);
            CheckDlgButton(hWnd, IDC_EXPORT_SELECTED_ONLY, pExporter->m_exportSelected);
            CheckDlgButton(hWnd, IDC_EXPORT_MATERIALS, pExporter->m_exportMaterials);
            CheckDlgButton(hWnd, IDC_EXPORT_LIGHTS, pExporter->m_exportLights);

                std::wstring strWindowTitle(L"", 256);

                wsprintf(&strWindowTitle[0], L"Max ExporterF3D version %d (c) 2014 Fedor Nekrasov", pExporter->Version());
                SetWindowText(hWnd, strWindowTitle.c_str());
        }
        return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDC_EXPORT_GEOMETRY:
            case IDC_EXPORT_IN_OBJECT_SPACE:
            case IDC_EXPORT_SELECTED_ONLY:
            case IDC_EXPORT_MATERIALS:
            case IDC_EXPORT_LIGHTS:
                break;

            case IDOK:
                pExporter->m_exportAsGeometry = IsDlgButtonChecked(hWnd, IDC_EXPORT_GEOMETRY) ? true : false;
                pExporter->m_exportObjectSpace = IsDlgButtonChecked(hWnd, IDC_EXPORT_IN_OBJECT_SPACE) ? true : false;
                pExporter->m_exportSelected = IsDlgButtonChecked(hWnd, IDC_EXPORT_SELECTED_ONLY) ? true : false;
                pExporter->m_exportMaterials = IsDlgButtonChecked(hWnd, IDC_EXPORT_MATERIALS) ? true : false;
                pExporter->m_exportLights = IsDlgButtonChecked(hWnd, IDC_EXPORT_LIGHTS) ? true : false;
                EndDialog(hWnd, 1);
                break;

            case IDCANCEL:
                EndDialog(hWnd, 0);
                break;

            default:
                return FALSE;
            }

        default:
            return FALSE;
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
