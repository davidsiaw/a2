//
//  emptyscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2014/10/16.
//  Copyright (c) 2014年 David Siaw. All rights reserved.
//

#ifndef VBOSCENE_HPP
#define VBOSCENE_HPP

#include "composablescene_interface.hpp"
#include "vbo_interface.hpp"
#include "printlog.hpp"

class VboScene : public ComposableSceneInterface
{
    std::shared_ptr<VboInterface> vbo;
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<SDL_Surface> texSurface;
    std::shared_ptr<GLuint> texture;
    bool texturePresent;
    GLuint buffer;
	GLuint array;
    int tile;
    Uint32 animationDelay;
    Uint32 lastUpdate;
    float transparency;

    glm::mat4 matrix;
    
    glm::mat4 translation;
    Uint16 x, y, z;
    
    void EstablishElements()
    {
        glVertexAttribPointer(shader->GetPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, v));
		PRINT_GLERROR
        
        if (shader->HasColorAttribute())
        {
            glVertexAttribPointer(shader->GetColorAttribute(), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Element), OFFSET_OF(Element, c));
			PRINT_GLERROR
        }
        
        if (shader->HasTexCoordAttribute())
        {
            glVertexAttribPointer(shader->GetTexCoordAttribute(), 2, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, t));
			PRINT_GLERROR
        }
        
        if (shader->HasTileAttribute())
        {
            glVertexAttribPointer(shader->GetTileOffsetWAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, tileOffsetW));
			PRINT_GLERROR
            glVertexAttribPointer(shader->GetTileOffsetHAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, tileOffsetH));
			PRINT_GLERROR
        }
        
        if (shader->HasTileNumAttribute())
        {
            glVertexAttribPointer(shader->GetTilenumAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, numtiles));
			PRINT_GLERROR
        }

    }
    
    void Upload()
    {
        int bufferSize = vbo->GetElementCount() * sizeof(Element);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
		PRINT_GLERROR
        
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vbo->GetElements(), GL_STATIC_DRAW);
		PRINT_GLERROR
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
		PRINT_GLERROR
    }
    
    virtual void DeleteTexture()
    {
        if (texture)
        {
            GLuint textureIndex = *texture;
            glDeleteTextures(1, &textureIndex);
            texture = NULL;
        }
    }
    
public:
    VboScene(
        std::shared_ptr<ShaderInterface> shader, 
        std::shared_ptr<VboInterface> vbo, 
        std::shared_ptr<SDL_Surface> texSurface,
        Uint32 animationDelay = 200)
            : vbo(vbo),
            shader(shader),
            texSurface(texSurface),
            texturePresent(texSurface.get() != nullptr),
            tile(0),
            animationDelay(animationDelay),
            lastUpdate(0),
            transparency(1.0f),
            x(0),
            y(0),
            z(0)
    {
		glGenVertexArrays(1, &array);
		PRINT_GLERROR
		glBindVertexArray(array);
		PRINT_GLERROR

        glGenBuffers(1, &buffer);
		PRINT_GLERROR
        Upload();

    }
    
    virtual ~VboScene()
    {
        DeleteTexture();
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &buffer);
		glDeleteVertexArrays(1, &array);
    }

    void SetMatrixToIdentity()
    {
        matrix = glm::mat4();
    }

    void SetMatrixTo2DView(int width, int height)
    {
        matrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 1.0f, 0.0f);
    }
    
    void SetMatrixTo2DRectangle(int x, int y, int width, int height)
    {
        matrix = glm::ortho((float)x, (float)width + (float)x, (float)height + (float)y, (float)y, 1.0f, 0.0f);    }
    
    virtual void Init(SDL_Window* window)
    {
        
        UpdateTexture();
    }
    
    virtual void _SetGLTexture(GLuint glTexture)
    {
        this->texture = std::shared_ptr<GLuint>(new GLuint(glTexture));
    }

    
    virtual void SetTexture(std::shared_ptr<SDL_Surface> texSurface)
    {
        this->texSurface = texSurface;
    }
    
    virtual void UpdateTexture()
    {
        if (texSurface)
        {
            GLuint textureIndex;
            if (!texture)
            {
                /* Create The Texture */
                glGenTextures( 1, &textureIndex );
            }
            else
            {
                textureIndex = *texture;
            }
            
            /* Typical Texture Generation Using Data From The Bitmap */
            glBindTexture( GL_TEXTURE_2D, textureIndex );
			PRINT_GLERROR
            
            /* Generate The Texture */
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, texSurface->w, texSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texSurface->pixels );
			PRINT_GLERROR
            
            /* Linear Filtering */
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			PRINT_GLERROR
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			PRINT_GLERROR
            
            if (!texture)
            {
                texture = std::shared_ptr<GLuint>(new GLuint(textureIndex));
            }
            
            if (texture)
            {
                texturePresent = true;
            }
        }
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        if (timestamp > lastUpdate + animationDelay)
        {
            lastUpdate = timestamp;
            tile++;
        }
        
        if (vbo->Changed())
        {
            Upload();
            vbo->ChangeCommitted();
        }
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        glUseProgram(shader->GetProgram());

        if (shader->HasTexCoordAttribute())
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			PRINT_GLERROR
            glEnable(GL_BLEND);
			PRINT_GLERROR
            glActiveTexture(GL_TEXTURE0);
			PRINT_GLERROR
            glBindTexture(GL_TEXTURE_2D, *texture);
			PRINT_GLERROR
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
		PRINT_GLERROR
        EstablishElements();

        shader->SetMatrix(matrix * translation);
        shader->SetActiveTileUniform(tile);
        
        glEnableVertexAttribArray(shader->GetPositionAttribute());
		PRINT_GLERROR
        if (shader->HasTexCoordAttribute()) glEnableVertexAttribArray(shader->GetTexCoordAttribute());
        if (shader->HasColorAttribute()) glEnableVertexAttribArray(shader->GetColorAttribute());
        if (shader->HasTileAttribute())
        {
            glEnableVertexAttribArray(shader->GetTileOffsetWAttribute());
			PRINT_GLERROR
            glEnableVertexAttribArray(shader->GetTileOffsetHAttribute());
			PRINT_GLERROR
        }
        if (shader->HasTileNumAttribute()) glEnableVertexAttribArray(shader->GetTilenumAttribute());
        
        if (shader->HasTransparencyUniform()) shader->SetTransparencyUniform(transparency);
        if (shader->HasActiveTileUniform())  shader->SetActiveTileUniform(tile);
        
        if (shader->HasTexCoordAttribute())
        {
            shader->SetTexturePresentUniform(texturePresent);
            shader->SetTextureUniform(0);
        }

        glDrawArrays(vbo->GetBufferType(), 0, vbo->GetElementCount());
        
        glDisableVertexAttribArray(shader->GetPositionAttribute());
		PRINT_GLERROR
        if (shader->HasTexCoordAttribute()) glDisableVertexAttribArray(shader->GetTexCoordAttribute());
        if (shader->HasColorAttribute()) glDisableVertexAttribArray(shader->GetColorAttribute());
        if (shader->HasTileAttribute())
        {
            glDisableVertexAttribArray(shader->GetTileOffsetWAttribute());
			PRINT_GLERROR
            glDisableVertexAttribArray(shader->GetTileOffsetHAttribute());
			PRINT_GLERROR
        }
        if (shader->HasTileNumAttribute()) glDisableVertexAttribArray(shader->GetTilenumAttribute());
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
		PRINT_GLERROR
        
        if (shader->HasTexCoordAttribute())
        {
            glBindTexture(GL_TEXTURE_2D, 0);
			PRINT_GLERROR
            glDisable(GL_BLEND);
			PRINT_GLERROR
        }
        
        
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z)
    {
        translation = glm::translate(glm::mat4(), glm::vec3((float)x, (float)y, (float)z));
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    virtual Uint16 GetOriginX() const
    {
        return x;
    }
    
    virtual Uint16 GetOriginY() const
    {
        return y;
    }
    
    virtual Uint16 GetOriginZ() const
    {
        return z;
    }

};



#endif //GUISCENE_HPP
