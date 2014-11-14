#include "ExporterF3D.h"
#include "ClassDesc.h"

#include "IGame.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "3dsmaxport.h"

//#include "stream/MemoryStream.h"
#include "utils/Logger.h"
#include "renderer/GL/GeometryGL.h"

using namespace v3d;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassDesc2* GetExporterDesc() 
{
    static ExporterClassDesc ExporterDesc;
    return &ExporterDesc; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TCHARToString(const TCHAR* str)
{
#ifdef _UNICODE
    std::string mbstr("", 265);
    std::wcstombs(&mbstr[0], str, 265);
#else
    std::string mbstr(str);
#endif //_UNICODE
    return mbstr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ExporterF3DErrorCallback errorCallback;

void ExporterF3DErrorCallback::ErrorProc(IGameError error)
{
    const TCHAR* buf = GetLastIGameErrorText();
    LOG_ERROR("ErrorCode = %d ErrorText = %s\n", error, TCHARToString(buf).c_str());
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
    , m_exportCamera(false)

    , m_scene(nullptr)
{
}

ExporterF3D::~ExporterF3D()
{
    if (m_scene)
    {
        delete m_scene;
        m_scene = nullptr;
    }
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

int ExporterF3D::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts, DWORD options)
{
#if USE_CONSOLE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    LOG_INFO("Start Export file [%s]", TCHARToString(name).c_str());

    Interface* ip = GetCOREInterface();
    SetErrorCallBack(&errorCallback);

    m_iGameVersion = GetIGameVersion();
    LOG_INFO("IGameVersion %f", m_iGameVersion);

    int err = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), i->GetMAXHWnd(), &ExporterF3D::ExporterF3DOptionsDlgProc, (LPARAM)this);
    if (err != IDOK)
    {
        if (err != FALSE)
        {
            LOG_ERROR("Error Create DialogBox %d", err);
            MessageBox(NULL, _T("Error Create DialogBox"), _T("CLOSE"), MB_OK | MB_ICONERROR);
        }
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

    if (success == eNoError && m_scene)
    {
        if (!m_scene->save(TCHARToString(name)))
        {
            success = eSaveError;
        }
    }
    LOG_INFO("Save Completed\n");

    if (success == eNoError)
    {
        LOG_INFO("Export Finished Success");
        MessageBox(NULL, _T("Finished Success"), _T("OK"), MB_OK);
    }
    else
    {
        LOG_ERROR("Error Export code %d", success);

        std::wstring steError(L"", 256);
        wsprintf(&steError[0], L"Error Export Model. Code %d", success);
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
    m_scene = new CSceneData();

    int nodeCount = m_iGameScene->GetTopLevelNodeCount();
    LOG_INFO("In Game Scene found [%d] Nodes\n", nodeCount);

    if (nodeCount == 0)
    {
        return eSceneEmptyError;
    }

    for (int objectIndex = 0; objectIndex < nodeCount; ++objectIndex)
    {
        IGameNode* gameNode = m_iGameScene->GetTopLevelNode(objectIndex);
        LOG_INFO("Processing Parent object [%d/%d:%s] ", objectIndex + 1, nodeCount, TCHARToString(gameNode->GetName()).c_str());

        EExportError error = ExportNode(gameNode, objectIndex);
        if (error != eNoError)
        {
            return error;
        }
    }

    return eNoError;
}

ExporterF3D::EExportError ExporterF3D::ExportNode(IGameNode* node, int index)
{
    IGameObject* gameObject = node->GetIGameObject();

    int numChildren = node->GetChildCount();
    LOG_INFO("Child count [%d]", numChildren);

    INode* maxNode = node->GetMaxNode();
    IGameObject::ObjectTypes objectType = gameObject->GetIGameType();

    ObjectPtr object = m_scene->createObject(objectType);
    if (!object)
    {
        LOG_WARNING("Export unknown type: %d", objectType);
        return eUnknownError;
    }

    object->setName(TCHARToString(node->GetName()));
    object->setID(node->GetNodeID());

    switch (objectType)
    {
        case IGameObject::IGAME_MESH:
        {
            LOG_INFO("Export IGAME_MESH: %s", TCHARToString(node->GetName()).c_str());
            renderer::MeshPtr mesh = std::static_pointer_cast<renderer::CMesh>(object);
            if (!ExportMesh(node, mesh))
            {
                return eNodeMeshError;
            }
        }
        break;

        case IGameObject::IGAME_LIGHT:
        {
            LOG_INFO("Export IGAME_LIGHT: %s", TCHARToString(node->GetName()).c_str());
            scene::LightPtr light = std::static_pointer_cast<scene::CLight>(object);
            if (!ExportLight(node, light))
            {
                return eNodeLightError;
            }
        }
        break;

        case IGameObject::IGAME_CAMERA:
        {
            LOG_INFO("Export IGAME_CAMERA: %s", TCHARToString(node->GetName()).c_str());
            scene::CameraPtr camera = std::static_pointer_cast<scene::CCamera>(object);
            if (!ExportCamera(node, camera))
            {
                return eNodeCameraError;
            }
        }
        break;

        default:
        {
            LOG_WARNING("Export unknown type: %d", objectType);
            return eUnknownError;
        }
    }

    m_scene->addObject(std::make_pair(objectType, object));

    for (int i = 0; i < numChildren; ++i)
    {
        LOG_INFO("Processing child [%d/%d:%s] ", i + 1, numChildren, TCHARToString(node->GetName()).c_str());
        IGameNode* childNode = node->GetNodeChild(i);

        EExportError error = ExportNode(childNode, i);
        if (error != eNoError)
        {
            return error;
        }
    }

    LOG_INFO("Export Node %s completed", TCHARToString(node->GetName()).c_str());

    return eNoError;
}

bool ExporterF3D::ExportMesh(IGameNode* node, renderer::MeshPtr& mesh)
{
    IGameMesh* gameMesh = (IGameMesh*)node->GetIGameObject();
    if (!gameMesh->InitializeData() || !gameMesh->IsRenderable())
    {
        LOG_WARNING("Skipping %s - The object is not renderable", TCHARToString((node->GetName())).c_str());
        return true;
    }

    /*gameMesh->SetCreateOptimizedNormalList();
    gameMesh->InitializeBinormalData();*/

    Box3 bbox; 
    gameMesh->GetBoundingBox(bbox);

    int numVerts = gameMesh->GetNumberOfVerts();
    int numNormals = gameMesh->GetNumberOfNormals();
    int numBinormals = gameMesh->GetNumberOfBinormals();
    int numTangents = gameMesh->GetNumberOfTangents();
    int numTexVerts = gameMesh->GetNumberOfTexVerts();
    int numColorVerts = gameMesh->GetNumberOfColorVerts();
    int numAlphaVerts = gameMesh->GetNumberOfAlphaVerts();

    LOG_INFO("Verts : %d", numVerts);
    LOG_INFO("Normals : %d", numNormals);
    LOG_INFO("Binormals : %d", numBinormals);
    LOG_INFO("Tangents : %d", numTangents);
    LOG_INFO("TexVerts : %d", numTexVerts);
    LOG_INFO("ColorVerts : %d", numColorVerts);
    LOG_INFO("AlphaVerts : %d", numAlphaVerts);

    Tab<int> materialList = gameMesh->GetActiveMatIDs();
    int materialCount = materialList.Count();
    LOG_INFO("materialCount : %d", materialCount);

    renderer::MaterialPtr& material = const_cast<renderer::MaterialPtr&>(mesh->getMaterial());
    renderer::GeometryPtr& geomerty = const_cast<renderer::GeometryPtr&>(mesh->getGeomerty());
    geomerty = std::make_shared<renderer::CGeometryGL>(nullptr);

    for (int i = 0; i < materialCount; ++i)
    {
        int matID = materialList[i];
        Tab<FaceEx*> faceList = gameMesh->GetFacesFromMatID(matID);
        int numFaces = faceList.Count();
        LOG_INFO("numFaces : %d", numFaces);

        IGameMaterial* sourceMaterial = gameMesh->GetMaterialFromFace(faceList[0]);
        if (ExporterF3D::ExportMaterial(sourceMaterial, material))
        {
            for (int j = 0; j < numFaces; ++j)
            {
                FaceEx* sourceFace = faceList[j];

                for (int k = 0; k < 3; ++k)
                {
                    //LOG_INFO("addIndex : %d", sourceFace->vert[k]);
                    geomerty->addIndex(sourceFace->vert[k]);

                    Point3 vertex = gameMesh->GetVertex(sourceFace->vert[k], m_exportObjectSpace);
                    //LOG_INFO("addVertex : x = %f, y = %f, z = %f", vertex.x, vertex.y, vertex.z);
                    geomerty->addVertex(convertPointToVector3(vertex));

                    Point3 normal = gameMesh->GetNormal(sourceFace->norm[k], m_exportObjectSpace).Normalize();
                    //LOG_INFO("addNormal : x = %f, y = %f, z = %f", normal.x, normal.y, normal.z);
                    geomerty->addNormal(convertPointToVector3(normal));

                    Point3 color = gameMesh->GetColorVertex(sourceFace->color[k]);

                    Point2 texCoord = gameMesh->GetTexVertex(sourceFace->texCoord[k]);
                    //LOG_INFO("addTexCoord : x = %f, y = %f\n", texCoord.x, texCoord.y);
                    geomerty->addTexCoord(0, convertPointToVector2(texCoord));
                }
            }
        }

    }

    return true;
}

bool ExporterF3D::ExportLight(IGameNode* node, scene::LightPtr& light)
{
    //TODO:
    return false;
}

bool ExporterF3D::ExportCamera(IGameNode* node, scene::CameraPtr& camera)
{
    //TODO:
    return false;
}

bool ExporterF3D::ExportMaterial(IGameMaterial* gameMaterial, renderer::MaterialPtr& material)
{
    //TODO:
    return true;
}

core::Vector3D ExporterF3D::convertPointToVector3(const Point3& point)
{
    return core::Vector3D(point.x, point.y, point.x);
}

core::Vector2D ExporterF3D::convertPointToVector2(const Point2& point)
{
    return core::Vector2D(point.x, point.y);
}



INT_PTR CALLBACK ExporterF3D::TestOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LOG_INFO("OptionsDlgProc");

    switch (message)
    {
    case WM_INITDIALOG:
        LOG_INFO("OptionsDlgProc WM_INITDIALOG");
        CenterWindow(hWnd, GetParent(hWnd));
        return TRUE;

    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return 1;
    }

    return 0;
}

INT_PTR CALLBACK ExporterF3D::ExporterF3DOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ExporterF3D* exporter = (ExporterF3D*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {
        case WM_INITDIALOG:
        {
            exporter = (ExporterF3D*)lParam;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);
            CenterWindow(hWnd, GetParent(hWnd));

            // set check box to default values
            CheckDlgButton(hWnd, IDC_EXPORT_GEOMETRY, exporter->m_exportAsGeometry);
            CheckDlgButton(hWnd, IDC_EXPORT_IN_OBJECT_SPACE, exporter->m_exportObjectSpace);
            CheckDlgButton(hWnd, IDC_EXPORT_SELECTED_ONLY, exporter->m_exportSelected);
            CheckDlgButton(hWnd, IDC_EXPORT_MATERIALS, exporter->m_exportMaterials);
            CheckDlgButton(hWnd, IDC_EXPORT_LIGHTS, exporter->m_exportLights);
            CheckDlgButton(hWnd, IDC_EXPORT_CAMERA, exporter->m_exportCamera);

            std::wstring strWindowTitle(L"", 256);

            wsprintf(&strWindowTitle[0], L"Max ExporterF3D version %d (c) 2014 Fedor Nekrasov", exporter->Version());
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
            case IDC_EXPORT_CAMERA:
                break;

            case IDOK:
                exporter->m_exportAsGeometry = IsDlgButtonChecked(hWnd, IDC_EXPORT_GEOMETRY) ? true : false;
                exporter->m_exportObjectSpace = IsDlgButtonChecked(hWnd, IDC_EXPORT_IN_OBJECT_SPACE) ? true : false;
                exporter->m_exportSelected = IsDlgButtonChecked(hWnd, IDC_EXPORT_SELECTED_ONLY) ? true : false;
                exporter->m_exportMaterials = IsDlgButtonChecked(hWnd, IDC_EXPORT_MATERIALS) ? true : false;
                exporter->m_exportLights = IsDlgButtonChecked(hWnd, IDC_EXPORT_LIGHTS) ? true : false;
                exporter->m_exportCamera = IsDlgButtonChecked(hWnd, IDC_EXPORT_CAMERA) ? true : false;
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
