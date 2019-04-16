// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <utility>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        /// <summary>
        /// C++/CLI analogue to std::unique_ptr
        /// for management of the lifetime of an unmanaged class instance by a managed object
        /// </summary>
        template <typename T>
        public ref class scoped_ptr
        {
        private:
            T* _native_ptr;

        private:
            // delete copy constructor and copy assignment
            scoped_ptr(scoped_ptr<T> %)
            {
            }

            scoped_ptr % operator=(scoped_ptr<T> %)
            {
            }

        public:
            // constructors and destructors
            scoped_ptr()
                : _native_ptr(nullptr)
            {
            }

            explicit scoped_ptr(T* ptr)
                : _native_ptr(ptr){};

            !scoped_ptr()
            {
                reset();
            }

            ~scoped_ptr()
            {
                scoped_ptr::!scoped_ptr();
            }

            // public methods
            void reset(T* ptr)
            {
                delete _native_ptr;
                _native_ptr = ptr;
            }

            void reset()
            {
                reset(nullptr);
            }

            scoped_ptr % operator=(T* ptr)
            {
                reset(ptr);
                return (scoped_ptr %) this;
            }

            T* get()
            {
                return _native_ptr;
            }

            T* release()
            {
                T* return_ptr = get();
                _native_ptr = nullptr;
            }

            void swap(scoped_ptr<T> % other)
            {
                std::swap(_native_ptr, other._native_ptr);
            }

            explicit operator bool()
            {
                return get();
            }

            T* operator->()
            {
                return get();
            }

            static T& operator*(scoped_ptr<T> % sptr)
            {
                return *sptr.get();
            }
        };

        template <typename T, typename... Ts>
        scoped_ptr<T> make_scoped(Ts&&... params)
        {
            return scoped_ptr<T>(new T(std::forward<Ts>(params...)));
        }
    }
}
}
