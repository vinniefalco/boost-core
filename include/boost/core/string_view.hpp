#ifndef BOOST_CORE_STRING_VIEW_HPP_INCLUDED
#define BOOST_CORE_STRING_VIEW_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

// boost::core::basic_string_view<Ch>
//
// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/assert.hpp>
#include <boost/throw_exception.hpp>
#include <boost/config.hpp>
#include <string>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <cstddef>
#include <cstring>
#if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
# include <string_view>
#endif

namespace boost
{
namespace core
{

template<class Ch> class basic_string_view
{
private:

    Ch const* p_;
    std::size_t n_;

public:

    // types

    typedef std::char_traits<Ch> traits_type;
    typedef Ch value_type;
    typedef Ch* pointer;
    typedef Ch const* const_pointer;
    typedef Ch& reference;
    typedef Ch const& const_reference;
    typedef Ch const* const_iterator;
    typedef const_iterator iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef const_reverse_iterator reverse_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // npos

    BOOST_STATIC_CONSTEXPR size_type npos = static_cast<size_type>( -1 );

public:

    // construction and assignment

    BOOST_CONSTEXPR basic_string_view() BOOST_NOEXCEPT: p_(), n_()
    {
    }

    BOOST_CONSTEXPR basic_string_view( Ch const* str ) BOOST_NOEXCEPT: p_( str ), n_( traits_type::length( str ) )
    {
    }

    BOOST_CONSTEXPR basic_string_view( Ch const* str, size_type len ) BOOST_NOEXCEPT: p_( str ), n_( len )
    {
    }

    BOOST_CXX14_CONSTEXPR basic_string_view( Ch const* begin, Ch const* end ) BOOST_NOEXCEPT: p_( begin ), n_( end - begin )
    {
        BOOST_ASSERT( end - begin >= 0 );
    }

    template<class A> basic_string_view( std::basic_string<Ch, std::char_traits<Ch>, A> const& str ) BOOST_NOEXCEPT: p_( str.data() ), n_( str.size() )
    {
    }

#if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)

    basic_string_view( std::basic_string_view<Ch, std::char_traits<Ch> > const& str ) BOOST_NOEXCEPT: p_( str.data() ), n_( str.size() )
    {
    }

#endif

    // BOOST_CONSTEXPR basic_string_view& operator=( basic_string_view const& ) BOOST_NOEXCEPT & = default;

    // iterator support

    BOOST_CONSTEXPR const_iterator begin() const BOOST_NOEXCEPT
    {
        return p_;
    }

    BOOST_CONSTEXPR const_iterator end() const BOOST_NOEXCEPT
    {
        return p_ + n_;
    }

    BOOST_CONSTEXPR const_iterator cbegin() const BOOST_NOEXCEPT
    {
        return p_;
    }

    BOOST_CONSTEXPR const_iterator cend() const BOOST_NOEXCEPT
    {
        return p_ + n_;
    }

    BOOST_CONSTEXPR const_reverse_iterator rbegin() const BOOST_NOEXCEPT
    {
        return const_reverse_iterator( begin() );
    }

    BOOST_CONSTEXPR const_reverse_iterator rend() const BOOST_NOEXCEPT
    {
        return const_reverse_iterator( end() );
    }

    BOOST_CONSTEXPR const_reverse_iterator crbegin() const BOOST_NOEXCEPT
    {
        return const_reverse_iterator( begin() );
    }

    BOOST_CONSTEXPR const_reverse_iterator crend() const BOOST_NOEXCEPT
    {
        return const_reverse_iterator( end() );
    }

    // capacity

    BOOST_CONSTEXPR size_type size() const BOOST_NOEXCEPT
    {
        return n_;
    }

    BOOST_CONSTEXPR size_type length() const BOOST_NOEXCEPT
    {
        return n_;
    }

    BOOST_CONSTEXPR size_type max_size() const BOOST_NOEXCEPT
    {
        return npos / sizeof( Ch );
    }

    BOOST_CONSTEXPR bool empty() const BOOST_NOEXCEPT
    {
        return n_ == 0;
    }

    // element access

    BOOST_CONSTEXPR const_reference operator[]( size_type pos ) const BOOST_NOEXCEPT
    {
        BOOST_ASSERT( pos < size() );
        return p_[ pos ];
    }

    BOOST_CONSTEXPR const_reference at( size_type pos ) const
    {
        if( pos >= size() )
        {
            boost::throw_exception( std::out_of_range( "basic_string_view::at" ), BOOST_CURRENT_LOCATION );
        }

        return p_[ pos ];
    }

    BOOST_CONSTEXPR const_reference front() const BOOST_NOEXCEPT
    {
        BOOST_ASSERT( !empty() );
        return p_[ 0 ];
    }

    BOOST_CONSTEXPR const_reference back() const BOOST_NOEXCEPT
    {
        BOOST_ASSERT( !empty() );
        return p_[ n_ - 1 ];
    }

    BOOST_CONSTEXPR const_pointer data() const BOOST_NOEXCEPT
    {
        return p_;
    }

    // modifiers

    BOOST_CXX14_CONSTEXPR void remove_prefix( size_type n ) BOOST_NOEXCEPT
    {
        BOOST_ASSERT( n <= size() );

        p_ += n;
        n_ -= n;
    }

    BOOST_CXX14_CONSTEXPR void remove_suffix( size_type n ) BOOST_NOEXCEPT
    {
        BOOST_ASSERT( n <= size() );

        n_ -= n;
    }

    BOOST_CXX14_CONSTEXPR void swap( basic_string_view& s ) BOOST_NOEXCEPT
    {
        std::swap( p_, s.p_ );
        std::swap( n_, s.n_ );
    }

    // string operations

    BOOST_CONSTEXPR size_type copy( Ch* s, size_type n, size_type pos = 0 ) const
    {
        if( pos > size() )
        {
            boost::throw_exception( std::out_of_range( "basic_string_view::copy" ), BOOST_CURRENT_LOCATION );
        }

        std::size_t rlen = std::min( n, size() - pos );

        traits_type::copy( s, data() + pos, rlen );

        return rlen;
    }

    BOOST_CONSTEXPR basic_string_view substr( size_type pos = 0, size_type n = npos ) const
    {
        if( pos > size() )
        {
            boost::throw_exception( std::out_of_range( "basic_string_view::substr" ), BOOST_CURRENT_LOCATION );
        }

        std::size_t rlen = std::min( n, size() - pos );

        return basic_string_view( data() + pos, rlen );
    }

    // compare

    BOOST_CONSTEXPR int compare( basic_string_view str ) const BOOST_NOEXCEPT
    {
        std::size_t rlen = std::min( size(), str.size() );

        int cmp = traits_type::compare( data(), str.data(), rlen );

        if( cmp != 0 ) return cmp;

        if( size() == str.size() ) return 0;
        
        return size() < str.size()? -1: +1;
    }

    BOOST_CONSTEXPR int compare( size_type pos1, size_type n1, basic_string_view str ) const
    {
        return substr( pos1, n1 ).compare( str );
    }

    BOOST_CONSTEXPR int compare( size_type pos1, size_type n1, basic_string_view str, size_type pos2, size_type n2 ) const
    {
        return substr( pos1, n1 ).compare( str.substr( pos2, n2 ) );

    BOOST_CONSTEXPR int compare( Ch const* s ) const BOOST_NOEXCEPT
    {
        return compare( basic_string_view( s ) );

    BOOST_CONSTEXPR int compare( size_type pos1, size_type n1, Ch const* s ) const
    {
        return substr( pos1, n1 ).compare( basic_string_view( s ) );
    }

    BOOST_CONSTEXPR int compare( size_type pos1, size_type n1, Ch const* s, size_type n2 ) const
    {
        return substr( pos1, n1 ).compare( basic_string_view( s, n2 ) );

    // starts_with

    BOOST_CONSTEXPR bool starts_with( basic_string_view x ) const BOOST_NOEXCEPT
    {
        return substr( 0, x.size() ) == x;
    }

    BOOST_CONSTEXPR bool starts_with( Ch x ) const BOOST_NOEXCEPT
    {
        return !empty() && front() == x;

    BOOST_CONSTEXPR bool starts_with( Ch const* x ) const
    {
        return starts_with( basic_string_view( x ) );
    }

    // ends_with

    BOOST_CONSTEXPR bool ends_with( basic_string_view x ) const BOOST_NOEXCEPT
    {
        return size() >= x.size() && compare( size() - x.size(), npos, x ) == 0;

    BOOST_CONSTEXPR bool ends_with( Ch x ) const BOOST_NOEXCEPT
    {
        return ends_with( basic_string_view( x ) );
    }

    // find

    BOOST_CONSTEXPR size_type find( basic_string_view str, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        return find( str.data(), pos, str.size() );
    }

    BOOST_CONSTEXPR size_type find( Ch c, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        if( pos >= size() ) return npos;

        Ch const* r = traits_type::find( data() + pos, size() - pos, c );

        return r? r - data(): npos;
    }

    BOOST_CONSTEXPR size_type find( Ch const* s, size_type pos, size_type n ) const BOOST_NOEXCEPT
    {
        if( pos + n > size() ) return npos;
        if( n == 0 ) return pos;

        Ch const* p = data() + pos;
        Ch const* last = data() + size();

        for( ;; )
        {
            p = traits_type::find( p, last - p, s[0] );

            if( p == 0 ) break;
            if( last - p < n ) break;

            if( traits_type::compare( p, s, n ) == 0 ) return p - data();

            ++p;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find( Ch const* s, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        return find( s, pos, traits_type::length( s ) );
    }

    // rfind

    BOOST_CONSTEXPR size_type rfind( basic_string_view str, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        return rfind( str.data(), pos, str.size() );
    }

    BOOST_CONSTEXPR size_type rfind( Ch c, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        if( pos > size() )
        {
            pos = size();
        }

        const_reverse_iterator r = std::find( rbegin() + pos, rend(), c );
        return r == rend()? npos: size() - 1 - ( r - rbegin() );
    }

    BOOST_CONSTEXPR size_type rfind( Ch const* s, size_type pos, size_type n ) const BOOST_NOEXCEPT
    {
        if( n > size() ) return npos;

        if( pos > size() - n )
        {
            pos = size() - n;
        }

        if( n == 0 ) return pos;

        for( ;; )
        {
            size_type xpos = rfind( s[0], pos );

            if( xpos == npos ) return npos;

            if( traits_type::compare( data() + xpos, s, n ) == 0 ) return xpos;

            if( xpos == 0 ) return npos;

            pos = xpos - 1;
        }
    }

    BOOST_CONSTEXPR size_type rfind( Ch const* s, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        return rfind( s, pos, traits_type::length( s ) );
    }

    // find_first_of

    BOOST_CONSTEXPR size_type find_first_of( basic_string_view str, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            if( str.contains( p_[ i ] ) ) return i;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find_first_of( Ch c, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        return find( c, pos );
    }

    BOOST_CONSTEXPR size_type find_first_of( Ch const* s, size_type pos, size_type n ) const BOOST_NOEXCEPT
    {
        return find_first_of( basic_string_view( s, n ), pos );
    }

    BOOST_CONSTEXPR size_type find_first_of( Ch const* s, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        return find_first_of( basic_string_view( s ), pos );
    }

    // find_last_of

    BOOST_CONSTEXPR size_type find_last_of( basic_string_view str, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        if( pos > size() )
        {
            pos = size();
        }

        for( std::size_t i = pos; i > 0; --i )
        {
            if( str.contains( p_[ i - 1 ] ) ) return i - 1;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find_last_of( Ch c, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        return rfind( c, pos );
    }

    BOOST_CONSTEXPR size_type find_last_of( Ch const* s, size_type pos, size_type n ) const BOOST_NOEXCEPT
    {
        return find_last_of( basic_string_view( s, n ), pos );
    }

    BOOST_CONSTEXPR size_type find_last_of( Ch const* s, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        return find_last_of( basic_string_view( s ), pos );
    }

    // find_first_not_of

    BOOST_CONSTEXPR size_type find_first_not_of( basic_string_view str, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            if( !str.contains( p_[ i ] ) ) return i;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find_first_not_of( Ch c, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        for( std::size_t i = pos; i < n_; ++i )
        {
            if( p_[ i ] != c ) return i;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find_first_not_of( Ch const* s, size_type pos, size_type n ) const BOOST_NOEXCEPT
    {
        return find_first_not_of( basic_string_view( s, n ), pos );
    }

    BOOST_CONSTEXPR size_type find_first_not_of( Ch const* s, size_type pos = 0 ) const BOOST_NOEXCEPT
    {
        return find_first_not_of( basic_string_view( s ), pos );
    }

    // find_last_not_of

    BOOST_CONSTEXPR size_type find_last_not_of( basic_string_view str, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        if( pos > size() )
        {
            pos = size();
        }

        for( std::size_t i = pos; i > 0; --i )
        {
            if( !str.contains( p_[ i - 1 ] ) ) return i - 1;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find_last_not_of( Ch c, size_type pos = npos ) const BOOST_NOEXCEPT
    {
        if( pos > size() )
        {
            pos = size();
        }

        for( std::size_t i = pos; i > 0; --i )
		{
            if( p_[ i - 1 ] != c ) return i - 1;
        }

        return npos;
    }

    BOOST_CONSTEXPR size_type find_last_not_of( Ch const* s, size_type pos, size_type n ) const BOOST_NOEXCEPT
    {
        return find_last_not_of( basic_string_view( s, n ), pos );

    {
        return find( sv ) != npos;
    }

    BOOST_CONSTEXPR bool contains( Ch c ) const BOOST_NOEXCEPT
    {
        return traits_type::find( data(), size(), c ) == 0;
    }

    BOOST_CONSTEXPR bool contains( Ch const* s ) const BOOST_NOEXCEPT
    {
        return find( s ) != npos;
    }
};

#if defined(BOOST_NO_CXX17_INLINE_VARIABLES)
template<class Ch> std::size_t const basic_string_view<Ch>::npos;
#endif

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;

#if !defined(BOOST_NO_CXX11_CHAR16_T)
typedef basic_string_view<char16_t> u16string_view;
#endif

#if !defined(BOOST_NO_CXX11_CHAR32_T)
typedef basic_string_view<char32_t> u32string_view;
#endif

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
typedef basic_string_view<char8_t> u8string_view;
#endif

} // namespace core
} // namespace boost

#endif  // #ifndef BOOST_CORE_STRING_VIEW_HPP_INCLUDED