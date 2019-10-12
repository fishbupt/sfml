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
    public:
        /// <summary>
        /// Constructs a scoped_ptr that onws nothing
        /// </summary>
        scoped_ptr()
            : _native_ptr(nullptr)
        {
        }

        /// <summary>
        /// Constructs a scoped_ptr which owns ptr
        /// </summary>
        /// <param name="ptr"></param>
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

        scoped_ptr % operator=(T* ptr)
        {
            reset(ptr);
            return (scoped_ptr %) this;
        }

        /// <summary>
        /// Replaces the managed object.
        /// </summary>
        /// <param name="ptr">pointer to a new object to managed. default=nullptr</param>
        void reset(T* ptr)
        {
            delete _native_ptr;
            _native_ptr = ptr;
        }

        /// <summary>
        /// Replaces the managed object with nullptr, equivalent to release()
        /// </summary>
        void reset()
        {
            reset(nullptr);
        }

        /// <summary>
        /// Releases the ownership of the managed object if any.
        /// get() return nullptr after the call.
        /// </summary>
        /// <returns></returns>
        T* release()
        {
            T* return_ptr = get();
            _native_ptr = nullptr;
        }

        /// <summary>
        /// Returns a pointer to the managed object or nullptr if no object is owned
        /// </summary>
        /// <returns></returns>
        T* get()
        {
            return _native_ptr;
        }

        /// <summary>
        /// Checks whether *this owns an object.
        /// i.e. whether get() != nullptr
        /// </summary>
        /// <returns></returns>
        explicit operator bool()
        {
            return get() != nullptr;
        }

        /// <summary>
        /// Returns a pointer to the object owned by *this, equivalent to get()
        /// </summary>
        /// <returns></returns>
        T* operator->()
        {
            return get();
        }

        /// <summary>
        /// Returns the object owned by *this, equivalent to *get()
        /// </summary>
        /// <param name="sptr"></param>
        /// <returns></returns>
        static T& operator*(scoped_ptr<T> % sptr)
        {
            return *sptr.get();
        }

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
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
