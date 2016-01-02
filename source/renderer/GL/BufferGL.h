#ifndef _V3D_BUFFER_GL_H_
#define _V3D_BUFFER_GL_H_

#include "renderer/gl/GeometryGL.h"
#include "renderer/DataTypes.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Module for buffer objects management. GL render only.
    */
    class BufferGL
    {
    public:

        static void         genBuffer(u32& buffer);
        static void         bindBuffer(EGeometryTarget target, u32 buffer);
        static void         bindBufferTarget(EGeometryTarget target, u32 buffer, u32 size = 0, u32 offset = 0);
        static void         deleteBuffer(u32& buffer);

        static void         bufferData(EGeometryTarget target, EGeomertyType type, u32 size, void* data);
        static void         bufferSubData(EGeometryTarget target, u32 offset, u32 size, void* data);

        static void*        mapBuffer(EGeometryTarget target, u32 access);
        static bool         unmapBuffer(EGeometryTarget target);
        static void*        mapBufferRange(EGeometryTarget target, u32 offset, u32 size, u32 flags);

        static void         getBufferPointer(EGeometryTarget target, u32 pname, void** params);

        static void         genVertexArray(u32& buffer);
        static void         bindVertexArray(u32 buffer);
        static void         deleteVertexArray(u32& buffer);

        static void         initVertexAttribPointer(u32 attrib, EDataType type, u32 count, u32 size = 0, u32 offset = 0);
        static void         vertexAttribArray(u32 attrib, bool enable);
        static void         vertexAttribDivisior(u32 attrib, u32 value);

        static void         drawElements(EDrawMode mode, u32 count, u32 primCount);
        static void         drawArrays(EDrawMode mode, u32 first, u32 count, u32 primCount);

    private:

        static u32          s_currentBuffer[eTargetCount];
        static u32          s_currentArray;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_BUFFER_GL_H_
