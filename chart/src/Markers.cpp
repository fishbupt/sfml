// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <Markers.hpp>
#include <Marker.hpp>
namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        int clip(int v, int lower, int upper) {
            return std::max(lower, std::min(v, upper));
        }

        Markers::Markers(int count)
            : _camera(new sf::OrbitCamera(45.0f, 0.1f, 100.0f))
        {
            _markers = gcnew List<Marker ^>();
            _selectedIndex = 0;
            for (int i = 0; i < count; i++)
            {
                _markers->Add(gcnew Marker());
            }
        }

        Markers::~Markers()
        {
            this->!Markers();
        }

        Markers::!Markers()
        {
        }

        int Markers::Count::get()
        {
            return _markers->Count;
        }

        void Markers::Count::set(int value)
        {
            int oldCount = Count;
            if (value == oldCount)return;
            if (value < 0) value = 0;

            if (value < oldCount)
            {
                _markers->RemoveRange(value, oldCount - value);
            }
            else
            {
                for (int i = oldCount; i < value; i++)
                    _markers->Add(gcnew Marker());
            }
        }

        int Markers::SelectedIndex::get()
        {
            return _selectedIndex;
        }

        void Markers::SelectedIndex::set(int value)
        {
            value = clip(value, 0, Count);
            if (value != _selectedIndex)
            {
                for each(Marker ^ marker in _markers)
                {
                    marker->Active = false;
                }
                _markers[_selectedIndex]->Active = true;
            }
        }

        void Markers::Draw(sf::RenderTarget* target, sf::RenderStates states, sf::Transform transform)
        {
            static const float defaultX = 1024;
            static const float defaultY = 768;
            const sf::Vector2f currentViewSize = target->getDefaultView().getSize();

            const sf::View& oldView = target->getView();
            target->setView(*_camera);
            for each(Marker ^ marker in _markers)
            {
                marker->Scale(defaultX / currentViewSize.x, defaultY / currentViewSize.y, 1.0f);
                if (marker->Visible)
                {
                    marker->Draw(target, states, transform);
                }
            }

            target->setView(oldView);
        }
    }
} // Presentation
} // Xsa
