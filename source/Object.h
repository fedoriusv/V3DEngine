#ifndef _V3D_OBJECT_H_
#define _V3D_OBJECT_H_

#include "common.h"

namespace v3d
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class EObjectType
	{
		eTypeUnknown,
		eTypeNode,
		eTypeTexture,
		eTypeMaterial,
		eTypeMesh,
		eTypeShader,
		eTypeShaderProgram,
		eTypesCount
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

    class CObject
    {
    public:

        CObject();
        ~CObject();

        const s32           getID() const;
        const std::string&  getName() const;
        const EObjectType   getObjectType() const;

        void                setID(const s32 id);
        void                setName(const std::string& name);

    protected:

        s32                 m_id;
        std::string         m_name;
        EObjectType         m_type;

    };

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_V3D_OBJECT_H_
