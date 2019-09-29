// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
using namespace System;
using namespace System::Runtime::InteropServices;

namespace Xsa::Presentation::Graph
{
    /// <summary>
    /// Vector3f is an utility class for manipulating 3 dimensional vectors with float components
    /// </summary>
    [StructLayout(LayoutKind::Sequential)]
    public value struct Vector3f : IEquatable<Vector3f>
    {
        /// <summary>
        /// X (horizontal) component of the vector
        /// </summary>
        property float X;

        /// <summary>
        /// Y (vertical) component of the vector
        /// </summary>
        property float Y;

        /// <summary>
        /// Z (depth) component of the vector
        /// </summary>
        property float Z;


        /// <summary>
        /// Construct the vector from its coordinates
        /// </summary>
        /// <param name="x">X coordinate</param>
        /// <param name="y">Y coordinate</param>
        /// <param name="z">Z coordinate</param>
        Vector3f(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// Construct the vector from its coordinates
        /// Set Z = 0
        /// </summary>
        /// <param name="x">X coordinate</param>
        /// <param name="y">Y coordinate</param>
        Vector3f(float x, float y) : Vector3f(x, y, 0)
        {
        }

        /// <summary>
        /// Provide a string describing the object
        /// </summary>
        /// <returns>String describing of the object</returns>
        String^ ToString() override
        {
            return String::Format("[Vector3f] X({0}) Y({1}) Z({2})", X, Y, Z);
        }

        /// <summary>
        /// Provide a integer describing the object
        /// </summary>
        /// <returns>Hash value of the object</returns>
        int GetHashCode() override
        {
            return X.GetHashCode() ^ Y.GetHashCode() ^ Z.GetHashCode();
        }

        /// <summary>
        /// Compare two vectors to check if they are equal
        /// </summary>
        /// <param name="other">Vector to check</param>
        /// <returns>Two vectors are equal</returns>
        virtual bool Equals(Vector3f other)
        {
            return (X == other.X) && (Y == other.Y) && (Z == other.Z);
        }

        /// <summary>
        /// Operator == overload ; check vector equality
        /// </summary>
        /// <param name="lhs">First vector</param>
        /// <param name="rhs">Second vector</param>
        /// <returns>lhs == rhs</returns>
        static bool operator ==(Vector3f lhs, Vector3f rhs)
        {
            return lhs.Equals(rhs);
        }

        /// <summary>
        /// Operator != overload ; check vector inequality
        /// </summary>
        /// <param name="lhs">First vector</param>
        /// <param name="rhs">Second vector</param>
        /// <returns>lhs != rhs</returns>
        static bool operator !=(Vector3f lhs, Vector3f rhs)
        {
            return !lhs.Equals(rhs);
        }
    };

    /// <summary>
    /// Vector2f is an utility class for manipulating 2 dimensional vectors with float components
    /// </summary>
    [StructLayout(LayoutKind::Sequential)]
    public value struct Vector2f : IEquatable<Vector2f>
    {
        /// <summary>
        /// X (horizontal) component of the vector
        /// </summary>
        property float X;

        /// <summary>
        /// Y (vertical) component of the vector
        /// </summary>
        property float Y;

        /// <summary>
        /// Construct the vector from its coordinates
        /// </summary>
        /// <param name="x">X coordinate</param>
        /// <param name="y">Y coordinate</param>
        Vector2f(float x, float y)
        {
            X = x;
            Y = y;
        }

        /// <summary>
        /// Provide a string describing the object
        /// </summary>
        /// <returns>String describing of the object</returns>
        String^ ToString() override
        {
            return String::Format("[Vector2f] X({0}) Y({1})", X, Y);
        }

        /// <summary>
        /// Provide a integer describing the object
        /// </summary>
        /// <returns>Hash value of the object</returns>
        int GetHashCode() override
        {
            return X.GetHashCode() ^ Y.GetHashCode();
        }

        /// <summary>
        /// Compare two vectors to check if they are equal
        /// </summary>
        /// <param name="other">Vector to check</param>
        /// <returns>Two vectors are equal</returns>
        virtual bool Equals(Vector2f other)
        {
            return (X == other.X) && (Y == other.Y);
        }

        /// <summary>
        /// Operator == overload ; check vector equality
        /// </summary>
        /// <param name="lhs">First vector</param>
        /// <param name="rhs">Second vector</param>
        /// <returns>lhs == rhs</returns>
        static bool operator ==(Vector2f lhs, Vector2f rhs)
        {
            return lhs.Equals(rhs);
        }

        /// <summary>
        /// Operator != overload ; check vector inequality
        /// </summary>
        /// <param name="lhs">First vector</param>
        /// <param name="rhs">Second vector</param>
        /// <returns>lhs != rhs</returns>
        static bool operator !=(Vector2f lhs, Vector2f rhs)
        {
            return !lhs.Equals(rhs);
        }
    };
}
