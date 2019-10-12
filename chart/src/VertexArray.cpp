// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <VertexArray.hpp>

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    VertexArray::VertexArray()
        :VertexArray(PrimitiveType::Points)
    {
    }

    VertexArray::VertexArray(PrimitiveType primitiveType)
        : VertexArray(primitiveType, 0)
    {
    }

    VertexArray::VertexArray(PrimitiveType primitiveType, int size)
        : _vertices(new std::vector<sf::Vertex>{ static_cast<std::size_t>(size) })
    {
        Type = primitiveType;
        IsVisible = true;
    }

    VertexArray::~VertexArray()
    {
        this->!VertexArray();
    }

    VertexArray::!VertexArray()
    {
    }

    int VertexArray::Size::get()
    {
        return static_cast<int>(_vertices->size());
    }

    void VertexArray::Size::set(int value)
    {
        _vertices->resize(value);
    }

    int VertexArray::Capacity::get()
    {
        return static_cast<int>(_vertices->capacity());
    }

    void VertexArray::Capacity::set(int value)
    {
        _vertices->reserve(value);
    }

    Vertex* VertexArray::AsPointer()
    {
        return (Vertex*)(_vertices->data());
    }

    void VertexArray::Clear()
    {
        _vertices->clear();
    }

    void VertexArray::Draw(sf::RenderTarget * target, sf::RenderStates states)
    {
        if (IsVisible && _vertices->size() > 0)
        {
            target->draw(_vertices->data(), Size, static_cast<sf::PrimitiveType>(Type), states);
        }
    }

    void VertexArray::Draw(IntPtr target, IntPtr states)
    {
        VertexArray::Draw((sf::RenderTarget*)target.ToPointer(), *(sf::RenderStates*)states.ToPointer());
    }
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
