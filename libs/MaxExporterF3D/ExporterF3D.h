#ifndef _EXPORTER_F3D_H_
#define _EXPORTER_F3D_H_

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

class IGameScene;

class ExporterF3D : public SceneExport 
{
public:

    ExporterF3D();
    virtual ~ExporterF3D();

    int             ExtCount();					// Number of extensions supported
    const TCHAR*    Ext(int n);					// Extension #n (i.e. "3DS")
    const TCHAR*    LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR*    ShortDesc();				// Short ASCII description (i.e. "3D Studio")
    const TCHAR*    AuthorName();				// ASCII Author name
    const TCHAR*    CopyrightMessage();			// ASCII Copyright message
    const TCHAR*    OtherMessage1();			// Other message #1
    const TCHAR*    OtherMessage2();			// Other message #2
    unsigned int    Version();					// Version number * 100 (i.e. v3.01 = 301)
    void            ShowAbout(HWND hWnd);		// Show DLL's "About..." box

    BOOL            SupportsOptions(int ext, DWORD options);
    int             DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

private:

    const float     m_exporterVersion = 1.01f;

    IGameScene*     m_igame;
//
//    CCoInitialize init;              //must be declared before any IXMLDOM objects
//    CComPtr<IXMLDOMDocument>  pXMLDoc;
//    CComPtr<IXMLDOMNode> pRoot;		//this is our root node 	
//    CComPtr<IXMLDOMNode> iGameNode;	//the IGame child - which is the main node
//    CComPtr<IXMLDOMNode> rootNode;
//    static HWND hParams;
//
//
//    int curNode;
//
//    int staticFrame;
//    int framePerSample;
//    BOOL exportGeom;
//    BOOL exportNormals;
//    BOOL exportVertexColor;
//    BOOL exportControllers;
//    BOOL exportFaceSmgp;
//    BOOL exportTexCoords;
//    BOOL exportMappingChannel;
//    BOOL exportConstraints;
//    BOOL exportMaterials;
//    BOOL exportSplines;
//    BOOL exportModifiers;
//    BOOL exportSkin;
//    BOOL exportGenMod;
//    BOOL forceSample;
//    BOOL splitFile;
//    BOOL exportQuaternions;
//    BOOL exportObjectSpace;
//    BOOL exportRelative;
//    BOOL exportNormalsPerFace;
//    int cS;
//    int exportCoord;
//    bool showPrompts;
//    bool exportSelected;
//
//    TSTR fileName;
//    TSTR splitPath;
//
//    float igameVersion, exporterVersion;

//    CComPtr <IXMLDOMNode> ExportSceneInfo(CComPtr<IXMLDOMDocument> pXMLDOMDoc, CComPtr<IXMLDOMNode>  pDOMRoot);
//    void ExportNodeInfo(IGameNode * node);
//    void ExportChildNodeInfo(CComPtr<IXMLDOMNode> parent, IGameNode * child);
//    void ExportMaterials();
//    void ExportPositionControllers(CComPtr<IXMLDOMNode> node, IGameControl * cont, Point3);
//    void ExportRotationControllers(CComPtr<IXMLDOMNode> node, IGameControl * cont);
//    void ExportScaleControllers(CComPtr<IXMLDOMNode> node, IGameControl * cont);
//    void ExportControllers(CComPtr<IXMLDOMNode> node, IGameControl * cont);
//
//    void DumpMaterial(CComPtr<IXMLDOMNode> node, IGameMaterial * mat, int index, int matID = -1);
//    void DumpTexture(CComPtr<IXMLDOMNode> node, IGameMaterial * mat);
//    void DumpBezierKeys(IGameControlType Type, IGameKeyTab Keys, CComPtr<IXMLDOMNode> prsData);
//    void DumpTCBKeys(IGameControlType Type, IGameKeyTab Keys, CComPtr<IXMLDOMNode> prsData);
//    void DumpLinearKeys(IGameControlType Type, IGameKeyTab Keys, CComPtr<IXMLDOMNode> prsData);
//    void DumpBezierFloatKeys(IGameControlType Type, IGameKeyTab, int, CComPtr<IXMLDOMNode>);
//    void DumpBezierPosKeys(IGameControlType, IGameKeyTab, Point3, CComPtr<IXMLDOMNode>);
//    void DumpBezierRotKeys(IGameKeyTab, CComPtr<IXMLDOMNode>);
//    void DumpBezierSclKeys(IGameKeyTab, CComPtr<IXMLDOMNode>);
//    void DumpConstraints(CComPtr<IXMLDOMNode> prsData, IGameConstraint * c);
//    void DumpModifiers(CComPtr<IXMLDOMNode> prsData, IGameModifier * m);
//    void DumpSkin(CComPtr<IXMLDOMNode> modNode, IGameSkin * s);
//    void DumpIKChain(IGameIKChain * ikch, CComPtr<IXMLDOMNode> ikData);
//
//    void DumpEulerController(IGameControl * sc, CComPtr<IXMLDOMNode> prsNode);
//    void DumpIndePositionController(IGameControl * sc, CComPtr<IXMLDOMNode>, Point3);
//    void DumpProperties(CComPtr<IXMLDOMNode> node, IGameProperty * prop);
//    void DumpNamedProperty(CComPtr<IXMLDOMNode>, TCHAR*, IGameProperty*);
//    void DumpProperty(IGameProperty*, CComPtr<IXMLDOMNode>);
//
//
//    void DumpMesh(IGameMesh *gm, CComPtr<IXMLDOMNode> geomData);
//    void DumpSpline(IGameSpline *sp, CComPtr<IXMLDOMNode> splineData);
//    void DumpLight(IGameLight *lt, CComPtr<IXMLDOMNode> parent);
//    void DumpCamera(IGameCamera *ca, CComPtr<IXMLDOMNode> parent);
//    void DumpSampleKeys(IGameControl * sc, CComPtr<IXMLDOMNode> prsNode, IGameControlType Type, bool quick = false);
//    void DumpListController(IGameControl * sc, CComPtr<IXMLDOMNode> listNode);
//    void DumpMatrix(Matrix3 tm, CComPtr<IXMLDOMNode> parent);
//
//
//    void MakeSplitFilename(IGameNode * node, TSTR & buf);
//    void makeValidURIFilename(TSTR&, bool = false);
//    BOOL ReadConfig();
//    void WriteConfig();
//    TSTR GetCfgFilename();
};

#endif //_EXPORTER_F3D_H_
