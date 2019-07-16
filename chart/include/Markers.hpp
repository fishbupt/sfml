// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include<SFML/Graphics/OrbitCamera.hpp>
#include<SFML/Graphics/Transformable.hpp>
#include<Drawable.hpp>
#include <ScopedPtr.hpp>

using namespace System;
using namespace System::Collections::Generic;

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        ref class Marker;
        // clang-format off
        public ref class Markers 
        // clang-format on
        {
        public:
            Markers(int count);

            ~Markers();

            !Markers();

            /// <summary>
            /// The number of markers.
            /// </summary>
            property int Count
            {
                int get();
                void set(int value);
            }

            /// <summary>
            /// Index of the marker that has been selected
            /// </summary>
            property int SelectedIndex
            {
                int get();
                void set(int value);
            }

            property Marker^ default[int]
            {
                Marker^ get(int index)
                {
                    if (index < 0 || index >= Count)
                        return nullptr;

                    return _markers[index];
                }
            }


            void Draw(sf::RenderTarget* target, sf::RenderStates states, sf::Transform transform);
        private:
            List<Marker^>^ _markers;
            scoped_ptr<sf::OrbitCamera> _camera;
            int _selectedIndex;
        };
    }
} // Presentation
} // Xsa
