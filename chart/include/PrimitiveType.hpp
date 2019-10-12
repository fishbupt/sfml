// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// Types of primitives that a Vertex can render.
    ///
    /// Points and lines have no area, therefore their thickness
    /// will always be 1 pixel, regardless the current transform
    /// and view.
    /// </summary>
    public enum class PrimitiveType
    {
        /// List of individual points
        Points,

        /// List of individual lines
        Lines,

        /// List of connected lines, a point uses the previous point to form a line
        LineStrip,

        /// List of individual triangles
        Triangles,

        /// List of connected triangles, a point uses the two previous points to form a triangle
        TriangleStrip,

        /// List of connected triangles, a point uses the common center and the previous point to form a triangle
        TriangleFan,

        /// List of individual quads
        Quads
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
