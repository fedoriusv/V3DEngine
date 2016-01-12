#ifndef _V3D_GEOMETRY_TARGET_H_
#define _V3D_GEOMETRY_TARGET_H_

#include "Target.h"
#include "DataTypes.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;
    class CRenderTechnique;
    class Buffer;

    /**
    * Inherited class for geometry target management.
    * Used output to vertex buffer through geometry
    */
    class CGeometryTarget : public ITarget
    {
    public:

        struct SBufferData
        {
            SBufferData();
            ~SBufferData();

            SBufferData& operator=(const SBufferData& data);

            std::string _name;
            EDataType   _type;
            u32         _size;
            Buffer*     _buffer;

            s32         _result;
        };

        CGeometryTarget();
        virtual                         ~CGeometryTarget();

        const std::vector<const c8*>&   getBufferStrings() const;

        const SBufferData*              getBuffer(const std::string& name);

    protected:

        friend                          CRenderPass;
        friend                          CRenderTechnique;

        void                            addBuffer(const std::string& name, EDataType type);
        bool                            parse(const tinyxml2::XMLElement* root);

        std::vector<const c8*>          m_bufferStrings;
        std::vector<SBufferData>        m_bufferList;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CGeometryTarget>        GeometryTargetPtr;
    typedef std::weak_ptr<CGeometryTarget>          GeometryTargetWPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TARGET_H_

