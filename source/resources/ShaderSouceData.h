#ifndef _V3D_SHADER_SOURCE_DATA_H_
#define _V3D_SHADER_SOURCE_DATA_H_

#include "stream/Resource.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderSourceData : public stream::CResource
    {

    public:

        CShaderSourceData();
        virtual             ~CShaderSourceData();

        void                init(const stream::IStreamPtr& stream)  override;
        bool                load()                                  override;

        const std::string&  getName() const;
        const std::string&  getBody() const;

    private:

        std::string         m_body;
        std::string         m_name;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d

#endif //_V3D_SHADER_SOURCE_DATA_H_
