#ifndef _F3D_SHADER_PROGRAM_GL_H_
#define _F3D_SHADER_PROGRAM_GL_H_

#include "renderer/ShaderProgram.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderProgramGL : public CShaderProgram
    {
    public:

        CShaderProgramGL();
        virtual ~CShaderProgramGL();

        void    create()    override;
        void    destroy()   override;
        void    bind()      override;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_PROGRAM_GL_H_