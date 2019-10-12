// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SFML/Graphics/RenderTarget.hpp>

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// Interface for every object than can be drawn to a render target
    /// </summary>
    interface class Drawable
    {
        /// <summary>
        /// Draw the object to a render target
        /// </summary>
        /// <param name="target">Render target to draw to</param>
        /// <param name="states">Current render states</param>
        void Draw(sf::RenderTarget* target, sf::RenderStates states);
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
