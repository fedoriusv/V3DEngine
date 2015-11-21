#include "Vertex.h"

using namespace v3d;
using namespace core;

void calcTriangleBasis(const Vector3D& E, const Vector3D& F, const Vector3D& G, float sE,
    float tE, float sF, float tF, float sG, float tG, Vector3D& tangentX, Vector3D& tangentY)
{
    Vector3D P = F - E;
    Vector3D Q = G - E;

    f32 s1 = sF - sE;
    f32 t1 = tF - tE;
    f32 s2 = sG - sE;
    f32 t2 = tG - tE;

    f32 pqMatrix[2][3];
    pqMatrix[0][0] = P[0];
    pqMatrix[0][1] = P[1];
    pqMatrix[0][2] = P[2];
    pqMatrix[1][0] = Q[0];
    pqMatrix[1][1] = Q[1];
    pqMatrix[1][2] = Q[2];

    f32 temp = 1.0f / (s1 * t2 - s2 * t1);

    f32 stMatrix[2][2];
    stMatrix[0][0] = t2 * temp;
    stMatrix[0][1] = -t1 * temp;
    stMatrix[1][0] = -s2 * temp;
    stMatrix[1][1] = s1 * temp;

    f32 tbMatrix[2][3];
    // stMatrix * pqMatrix
    tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
    tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
    tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
    tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
    tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
    tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];

    tangentX = Vector3D(tbMatrix[0][0], tbMatrix[0][1], tbMatrix[0][2]);
    tangentY = Vector3D(tbMatrix[1][0], tbMatrix[1][1], tbMatrix[1][2]);
    tangentX.normalize();
    tangentY.normalize();
}

Vector3D closestPointOnLine(const Vector3D& a, const Vector3D& b, const Vector3D& p)
{
    Vector3D c = p - a;
    Vector3D V = b - a;
    f32 d = V.length();
    V.normalize();

    f32 t = dotProduct(V, c);

    if (t < 0.0f)
    {
        return a;
    }

    if (t > d)
    {
        return b;
    }

    V *= t;

    return (a + V);
}

Vector3D orthogonalize(const Vector3D& v1, const Vector3D& v2)
{
    Vector3D v2ProjV1 = closestPointOnLine(v1, -v1, v2);
    Vector3D res = v2 - v2ProjV1;
    res.normalize();

    return res;
}

SVertexData::SVertexData()
{
}

void SVertexData::clear()
{
    _vertices.clear();
    _colors.clear();
    _normals.clear();
    _binormals.clear();
    _tangents.clear();

    for (auto& layer : _texCoords)
    {
        layer.clear();
    }
    _texCoords.clear();

    _indices.clear();
}

void SVertexData::malloc(u32 count, u32 index, u32 layer, bool basis, bool color)
{
    clear();

    _vertices.resize(count);
    _normals.resize(count);
    if (basis)
    {
        _binormals.resize(count);
        _tangents.resize(count);
    }
    if (color)
    {
        _colors.resize(count);
    }

    for (u32 i = 0; i < layer; ++i)
    {
        std::vector<Vector2D> texCoord;
        texCoord.resize(count);

        _texCoords.push_back(texCoord);
    }

    if (index > 0)
    {
        _indices.resize(index);
    }

}

u32 SVertexData::verticesSize()
{
    return (u32)_vertices.size();
}

u32 SVertexData::indicesSize()
{
    return (u32)_indices.size();
}

bool SVertexData::empty() const
{
    return _vertices.empty()
        && _normals.empty()
        && _binormals.empty()
        && _tangents.empty()
        && _texCoords.at(0).empty()
        && _colors.empty();
}

void SVertexData::computeTangentBasis()
{
    ASSERT(!_vertices.empty() && "Vertex array empty");
    ASSERT(!_normals.empty() && "Normal array empty");
    ASSERT(!_texCoords.empty() && !_texCoords[0].empty() && "Texture array empty");

    _binormals.clear();
    _tangents.clear();

    _binormals.resize(_vertices.size());
    _tangents.resize(_vertices.size());

    std::vector<Vector3D> tangents, binormals;

    for (u32 i = 0; i < _indices.size(); i += 3)
    {
        u32 index0 = _indices[i + 0];
        u32 index1 = _indices[i + 1];
        u32 index2 = _indices[i + 2];

        Vector3D v0 = _vertices[index0];
        Vector3D v1 = _vertices[index1];
        Vector3D v2 = _vertices[index2];

        f32 s0 = _texCoords[0][index0].x;
        f32 t0 = _texCoords[0][index0].y;
        f32 s1 = _texCoords[0][index1].x;
        f32 t1 = _texCoords[0][index1].y;
        f32 s2 = _texCoords[0][index2].x;
        f32 t2 = _texCoords[0][index2].y;

        Vector3D tangent, binormal;
        calcTriangleBasis(v0, v1, v2, s0, t0, s1, t1, s2, t2, tangent, binormal);

        tangents.push_back(tangent);
        binormals.push_back(binormal);
    }

    for (u32 i = 0; i < _vertices.size(); i++)
    {
        std::vector<Vector3D> rt, rb;
        for (u32 j = 0; j < _indices.size(); j += 3)
        {
            if (_indices[j + 0] == i || _indices[j + 1] == i || _indices[j + 2] == i)
            {
                rt.push_back(tangents[j / 3]);
                rb.push_back(binormals[j / 3]);
            }
        }

        Vector3D tangentRes(0.0f);
        Vector3D binormalRes(0.0f);
        for (u32 j = 0; j < rt.size(); j++)
        {
            tangentRes += rt[j];
            binormalRes += rb[j];
        }
        tangentRes = tangentRes / f32(rt.size());
        binormalRes = binormalRes / f32(rb.size());

        tangentRes = orthogonalize(_normals[i], tangentRes);
        binormalRes = orthogonalize(_normals[i], binormalRes);

        _tangents[i] = tangentRes;
        _binormals[i] = binormalRes;
    }
}