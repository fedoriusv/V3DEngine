#ifndef _F3D_GEOMETRY_GL__H_
#define _F3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"

namespace f3d
{
namespace renderer
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum EShaderAttribute
	{
		eAttributeVertex = 0,
		eAttributeColor,
		eAttributeNormal,
		eAttributeBinormal,
		eAttributeTangent,
		eAttributeTexture0,
		eAttributeTexture1,
		eAttributeTexture2,
		eAttributeTexture3,

		eAttributeCount
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CGeometryGL : public CGeometry
	{
	public:

		CGeometryGL();
		virtual      ~CGeometryGL();

		void         init()    override;
		void         update()  override;
		void         free()    override;
		void         refresh();

	private:

		f3d::u32     m_drawModeGL;
		f3d::u32     m_arrayId;

		void         genBuffers(f3d::u32& buffer);
		void         bindBuffers(const f3d::u32 target, const f3d::u32 buffer);
		void         deleteBuffers(f3d::u32& buffer);

		void         bufferData(const f3d::u32 target, const f3d::u32 size, void* data);
		void         bufferSubData(const f3d::u32 target, const f3d::u32 offset, const f3d::u32 size, void* data);

		void*        mapBuffer(const f3d::u32 target, const f3d::u32 access);
		bool         unmapBuffer(const f3d::u32 target);
		void*        mapBufferRange(const f3d::u32 target, const f3d::u32 offset, const f3d::u32 size, const f3d::u32 flags);

		void         getBufferPointer(const f3d::u32 target, const f3d::u32 pname, void** params);

		void         genVertexArray(f3d::u32& buffer);
		void         bindVertexArray(const f3d::u32 buffer);
		void         deleteVertexArray(f3d::u32& buffer);

		void         initVertexAttribPointer(const f3d::u32 vertexAttrib, const f3d::u32 size);
		void         refreshBufferData();

		f3d::u32     getGLDrawMode(EDrawMode mode);

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_GEOMETRY_GL_H_
