/*
 * Copyright 2017 Adobe. All rights reserved.
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

#include <lagrange/SurfaceMesh.h>
#include <lagrange/fs/filesystem.h>
#include <lagrange/io/types.h>

#ifdef LAGRANGE_ENABLE_LEGACY_FUNCTIONS
    #include <lagrange/io/legacy/save_mesh.h>
#endif

#include <iosfwd>

namespace lagrange::io {

/**
 * Save a mesh to a stream.
 *
 * @param[in] output_stream  Stream to output
 * @param[in] mesh           Mesh to save
 * @param[in] format         Mesh format to use.
 * @param[in] options        Extra options related to saving.
 */
template <typename Scalar, typename Index>
void save_mesh(
    std::ostream& output_stream,
    const SurfaceMesh<Scalar, Index>& mesh,
    FileFormat format,
    const SaveOptions& options = {});

/**
 * Save a mesh to a file.
 *
 * @param[in] filename  path to output
 * @param[in] mesh      mesh to save
 * @param[in] options   Extra options related to saving.
 */
template <typename Scalar, typename Index>
void save_mesh(
    const fs::path& filename,
    const SurfaceMesh<Scalar, Index>& mesh,
    const SaveOptions& options = {});

} // namespace lagrange::io
