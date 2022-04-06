/*
 * Copyright 2022 Adobe. All rights reserved.
 * This file is licensed to you under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
 * OF ANY KIND, either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
#pragma once

#include <lagrange/utils/assert.h>

#include <memory>

namespace lagrange {

/// @addtogroup group-utils-misc
/// @{

///
/// A handle type with copy-on-write semantics. Any copy of the handle will share data ownership,
/// causing potential copies of the data on write access.
///
/// @tparam     T     Pointee type.
///
template <typename T>
class copy_on_write_ptr
{
public:
    using pointer = T*;
    using const_pointer = typename std::add_const<T>::type*;
    using reference = T&;
    using const_reference = typename std::add_const<T>::type&;
    using element_type = T;

public:
    /// Construct a copy-on-write ptr from a shared-pointer
    copy_on_write_ptr(std::shared_ptr<T>&& ptr = nullptr)
        : m_data(std::move(ptr))
    {
        if (m_data) {
            la_runtime_assert(m_data.use_count() == 1);
        }
    }

    /// Default move constructor.
    copy_on_write_ptr(copy_on_write_ptr&&) = default;

    /// Default move assignment operator.
    copy_on_write_ptr& operator=(copy_on_write_ptr&&) = default;

    /// Move-construct from a derived type.
    template <typename Derived>
    copy_on_write_ptr(copy_on_write_ptr<Derived>&& ptr)
        : m_data{std::dynamic_pointer_cast<T>(std::move(ptr.m_data))}
    {}

    /// Default copy constructor.
    copy_on_write_ptr(const copy_on_write_ptr&) = default;

    /// Default copy assignment operator.
    copy_on_write_ptr& operator=(const copy_on_write_ptr&) = default;

    /// Returns a const pointer to the data. Does not require ownership and will not lead to any copy.
    const T* read() const { return m_data.get(); }

    /// Returns a const pointer to the data. Does not require ownership and will not lead to any copy.
    template <typename Derived>
    const Derived* dynamic_read() const
    {
        return dynamic_cast<const Derived*>(m_data.get());
    }

    /// Returns a writable pointer to the data. Will cause a copy if ownership is shared.
    template <typename Derived>
    Derived* dynamic_write()
    {
        ensure_unique_owner<Derived>();
        return dynamic_cast<Derived*>(m_data.get());
    }

    /// Returns a writable smart pointer to the data. Will cause a copy if ownership is shared.
    template <typename Derived>
    std::shared_ptr<Derived> release_ptr()
    {
        ensure_unique_owner<Derived>();
        return std::dynamic_pointer_cast<Derived>(std::move(m_data));
    }

protected:
    /// Shared object.
    std::shared_ptr<T> m_data;

    /// If we are not the owner of the shared object, make a private copy of it
    template <typename Derived>
    void ensure_unique_owner()
    {
        if (m_data.use_count() != 1) {
            auto ptr = dynamic_cast<const Derived*>(m_data.get());
            la_debug_assert(ptr);
            m_data = std::dynamic_pointer_cast<T>(std::make_shared<Derived>(*ptr));
        }
    }
};

/// @}

} // namespace lagrange