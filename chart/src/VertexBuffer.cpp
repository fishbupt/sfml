// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <VertexBuffer.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        VertexBuffer::VertexBuffer()
            : _ptrImpl(new sf::VertexBuffer())
        {
            _ptrImpl->create(0);
        }

        VertexBuffer::~VertexBuffer()
        {
            this->!VertexBuffer();
        }

        VertexBuffer::!VertexBuffer()
        {
        }

        bool VertexBuffer::Update(VertexArray^ vertices)
        {
            Type = vertices->Type;
            _ptrImpl->setUsage((sf::VertexBuffer::Usage)DrawUsage);
            return _ptrImpl->update((const sf::Vertex*)vertices->AsPointer(), vertices->Size, 0);
        }

        void VertexBuffer::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            target->draw(*_ptrImpl, states);
        }

        void VertexBuffer::Draw(IntPtr target, IntPtr states)
        {
            VertexBuffer::Draw((sf::RenderTarget*)target.ToPointer(), *(sf::RenderStates*)states.ToPointer());
        }
    } // namespace Xsa
} // namespace Presentation
} // namespace Graph
