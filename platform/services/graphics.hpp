#pragma once

namespace lib {

class GraphicsAPI {
public:
    virtual void CreateVertexBuffer(unsigned int size, const void* data, unsigned int& vbo) = 0;
    virtual void CreateIndexBuffer(unsigned int size, const void* data, unsigned int& ibo) = 0;
    virtual void BindVertexBuffer(unsigned int vbo) = 0;
    virtual void BindIndexBuffer(unsigned int ibo) = 0;
    virtual void DrawIndexed(unsigned int count) = 0;
};
}