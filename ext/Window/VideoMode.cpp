/* rbSFML
 * Copyright (c) 2010 Henrik Valter Vogelius Hansson - groogy@groogy.se
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#define WINDOW_VIDEOMODE_CPP
#include "VideoMode.hpp"

void rbVideoMode::Init( VALUE SFML )
{
    rbVideoMode::Class = rb_define_class_under( SFML, "VideoMode", rb_cObject );
    rb_include_module( rbVideoMode::Class, rb_mComparable );
    
    // Class methods
    rb_define_alloc_func( rbVideoMode::Class, rbMacros::Allocate< sf::VideoMode > );
    rb_define_class_method( rbVideoMode::Class, "desktop_mode",     rbVideoMode::GetDesktopMode,     0 );
    rb_define_class_method( rbVideoMode::Class, "fullscreen_modes", rbVideoMode::GetFullscreenModes, 0 );
    
    // Instance methods
    rb_define_method( rbVideoMode::Class, "initialize",      rbVideoMode::Initialize,     -1 );
    rb_define_method( rbVideoMode::Class, "initialize_copy", rbVideoMode::InitializeCopy,  1 );
    rb_define_method( rbVideoMode::Class, "marshal_dump",    rbVideoMode::MarshalDump,     0 );
    rb_define_method( rbVideoMode::Class, "marshal_load",    rbVideoMode::MarshalLoad,     1 );
    rb_define_method( rbVideoMode::Class, "valid?",          rbVideoMode::IsValid,         0 );
    rb_define_method( rbVideoMode::Class, "width",           rbVideoMode::GetWidth,        0 );
    rb_define_method( rbVideoMode::Class, "height",          rbVideoMode::GetHeight,       0 );
    rb_define_method( rbVideoMode::Class, "bpp",             rbVideoMode::GetBitsPerPixel, 0 );
    rb_define_method( rbVideoMode::Class, "width=",          rbVideoMode::SetWidth,        1 );
    rb_define_method( rbVideoMode::Class, "height=",         rbVideoMode::SetHeight,       1 );
    rb_define_method( rbVideoMode::Class, "bpp=",            rbVideoMode::SetBitsPerPixel, 1 );
    rb_define_method( rbVideoMode::Class, "<=>",             rbVideoMode::Compare,         1 );
    rb_define_method( rbVideoMode::Class, "inspect",         rbVideoMode::Inspect,         0 );
    rb_define_method( rbVideoMode::Class, "memory_usage",    rbVideoMode::GetMemoryUsage,  0 );
    
    // Class aliasses
    VALUE sVideoMode = rb_singleton_class( rbVideoMode::Class );
    rb_define_alias( sVideoMode, "GetDesktopMode",     "desktop_mode"     );
    rb_define_alias( sVideoMode, "GetFullscreenModes", "fullscreen_modes" );
    
    // Instance aliasses
    rb_define_alias( rbVideoMode::Class, "IsValid",         "valid?"  );
    rb_define_alias( rbVideoMode::Class, "Width",           "width"   );
    rb_define_alias( rbVideoMode::Class, "Height",          "height"  );
    rb_define_alias( rbVideoMode::Class, "BitsPerPixel",    "bpp"     );
    rb_define_alias( rbVideoMode::Class, "bits_per_pixel",  "bpp"     );
    rb_define_alias( rbVideoMode::Class, "bits",            "bpp"     );
    rb_define_alias( rbVideoMode::Class, "Width=",          "width="  );
    rb_define_alias( rbVideoMode::Class, "Height=",         "height=" );
    rb_define_alias( rbVideoMode::Class, "BitsPerPixel=",   "bpp="    );
    rb_define_alias( rbVideoMode::Class, "bits_per_pixel=", "bpp="    );
    rb_define_alias( rbVideoMode::Class, "bits=",           "bpp="    );
    rb_define_alias( rbVideoMode::Class, "eql?",            "=="      );
    rb_define_alias( rbVideoMode::Class, "equal?",          "=="      );
    rb_define_alias( rbVideoMode::Class, "to_s",            "inspect" );
}

// VideoMode::desktop_mode
// VideoMode::GetDesktopMode
VALUE rbVideoMode::GetDesktopMode( VALUE aSelf )
{
    VALUE videoMode = rbMacros::Allocate< sf::VideoMode >( rbVideoMode::Class );
    *rbVideoMode::ToSFML( videoMode ) = sf::VideoMode::GetDesktopMode();
    return videoMode;
}

// VideoMode::fullscreen_modes
// VideoMode::GetFullscreenModes
VALUE rbVideoMode::GetFullscreenModes( VALUE aSelf )
{
    const std::vector< sf::VideoMode >& modes = sf::VideoMode::GetFullscreenModes();
    VALUE ary = rb_ary_new2( modes.size() );
    for (std::size_t index = 0; index < modes.size(); index++ )
    {
        VALUE videoMode = rbMacros::Allocate< sf::VideoMode >( rbVideoMode::Class );
        *ToSFML( videoMode ) = modes[ index ];
        rb_ary_push( ary, videoMode );
    }
    return ary;
}

// VideoMode#initialize
VALUE rbVideoMode::Initialize( int argc, VALUE argv[], VALUE aSelf )
{
    sf::VideoMode* videoMode = rbVideoMode::ToSFML( aSelf );
  
    switch( argc )
    {
        case 0:
            videoMode->Width = 0;
            videoMode->Height = 0;
            videoMode->BitsPerPixel = 0;
            break;
        case 1:
            InitializeCopy( aSelf, ToRuby( argv[ 0 ] ) );
            break;
        case 2:
            videoMode->Width = NUM2UINT( argv[ 0 ] );
            videoMode->Height = NUM2UINT( argv[ 1 ] );
            videoMode->BitsPerPixel = 32;
            break;
        case 3:
            videoMode->Width = NUM2UINT( argv[ 0 ] );
            videoMode->Height = NUM2UINT( argv[ 1 ] );
            videoMode->BitsPerPixel = NUM2UINT( argv[ 2 ] );
            break;
        default:
            rb_raise( rb_eArgError,
                      "wrong number of arguments(%i for 0..3)", argc );
    }
    
    return Qnil;
}

// VideoMode#initialize_copy(video_mode)
VALUE rbVideoMode::InitializeCopy( VALUE aSelf, VALUE aVideoMode )
{
    *rbVideoMode::ToSFML( aSelf ) = *rbVideoMode::ToSFML( aVideoMode );
    return aSelf;
}

// VideoMode#marshal_dump
VALUE rbVideoMode::MarshalDump( VALUE aSelf )
{
    sf::VideoMode* videoMode = rbVideoMode::ToSFML( aSelf );
    
    VALUE ptr[ 3 ];
    ptr[ 0 ] = UINT2NUM( videoMode->Width );
    ptr[ 1 ] = UINT2NUM( videoMode->Height );
    ptr[ 2 ] = UINT2NUM( videoMode->BitsPerPixel );
    return rb_ary_new4( 3, ptr );
}

// VideoMode#marshal_load(data)
VALUE rbVideoMode::MarshalLoad( VALUE aSelf, VALUE someData )
{
    sf::VideoMode* videoMode = rbVideoMode::ToSFML( aSelf );
    
    VALUE* ptr = RARRAY_PTR( someData );
    videoMode->Width        = NUM2UINT( ptr[ 0 ] );
    videoMode->Height       = NUM2UINT( ptr[ 1 ] );
    videoMode->BitsPerPixel = NUM2UINT( ptr[ 2 ] );
    return Qnil;
}

// VideoMode#valid?
// VideoMode#IsValid
VALUE rbVideoMode::IsValid( VALUE aSelf )
{
    return RBOOL( rbVideoMode::ToSFML( aSelf )->IsValid() );
}

// VideoMode#width
// VideoMode#Width
VALUE rbVideoMode::GetWidth( VALUE aSelf )
{
    return UINT2NUM( rbVideoMode::ToSFML( aSelf )->Width );
}

// VideoMode#height
// VideoMode#Height
VALUE rbVideoMode::GetHeight( VALUE aSelf )
{
    return UINT2NUM( rbVideoMode::ToSFML( aSelf )->Height );
}

// VideoMode#bpp
// VideoMode#BitsPerPixel
// VideoMode#bits_per_pixel
// VideoMode#bits
VALUE rbVideoMode::GetBitsPerPixel( VALUE aSelf )
{
    return UINT2NUM( rbVideoMode::ToSFML( aSelf )->BitsPerPixel );
}

// VideoMode#width=(value)
// VideoMode#Width=(value)
VALUE rbVideoMode::SetWidth( VALUE aSelf, VALUE aValue )
{
    rb_check_frozen( aSelf );
    rbVideoMode::ToSFML( aSelf )->Width = NUM2UINT( aValue );
    return Qnil;
}

// VideoMode#height=(value)
// VideoMode#Height=(value)
VALUE rbVideoMode::SetHeight( VALUE aSelf, VALUE aValue )
{
    rb_check_frozen( aSelf );
    rbVideoMode::ToSFML( aSelf )->Height = NUM2UINT( aValue );
    return Qnil;
}

// VideoMode#bpp=(value)
// VideoMode#BitsPerPixel=(value)
// VideoMode#bits_per_pixel=(value)
// VideoMode#bits=(value)
VALUE rbVideoMode::SetBitsPerPixel( VALUE aSelf, VALUE aValue )
{
    rb_check_frozen( aSelf );
    rbVideoMode::ToSFML( aSelf )->BitsPerPixel = NUM2UINT( aValue );
    return Qnil;
}

// VideoMode#<=>(other)
VALUE rbVideoMode::Compare( VALUE aSelf, VALUE anOther )
{
    sf::VideoMode* vm1 = rbVideoMode::ToSFML( aSelf );
    sf::VideoMode* vm2 = rbVideoMode::ToSFML( anOther );
    if( *vm1 == *vm2 ) return INT2FIX( 0 );
    if( *vm1 > *vm2 ) return INT2FIX( 1 );
    return INT2FIX( -1 );
}

// VideoMode#inspect
// VideoMode#to_s
VALUE rbVideoMode::Inspect( VALUE aSelf )
{
    sf::VideoMode* videoMode = rbVideoMode::ToSFML( aSelf );
    return rb_sprintf( "%s(%ux%u, %u-bits)",
                       rb_obj_classname( aSelf ),
                       videoMode->Width,
                       videoMode->Height,
                       videoMode->BitsPerPixel );
}

// VideoMode#memory_usage
VALUE rbVideoMode::GetMemoryUsage( VALUE aSelf )
{
    return SIZET2NUM( sizeof( sf::VideoMode ) );
}
