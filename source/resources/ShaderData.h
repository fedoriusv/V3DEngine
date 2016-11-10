#pragma once

#include "stream/Resource.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ShaderData : public stream::IResource
    {

    public:

        ShaderData();
        virtual             ~ShaderData();

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
