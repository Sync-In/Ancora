#include "aepch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Ancora {

  OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height)
  {
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_Path(path)
  {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    AE_CORE_ASSERT(data, "Failed to load image!");
    m_Width = width;
    m_Height = height;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4)
    {
      internalFormat = GL_RGBA8;
      dataFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
      internalFormat = GL_RGB8;
      dataFormat = GL_RGB;
    }

    m_InternalFormat = internalFormat;
    m_DataFormat = dataFormat;

    AE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  OpenGLTexture2D::~OpenGLTexture2D()
  {
    glDeleteTextures(1, &m_RendererID);
  }

  void OpenGLTexture2D::SetData(void* data, uint32_t size)
  {
    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    AE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
  }

  void OpenGLTexture2D::Bind(uint32_t slot) const
  {
    glBindTextureUnit(slot, m_RendererID);
  }


  OpenGLCubeMap::OpenGLCubeMap(const std::array<std::string, 6>& cubePaths)
  {
    // Change this to be OpenGL 4.5 compatible
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, channels;

    for (GLuint i = 0; i < 6; i++)
    {
      stbi_uc* data = stbi_load(cubePaths[i].c_str(), &width, &height, &channels, 0);
      AE_CORE_ASSERT(data, "Failed to load image!");
      m_Size = width;

      GLenum internalFormat = 0, dataFormat = 0;
      if (channels == 4)
      {
        internalFormat = GL_RGBA;
        dataFormat = GL_RGBA;
      }
      else if (channels == 3)
      {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
      }

      m_InternalFormat = internalFormat;
      m_DataFormat = dataFormat;

      AE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }
  }

  OpenGLCubeMap::~OpenGLCubeMap()
  {
    glDeleteTextures(1, &m_RendererID);
  }

  void OpenGLCubeMap::Bind(uint32_t slot) const
  {
    glBindTextureUnit(slot, m_RendererID);
  }

}
