/*
 * Copyright (c) 2019 - 2023 Geode-solutions
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <geode/mesh/io/polyhedral_solid_input.h>

#include <absl/strings/ascii.h>

#include <geode/basic/filename.h>
#include <geode/basic/identifier_builder.h>
#include <geode/basic/timer.h>

#include <geode/mesh/core/mesh_factory.h>
#include <geode/mesh/core/polyhedral_solid.h>

namespace geode
{
    template < index_t dimension >
    std::unique_ptr< PolyhedralSolid< dimension > > load_polyhedral_solid(
        const MeshImpl& impl, absl::string_view filename )
    {
        try
        {
            Timer timer;
            const auto extension =
                absl::AsciiStrToLower( extension_from_filename( filename ) );
            OPENGEODE_EXCEPTION(
                PolyhedralSolidInputFactory< dimension >::has_creator(
                    extension ),
                "Unknown extension: ", extension );
            auto input = PolyhedralSolidInputFactory< dimension >::create(
                extension, filename );
            auto polyhedral_solid = input->read( impl );
            if( polyhedral_solid->name() == Identifier::DEFAULT_NAME )
            {
                IdentifierBuilder{ *polyhedral_solid }.set_name(
                    filename_without_extension( filename ) );
            }
            Logger::info( "PolyhedralSolid", dimension, "D loaded from ",
                filename, " in ", timer.duration() );
            Logger::info( "PolyhedralSolid", dimension,
                "D has: ", polyhedral_solid->nb_vertices(), " vertices, ",
                polyhedral_solid->nb_polyhedra(), " polyhedra" );
            return polyhedral_solid;
        }
        catch( const OpenGeodeException& e )
        {
            Logger::error( e.what() );
            throw OpenGeodeException{ "Cannot load PolyhedralSolid from file: ",
                filename };
        }
    }

    template < index_t dimension >
    std::unique_ptr< PolyhedralSolid< dimension > > load_polyhedral_solid(
        absl::string_view filename )
    {
        return load_polyhedral_solid< dimension >(
            MeshFactory::default_impl(
                PolyhedralSolid< dimension >::type_name_static() ),
            filename );
    }

    template std::unique_ptr< PolyhedralSolid< 3 > > opengeode_mesh_api
        load_polyhedral_solid( const MeshImpl&, absl::string_view );

    template std::unique_ptr< PolyhedralSolid< 3 > >
        opengeode_mesh_api load_polyhedral_solid( absl::string_view );
} // namespace geode
