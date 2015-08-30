#ifndef _V3D_EXPORT_SETTINGS_H_
#define _V3D_EXPORT_SETTINGS_H_

namespace v3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSettings
    {
    public:

        CSettings();
        ~CSettings();

        void setExportNormals(bool exported);
        void setExportBinormals(bool exported);
        void setExportTangents(bool exported);
        void setExportColors(bool exported);
        void setExportTexCoords(bool exported);

        void setExportObjectSpace(bool exported);
        void setExportSelected(bool exported);
        void setExportMaterials(bool exported);
        void setExportLights(bool exported);
        void setExportCameras(bool exported);


        bool isExportNormals() const;
        bool isExportBinormals() const;
        bool isExportTangents() const;
        bool isExportColors() const;
        bool isExportTexCoords() const;

        bool isExportObjectSpace() const;
        bool isExportSelected() const;
        bool isExportMaterials() const;
        bool isExportLights() const;
        bool isExportCameras() const;

    private:

        //Geometry
        bool    m_exportNormals;
        bool    m_exportBinormals;
        bool    m_exportTangents;
        bool    m_exportColors;
        bool    m_exportTexCoords;

        //Environment
        bool    m_exportObjectSpace;
        bool    m_exportSelected;
        bool    m_exportMaterials;
        bool    m_exportLights;
        bool    m_exportCameras;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_V3D_EXPORT_SETTINGS_H_