#pragma once

#include <string>

#include "Ancora/Core/Core.h"

namespace Ancora {

  class Texture
  {
  public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual std::string GetName() const = 0;

    virtual void SetData(void* data, uint32_t size) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
  };

  class Texture2D : public Texture
  {
  public:
    static Ref<Texture2D> Create(uint32_t width, uint32_t height);
    static Ref<Texture2D> Create(const std::string& path);
  };

  class CubeMap
  {
  public:
    virtual ~CubeMap() = default;

    virtual uint32_t GetSize() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;

    static Ref<CubeMap> Create(const std::array<std::string, 6>& cubePaths);
  };

}
