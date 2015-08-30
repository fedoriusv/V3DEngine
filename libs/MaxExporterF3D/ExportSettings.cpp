#include "ExportSettings.h"

using namespace v3d;

CSettings::CSettings()
: m_exportNormals(true)
, m_exportBinormals(false)
, m_exportTangents(false)
, m_exportColors(false)
, m_exportTexCoords(true)

, m_exportObjectSpace(false)
, m_exportSelected(false)
, m_exportMaterials(true)
, m_exportLights(true)
, m_exportCameras(true)
{
}

CSettings::~CSettings()
{
}

void CSettings::setExportNormals(bool exported)
{
    m_exportNormals = exported;
}

void CSettings::setExportBinormals(bool exported)
{
    m_exportBinormals = exported;
}

void CSettings::setExportTangents(bool exported)
{
    m_exportTangents = exported;
}

void CSettings::setExportColors(bool exported)
{
    m_exportColors = exported;
}

void CSettings::setExportTexCoords(bool exported)
{
    m_exportTexCoords = exported;
}

void CSettings::setExportObjectSpace(bool exported)
{
    m_exportObjectSpace = exported;
}

void CSettings::setExportSelected(bool exported)
{
    m_exportSelected = exported;
}

void CSettings::setExportMaterials(bool exported)
{
    m_exportMaterials = exported;
}

void CSettings::setExportLights(bool exported)
{
    m_exportLights = exported;
}

void CSettings::setExportCameras(bool exported)
{
    m_exportCameras = exported;
}

bool CSettings::isExportNormals() const
{
    return m_exportNormals;
}

bool CSettings::isExportBinormals() const
{
    return m_exportBinormals;
}

bool CSettings::isExportTangents() const
{
    return m_exportTangents;
}

bool CSettings::isExportColors() const
{
    return m_exportColors;
}

bool CSettings::isExportTexCoords() const
{
    return m_exportTexCoords;
}

bool CSettings::isExportObjectSpace() const
{
    return m_exportObjectSpace;
}

bool CSettings::isExportSelected() const
{
    return m_exportSelected;
}

bool CSettings::isExportMaterials() const
{
    return m_exportMaterials;
}

bool CSettings::isExportLights() const
{
    return m_exportLights;
}

bool CSettings::isExportCameras() const
{
    return m_exportCameras;
}
