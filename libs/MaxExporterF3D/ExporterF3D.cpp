#include "ExporterF3D.h"
#include "ClassDesc.h"

#include "IGame.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "3dsmaxport.h"

#include "utils/Logger.h"
#include "scene/Node.h"
#include "scene/Mesh.h"
#include "scene/Camera.h"
#include "scene/Light.h"
#include "renderer/NULL/GeometryNull.h"

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
    size_t size = wcslen(str);
    std::string mbstr("", size);
    std::wcstombs(&mbstr[0], str, size);
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

    , m_scene(nullptr)
{
    m_settings = new CSettings();
}

ExporterF3D::~ExporterF3D()
{
    if (m_settings)
    {
        delete m_settings;
        m_settings = nullptr;
    }

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

    INT_PTR err = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), i->GetMAXHWnd(), &ExporterF3D::ExporterF3DOptionsDlgProc, (LPARAM)this);
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
    m_iGameScene->InitialiseIGame(m_settings->isExportSelected());

    IGameConversionManager* cm = GetConversionManager();
    cm->SetCoordSystem(IGameConversionManager::IGAME_USER); // IGAME_OGL); // IGAME_D3D);

    UserCoord userCoordSystem;
    userCoordSystem.rotation = 1;
    userCoordSystem.xAxis = 0; // 1
    userCoordSystem.yAxis = 3; // 2
    userCoordSystem.zAxis = 5; // 4
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
        if (!m_scene->save(TCHARToString(name), m_exporterVersion))
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

v3d::CSettings* ExporterF3D::getExportSettings() const
{
    return m_settings;
}

ExporterF3D::EExportError ExporterF3D::CreateModel()
{
    m_scene = new CSceneData();

    s32 nodeCount = m_iGameScene->GetTopLevelNodeCount();
    LOG_INFO("In Game Scene found [%d] Nodes", nodeCount);

    if (nodeCount == 0)
    {
        return eSceneEmptyError;
    }

    s32 id = 0;
    m_scene->setId(id);
    m_scene->setName("");
    LOG_INFO("Game Scene Name %s, id %d", TCHARToString(m_iGameScene->GetSceneFileName()).c_str(), id);

    for (u32 objectIndex = 0; objectIndex < nodeCount; ++objectIndex)
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

ExporterF3D::EExportError ExporterF3D::ExportNode(IGameNode* node, u32 index, scene::CNode* parent)
{
    IGameObject* gameObject = node->GetIGameObject();

    int numChildren = node->GetChildCount();
    LOG_INFO("Child count [%d]", numChildren);

    INode* maxNode = node->GetMaxNode();
    IGameObject::ObjectTypes objectType = gameObject->GetIGameType();
    
    if (!m_settings->isExportLights() && objectType == IGameObject::IGAME_LIGHT)
    {
        LOG_WARNING("Skip export Ligth: %s", TCHARToString(node->GetName()).c_str());
        return eNoError;
    }
    else if (!m_settings->isExportCameras() && objectType == IGameObject::IGAME_CAMERA)
    {
        LOG_WARNING("Skip export Camera: %s", TCHARToString(node->GetName()).c_str());
        return eNoError;
    }

    scene::CNode* object = m_scene->createNode(objectType);
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
            scene::CMesh* mesh = static_cast<scene::CMesh*>(object);
            if (!ExportMesh(node, mesh))
            {
                delete object;
                object = nullptr;

                return eNodeMeshError;
            }
        }
        break;

        case IGameObject::IGAME_LIGHT:
        {
            LOG_INFO("Export IGAME_LIGHT: %s", TCHARToString(node->GetName()).c_str());
            scene::CLight* light = static_cast<scene::CLight*>(object);
            if (!ExportLight(node, light))
            {
                delete object;
                object = nullptr;

                return eNodeLightError;
            }
        }
        break;

        case IGameObject::IGAME_CAMERA:
        {
            LOG_INFO("Export IGAME_CAMERA: %s", TCHARToString(node->GetName()).c_str());
            scene::CCamera* camera = static_cast<scene::CCamera*>(object);
            if (!ExportCamera(node, camera))
            {
                delete object;
                object = nullptr;

                return eNodeCameraError;
            }
        }
        break;

        default:
        {
            delete object;
            object = nullptr;

            LOG_WARNING("Export unknown type: %d", objectType);
            return eUnknownError;
        }
    }

    if (parent)
    {
        parent->attachChild(object);
    }

    m_scene->addNode(std::make_pair(objectType, object));

    for (u32 i = 0; i < numChildren; ++i)
    {
        LOG_INFO("Processing child [%d/%d:%s] ", i + 1, numChildren, TCHARToString(node->GetName()).c_str());
        IGameNode* childNode = node->GetNodeChild(i);

        EExportError error = ExportNode(childNode, i, object);
        if (error != eNoError)
        {
            return error;
        }
    }

    LOG_INFO("Export Node %s completed\n", TCHARToString(node->GetName()).c_str());

    return eNoError;
}

bool ExporterF3D::ExportMesh(IGameNode* node, scene::CMesh* mesh)
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

    s32 numVerts = gameMesh->GetNumberOfVerts();
    s32 numNormals = gameMesh->GetNumberOfNormals();
    s32 numBinormals = gameMesh->GetNumberOfBinormals();
    s32 numTangents = gameMesh->GetNumberOfTangents();
    s32 numTexVerts = gameMesh->GetNumberOfTexVerts();
    s32 numColorVerts = gameMesh->GetNumberOfColorVerts();
    s32 numAlphaVerts = gameMesh->GetNumberOfAlphaVerts();

    LOG_INFO("Verts : %d", numVerts);
    LOG_INFO("Normals : %d", numNormals);
    LOG_INFO("Binormals : %d", numBinormals);
    LOG_INFO("Tangents : %d", numTangents);
    LOG_INFO("TexVerts : %d", numTexVerts);
    LOG_INFO("ColorVerts : %d", numColorVerts);
    LOG_INFO("AlphaVerts : %d", numAlphaVerts);

    Tab<s32> materialList = gameMesh->GetActiveMatIDs();
    int materialCount = materialList.Count();
    LOG_INFO("MaterialCount : %d", materialCount);

    renderer::MaterialPtr& material = const_cast<renderer::MaterialPtr&>(mesh->getMaterial());
    renderer::GeometryPtr& geomerty = const_cast<renderer::GeometryPtr&>(mesh->getGeometry());
    geomerty = std::make_shared<renderer::CGeometryNull>(nullptr);

    std::vector<s32> indexList;

    for (u32 i = 0; i < materialCount; ++i)
    {
        s32 matID = materialList[i];
        Tab<FaceEx*> faceList = gameMesh->GetFacesFromMatID(matID);
        s32 numFaces = faceList.Count();
        LOG_INFO("numFaces : %d", numFaces);

        IGameMaterial* sourceMaterial = gameMesh->GetMaterialFromFace(faceList[0]);
        if (ExporterF3D::ExportMaterial(sourceMaterial, material))
        {
            for (u32 j = 0; j < numFaces; ++j)
            {
                FaceEx* sourceFace = faceList[j];

                for (u32 k = 0; k < 3; ++k)
                {
                    LOG_GEBUG("addIndex : %d", sourceFace->vert[k]);
                    geomerty->addIndex(sourceFace->vert[k]);

                    if (std::find(indexList.cbegin(), indexList.cend(), sourceFace->vert[k]) == indexList.cend())
                    {
                        Point3 vertex = gameMesh->GetVertex(sourceFace->vert[k], m_settings->isExportObjectSpace());
                        LOG_GEBUG("addVertex : (%f, %f, %f)", vertex.x, vertex.y, vertex.z);
                        geomerty->addVertex(convertPointToVector3(vertex));

                        if (m_settings->isExportNormals())
                        {
                            Point3 normal = gameMesh->GetNormal(sourceFace->norm[k], m_settings->isExportObjectSpace()).Normalize();
                            LOG_GEBUG("addNormal : (%f, %f, %f)", normal.x, normal.y, normal.z);
                            geomerty->addNormal(convertPointToVector3(normal));
                        }

                        if (m_settings->isExportBinormals())
                        {
                            s32 binormalTangentIndex = gameMesh->GetFaceVertexTangentBinormal(j, k);
                            Point3 binormal = gameMesh->GetBinormal(binormalTangentIndex).Normalize();
                            LOG_GEBUG("addBinormal : (%f, %f, %f)", binormal.x, binormal.y, binormal.z);
                            geomerty->addBinormal(convertPointToVector3(binormal));
                        }

                        if (m_settings->isExportTangents())
                        {
                            s32 binormalTangentIndex = gameMesh->GetFaceVertexTangentBinormal(j, k);
                            Point3 tangent = gameMesh->GetTangent(binormalTangentIndex).Normalize();
                            LOG_GEBUG("addTangent : (%f, %f, %f)", tangent.x, tangent.y, tangent.z);
                            geomerty->addTangent(convertPointToVector3(tangent));
                        }

                        if (m_settings->isExportColors())
                        {
                            Point3 color = gameMesh->GetColorVertex(sourceFace->color[k]);
                            core::Vector3D col = convertPointToVector3(color);
                            col.x = core::abs(col.x);
                            col.y = core::abs(col.y);
                            col.z = core::abs(col.z);
                            LOG_GEBUG("addColor : (%f, %f, %f)", col.x, col.y, col.z);
                            geomerty->addColor(col);
                        }

                        if (m_settings->isExportTexCoords())
                        {
                            u32 layer = 0;
                            Point2 texCoord = gameMesh->GetTexVertex(sourceFace->texCoord[k]);
                            //TODO: tiling factor * texCoord
                            //TODO: texcoord layers
                            LOG_GEBUG("addTexCoord : (%f, %f)", texCoord.x, texCoord.y);
                            geomerty->addTexCoord(layer, convertPointToVector2(texCoord));
                        }

                        indexList.push_back(sourceFace->vert[k]);
                    }
                }
            }
        }
    }

    return true;
}

bool ExporterF3D::ExportLight(IGameNode* node, scene::CLight* light)
{
    //TODO:
    return false;
}

bool ExporterF3D::ExportCamera(IGameNode* node, scene::CCamera* camera)
{
    //TODO:
    return false;
}

bool ExporterF3D::ExportMaterial(IGameMaterial* gameMaterial, renderer::MaterialPtr& material)
{
    if (m_settings->isExportMaterials())
    {
        return true;
    }

    //TODO:
    return true;
}

core::Vector3D ExporterF3D::convertPointToVector3(const Point3& point)
{
    return core::Vector3D(core::round(point.x, 6), core::round(point.y, 6), core::round(point.z, 6));
}

core::Vector2D ExporterF3D::convertPointToVector2(const Point2& point)
{
    return core::Vector2D(core::round(point.x), core::round(point.y));
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
            v3d::CSettings* settings = exporter->getExportSettings();
            CheckDlgButton(hWnd, IDC_EXPORT_IN_OBJECT_SPACE, settings->isExportObjectSpace());
            CheckDlgButton(hWnd, IDC_EXPORT_SELECTED_ONLY, settings->isExportSelected());
            CheckDlgButton(hWnd, IDC_EXPORT_MATERIALS, settings->isExportMaterials());
            CheckDlgButton(hWnd, IDC_EXPORT_LIGHTS, settings->isExportLights());
            CheckDlgButton(hWnd, IDC_EXPORT_CAMERA, settings->isExportCameras());

            CheckDlgButton(hWnd, IDC_EXPORT_NORMAL, settings->isExportNormals());
            CheckDlgButton(hWnd, IDC_EXPORT_BINORMAL, settings->isExportBinormals());
            CheckDlgButton(hWnd, IDC_EXPORT_TANGENT, settings->isExportTangents());
            CheckDlgButton(hWnd, IDC_EXPORT_COLOR, settings->isExportColors());
            CheckDlgButton(hWnd, IDC_EXPORT_TEXCOORD, settings->isExportTexCoords());

            std::wstring strWindowTitle(L"", 256);

            wsprintf(&strWindowTitle[0], L"Max ExporterF3D version %d (c) 2014 Fedor Nekrasov", exporter->Version());
            SetWindowText(hWnd, strWindowTitle.c_str());
        }
        return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDC_EXPORT_IN_OBJECT_SPACE:
            case IDC_EXPORT_SELECTED_ONLY:
            case IDC_EXPORT_MATERIALS:
            case IDC_EXPORT_LIGHTS:
            case IDC_EXPORT_CAMERA:
                break;

            case IDOK:
            {
                v3d::CSettings* settings = exporter->getExportSettings();

                settings->setExportObjectSpace(IsDlgButtonChecked(hWnd, IDC_EXPORT_IN_OBJECT_SPACE) ? true : false);
                settings->setExportSelected(IsDlgButtonChecked(hWnd, IDC_EXPORT_SELECTED_ONLY) ? true : false);
                settings->setExportMaterials(IsDlgButtonChecked(hWnd, IDC_EXPORT_MATERIALS) ? true : false);
                settings->setExportLights(IsDlgButtonChecked(hWnd, IDC_EXPORT_LIGHTS) ? true : false);
                settings->setExportCameras(IsDlgButtonChecked(hWnd, IDC_EXPORT_CAMERA) ? true : false);

                settings->setExportNormals(IsDlgButtonChecked(hWnd, IDC_EXPORT_NORMAL) ? true : false);
                settings->setExportBinormals(IsDlgButtonChecked(hWnd, IDC_EXPORT_BINORMAL) ? true : false);
                settings->setExportTangents(IsDlgButtonChecked(hWnd, IDC_EXPORT_TANGENT) ? true : false);
                settings->setExportTexCoords(IsDlgButtonChecked(hWnd, IDC_EXPORT_TEXCOORD) ? true : false);
                settings->setExportColors(IsDlgButtonChecked(hWnd, IDC_EXPORT_COLOR) ? true : false);

                EndDialog(hWnd, 1);
            }
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
