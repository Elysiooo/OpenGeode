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

#include <geode/model/mixin/core/corner.h>

#include <geode/basic/bitsery_archive.h>
#include <geode/basic/pimpl_impl.h>

#include <geode/mesh/builder/point_set_builder.h>
#include <geode/mesh/core/mesh_factory.h>
#include <geode/mesh/core/point_set.h>

#include <geode/model/mixin/core/detail/mesh_storage.h>

namespace geode
{
    template < index_t dimension >
    class Corner< dimension >::Impl
        : public detail::MeshStorage< PointSet< dimension > >
    {
    private:
        friend class bitsery::Access;
        template < typename Archive >
        void serialize( Archive& archive )
        {
            archive.ext( *this,
                Growable< Archive, Impl >{ { []( Archive& a, Impl& impl ) {
                    a.ext( impl,
                        bitsery::ext::BaseClass<
                            detail::MeshStorage< PointSet< dimension > > >{} );
                } } } );
        }
    };

    template < index_t dimension >
    Corner< dimension >::~Corner() // NOLINT
    {
    }

    template < index_t dimension >
    Corner< dimension >::Corner()
        : Corner( MeshFactory::default_impl(
            PointSet< dimension >::type_name_static() ) )
    {
    }

    template < index_t dimension >
    Corner< dimension >::Corner( Corner&& other )
        : impl_( std::move( other.impl_ ) )
    {
    }

    template < index_t dimension >
    Corner< dimension >::Corner( const MeshImpl& impl )
    {
        impl_->set_mesh( this->id(), PointSet< dimension >::create( impl ) );
    }

    template < index_t dimension >
    const PointSet< dimension >& Corner< dimension >::mesh() const
    {
        return impl_->mesh();
    }

    template < index_t dimension >
    PointSet< dimension >& Corner< dimension >::modifiable_mesh()
    {
        return impl_->modifiable_mesh();
    }

    template < index_t dimension >
    const MeshImpl& Corner< dimension >::mesh_type() const
    {
        return impl_->mesh_type();
    }

    template < index_t dimension >
    template < typename Archive >
    void Corner< dimension >::serialize( Archive& archive )
    {
        archive.ext( *this,
            Growable< Archive, Corner >{
                { []( Archive& a, Corner& corner ) {
                     a.object( corner.impl_ );
                     a.ext( corner,
                         bitsery::ext::BaseClass< Component< dimension > >{} );
                     IdentifierBuilder mesh_builder{ corner.modifiable_mesh() };
                     mesh_builder.set_id( corner.id() );
                 },
                    []( Archive& a, Corner& corner ) {
                        a.object( corner.impl_ );
                        a.ext( corner, bitsery::ext::BaseClass<
                                           Component< dimension > >{} );
                    } } } );
    }

    template < index_t dimension >
    void Corner< dimension >::set_mesh(
        std::unique_ptr< PointSet< dimension > > mesh, CornersKey )
    {
        impl_->set_mesh( this->id(), std::move( mesh ) );
    }

    template < index_t dimension >
    void Corner< dimension >::set_mesh(
        std::unique_ptr< PointSet< dimension > > mesh, CornersBuilderKey )
    {
        impl_->set_mesh( this->id(), std::move( mesh ) );
    }

    template class opengeode_model_api Corner< 2 >;
    template class opengeode_model_api Corner< 3 >;

    SERIALIZE_BITSERY_ARCHIVE( opengeode_model_api, Corner< 2 > );
    SERIALIZE_BITSERY_ARCHIVE( opengeode_model_api, Corner< 3 > );
} // namespace geode
