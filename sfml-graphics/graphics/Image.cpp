/* rbSFML - Copyright (c) 2010 Henrik Valter Vogelius Hansson - groogy@groogy.se
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    If you use this software in a product, an acknowledgment
 *    in the product documentation would be appreciated but
 *    is not required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any
 *    source distribution.
 */
 
#include "Image.hpp"
#include "Color.hpp"
#include "Rect.hpp"
#include "main.hpp"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>

VALUE globalImageClass;

/* External classes */
extern VALUE globalColorClass;
extern VALUE globalRectClass;
extern VALUE globalInputStreamClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Image_Free( sf::Image *anObject )
{
	delete anObject;
}

/* call-seq:
 *   image.loadFromFile( filename )	-> true or false
 *
 * Load the image from a file on disk.
 *
 * The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic. Some format options
 * are not supported, like progressive jpeg. If this function fails, the image is left unchanged.
 */
static VALUE Image_LoadFromFile( VALUE self, VALUE aFileName )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	if( object->LoadFromFile( rb_string_value_cstr( &aFileName ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   image.loadFromStream( stream )	-> true or false
 *
 * Load the image from a custom stream.
 *
 * The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic. Some format options
 * are not supported, like progressive jpeg. If this function fails, the image is left unchanged.
 */
static VALUE Image_LoadFromStream( VALUE self, VALUE aStream )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	VALIDATE_CLASS( aStream, globalInputStreamClass, "stream" );
	sf::InputStream *stream = NULL;
	Data_Get_Struct( self, sf::InputStream, stream );
	if( object->LoadFromStream( *stream ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   image.saveToFile( filename )	-> true or false
 *
 * Save the image to a file on disk.
 *
 * The format of the image is automatically deduced from the extension. The supported image formats are bmp, png, 
 * tga, jpg, dds and psd. The destination file is overwritten if it already exists.
 */
static VALUE Image_SaveToFile( VALUE self, VALUE aFileName )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	if( object->SaveToFile( rb_string_value_cstr( &aFileName ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static void priv_CreateFromPixels( sf::Image *self, unsigned int aWidth, unsigned int aHeight, VALUE somePixels )
{
	const unsigned long dataSize = aWidth * aHeight * 4;
	VALUE pixels = rb_funcall( somePixels, rb_intern("flatten"), 0 );
	if (RARRAY_LEN(pixels) < dataSize)
		return;
		
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	sf::Uint8 * const tempData = new sf::Uint8[dataSize];
	for(unsigned long index = 0; index < dataSize; index++)
	{
		sf::Uint8 val = NUM2CHR( rb_ary_entry( pixels, index ) );
		tempData[index] = val;
	}
	
	object->Create( aWidth, aHeight, tempData );
	delete[] tempData;
}

/* call-seq:
 *   image.create( width, height, color = SFML::Color::Black )
 *   image.create( width, height, pixels )
 *
 * Create the image and fill it with a unique color or array of pixels.
 *
 * The pixels argument must point to an array of 32 bits RGBA pixels. In other words, the pixel array must have 
 * this memory layout:
 *
 * [r0 g0 b0 a0 r1 g1 b1 a1 r2...]
 */
static VALUE Image_Create( int argc, VALUE *args, VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	unsigned int width = 0;
	unsigned int height = 0;
	VALUE rubyColor = Qnil;
	sf::Color color;
	
	switch( argc )
	{
		case 3:
			if( rb_obj_is_kind_of( args[ 2 ], rb_cArray ) == Qtrue )
			{
				priv_CreateFromPixels( object, FIX2UINT( args[ 0 ] ), FIX2UINT( args[ 1 ] ), args[ 2 ] );
				return Qnil;
			}
			else
			{
				color = Color_ToSFML( Color_ForceType( args[ 2 ] ) );
			}
		case 2:
			width = FIX2UINT( args[0] );
			height = FIX2UINT( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2 or 3 arguments but was given %d", argc );
	}
	
	object->Create( width, height, color );
	return Qnil;
}

/* call-seq:
 *   image.createMaskFromColor( color, alpha = 0 )
 *
 * Create a transparency mask from a specified colorkey.
 *
 * This function sets the alpha value of every pixel matching the given color to alpha (0 by default), 
 * so that they become transparent.
 */
static VALUE Image_CreateMaskFromColor( int argc, VALUE *args, VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	sf::Uint8 alpha = 0;
	VALUE rubyColor = Qnil;
	sf::Color color;
	
	switch( argc )
	{
		case 2:
			alpha = FIX2UINT( alpha );
		case 1:
			rubyColor = Color_ForceType( args[0] );
			color.r = FIX2INT( Color_GetR( rubyColor ) );
			color.g = FIX2INT( Color_GetG( rubyColor ) );
			color.b = FIX2INT( Color_GetB( rubyColor ) );
			color.a = FIX2INT( Color_GetA( rubyColor ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	object->CreateMaskFromColor( color, alpha );
	return Qnil;
}

/* call-seq:
 *   image.copy( source, destX, destY, sourceRect = [0, 0, 0, 0], applyAlpha = false )
 *
 * Copy pixels from another image onto this one.
 *
 * This function does a slow pixel copy and should only be used at initialization time. It can be used to prepare 
 * a complex static image from several others, but if you need this kind of feature in real-time you'd better use 
 * SFML::RenderImage. If sourceRect is empty, the whole image is copied. If applyAlpha is set to true, the 
 * transparency of source pixels is applied. If it is false, the pixels are copied unchanged with their alpha value.
 */
static VALUE Image_Copy( int argc, VALUE *args, VALUE self )
{
	sf::Image *source;
	unsigned int destX = 0;
	unsigned int destY = 0;
	sf::IntRect sourceRect = sf::IntRect(0, 0, 0, 0);
	VALUE rubySourceRect = Qnil;
	bool applyAlpha = false;
	
	switch( argc )
	{
		case 5:
			if( args[4] == Qtrue )
			{
				applyAlpha = true;
			}
			else if( args[4] == Qfalse )
			{
				applyAlpha = false;
			}
			else
			{
				VALIDATE_CLASS( args[4], rb_cTrueClass, "applyAlpha" );
			}
		case 4:
			rubySourceRect = Rect_ForceType( args[3] );
			sourceRect.Left = FIX2INT( Rect_GetLeft( rubySourceRect ) );
			sourceRect.Top = FIX2INT( Rect_GetTop( rubySourceRect ) );
			sourceRect.Width = FIX2INT( Rect_GetWidth( rubySourceRect ) );
			sourceRect.Height = FIX2INT( Rect_GetHeight( rubySourceRect ) );
		case 3:
			VALIDATE_CLASS( args[0], globalImageClass, "source" );
			Data_Get_Struct( args[0], sf::Image, source );
			destX = FIX2UINT( args[1] );
			destX = FIX2UINT( args[2] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 3..5 arguments but was given %d", argc );
	}
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->Copy( *source, destX, destY, sourceRect, applyAlpha );
	return Qnil;
}

/* call-seq:
 *   image.setPixel( x, y, color )
 *
 * Change the color of a pixel.
 *
 * This function doesn't check the validity of the pixel coordinates, using out-of-range values will 
 * result in an undefined behaviour.
 */
static VALUE Image_SetPixel( VALUE self, VALUE aX, VALUE aY, VALUE aColor )
{
	VALUE rbColor = Color_ForceType( aColor );
	sf::Color color;
	color.r = FIX2INT( Color_GetR( rbColor ) );
	color.g = FIX2INT( Color_GetG( rbColor ) );
	color.b = FIX2INT( Color_GetB( rbColor ) );
	color.a = FIX2INT( Color_GetA( rbColor ) );
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->SetPixel( FIX2INT( aX ), FIX2INT( aY ), color );
	return Qnil;
}

/* call-seq:
 *   image.getPixel( x, y )	-> color
 *
 * Get the color of a pixel.
 *
 * This function doesn't check the validity of the pixel coordinates, using out-of-range values will 
 * result in an undefined behaviour.
 */
static VALUE Image_GetPixel( VALUE self, VALUE aX, VALUE aY )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	const sf::Color color = object->GetPixel( FIX2INT( aX ), FIX2INT( aY ) );
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4,
				INT2FIX( color.r ), INT2FIX( color.g ),
				INT2FIX( color.b ), INT2FIX( color.a ) );
}

/* call-seq:
 *   image.getPixelsPtr()	-> array of pixels
 *
 * Get a read-only pointer to the array of pixels.
 *
 * The returned value points to an array of RGBA pixels made of 8 bits integers components. 
 * The size of the array is width * height * 4. Warning: the returned pointer may become invalid if 
 * you modify the image, so you should never store it for too long.
 */
static VALUE Image_GetPixelsPtr( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	const unsigned int rawWidth = object->GetWidth();
	const unsigned int rawHeight = object->GetHeight();
	const unsigned long dataSize = rawWidth * rawHeight * 4;
	
	VALUE pixels = rb_ary_new2( dataSize );
	const sf::Uint8 *const pixelPointer = object->GetPixelsPtr();
	for(unsigned long index = 0; index < dataSize; index++)
	{
		rb_ary_store( pixels, index, CHR2FIX( pixelPointer[index] ) );
	}
	
	return pixels;
}

/* call-seq:
 *   image.getWidth()	-> width
 *
 * Return the width of the image. 
 */
static VALUE Image_GetWidth( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	return INT2FIX( object->GetWidth() );
}

/* call-seq:
 *   image.getHeight()	-> height
 *
 * Return the height of the image. 
 */
static VALUE Image_GetHeight( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	return INT2FIX( object->GetHeight() );
}

/* call-seq:
 *   Image.new()						-> image
 *   Image.new( filename )				-> image
 *   Image.new( width, height, pixels )	-> image
 *   Image.new( width, height, color ) -> image
 *
 * Will create a new image instance.
 * 
 * If a filename argument is specified then Image#loadFromFile will be called on the created instance. If width, height
 * and pixels are specified then Image#loadFromPixels will be called on the created instance.  If width, height
 * and a color are specified then Image#create will be called on the created instance.
 */
static VALUE Image_Initialize( int argc, VALUE *args, VALUE self )
{
	switch( argc )
	{
	case 3:
		rb_funcall2( self, rb_intern( "create" ), argc, args );
		break;
	case 1:
		rb_funcall2( self, rb_intern( "loadFromFile" ), argc, args );
		break;
	case 0:
		break;
	default:
		rb_raise( rb_eArgError, "Expected 0, 1 or 3 arguments but was given %d", argc );
	}
	return self;
}

static VALUE Image_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	sf::Image *source = NULL;
	Data_Get_Struct( aSource, sf::Image, source );
	*object = *source;
}

static VALUE Image_FlipHorizontally( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->FlipHorizontally();
	return Qnil;
}

static VALUE Image_FlipVertically( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->FlipVertically();
	return Qnil;
}

static VALUE Image_Alloc( VALUE aKlass )
{
	sf::Image *object = new sf::Image();
	return Data_Wrap_Struct( aKlass, 0, Image_Free, object );
}

void Init_Image( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Class for loading, manipulating and saving images.
 *
 * sf::Image is an abstraction to manipulate images as bidimensional arrays of pixels.
 *
 * The class provides functions to load, read, write and save pixels, as well as many other
 * useful functions.
 *
 * sf::Image can handle a unique internal representation of pixels, which is RGBA 32 bits. This
 * means that a pixel must be composed of 8 bits red, green, blue and alpha channels -- just like
 * a sf::Color. All the functions that return an array of pixels follow this rule, and all 
 * parameters that you pass to sf::Image functions (such as LoadFromPixels) must use this 
 * representation as well.
 *
 * A sf::Image can be copied, but it is a heavy resource and if possible you should always use 
 * [const] references to pass or return them to avoid useless copies.
 *
 * Usage example:
 *
 *   # Load an image file from a file
 *   background = SFML::Image.new
 *   if background.load_from_file( "background.jpg" ) == false
 *     return -1
 *
 *   # Create a 20x20 image filled with black color
 *   image = sf::Image.new( 20, 20, sf::Color::Black )
 *
 *   # Copy image1 on image2 at position (10, 10)
 *   image.copy( background, 10, 10 )
 *
 *   # Make the top-left pixel transparent
 *   color = image.get_pixel( 0, 0 );
 *   color.a = 0
 *   image.set_pixel( 0, 0, color )
 *
 *   # Save the image to a file
 *   if image.save_to_file( "result.png" ) == false
 *     return -1
 */
	globalImageClass = rb_define_class_under( sfml, "Image", rb_cObject );
	
	// Class methods
	rb_define_alloc_func( globalImageClass, Image_Alloc );
	
	// Instance methods
	rb_define_method( globalImageClass, "initialize", Image_Initialize, -1 );
	rb_define_method( globalImageClass, "initialize_copy", Image_InitializeCopy, 1 );
	rb_define_method( globalImageClass, "loadFromFile", Image_LoadFromFile, 1 );
	rb_define_method( globalImageClass, "saveToFile", Image_SaveToFile, 1 );
	rb_define_method( globalImageClass, "create", Image_Create, -1 );
	rb_define_method( globalImageClass, "createMaskFromColor", Image_CreateMaskFromColor, -1 );
	rb_define_method( globalImageClass, "copy", Image_Copy, -1 );
	rb_define_method( globalImageClass, "setPixel", Image_SetPixel, 3 );
	rb_define_method( globalImageClass, "getPixel", Image_GetPixel, 2 );
	rb_define_method( globalImageClass, "getPixelsPtr", Image_GetPixelsPtr, 0 );
	rb_define_method( globalImageClass, "getWidth", Image_GetWidth, 0 );
	rb_define_method( globalImageClass, "getHeight", Image_GetHeight, 0 );
	rb_define_method( globalImageClass, "flipHorizontally", Image_FlipHorizontally, 0 );
	rb_define_method( globalImageClass, "flipVertically", Image_FlipVertically , 0 );
	
	// Instance Aliases
	rb_define_alias( globalImageClass, "load_from_file", "loadFromFile");
	rb_define_alias( globalImageClass, "loadFile", "loadFromFile");
	rb_define_alias( globalImageClass, "load_file", "loadFromFile");

	rb_define_alias( globalImageClass, "save_to_file", "saveToFile");
	rb_define_alias( globalImageClass, "save", "saveToFile");
	
	rb_define_alias( globalImageClass, "create_mask_from_color", "createMaskFromColor");
	rb_define_alias( globalImageClass, "create_mask", "createMaskFromColor");
	rb_define_alias( globalImageClass, "createMask", "createMaskFromColor");
	
	rb_define_alias( globalImageClass, "set_pixel", "setPixel");
	rb_define_alias( globalImageClass, "get_pixel", "getPixel");
	rb_define_alias( globalImageClass, "get_pixels_ptr", "getPixelsPtr");
	rb_define_alias( globalImageClass, "pixelsPtr", "getPixelsPtr");
	rb_define_alias( globalImageClass, "pixels_ptr", "getPixelsPtr");
	rb_define_alias( globalImageClass, "getPixels", "getPixelsPtr");
	rb_define_alias( globalImageClass, "get_pixels", "getPixelsPtr");
	rb_define_alias( globalImageClass, "pixels", "getPixelsPtr");
	
	rb_define_alias( globalImageClass, "get_width", "getWidth");
	rb_define_alias( globalImageClass, "width", "getWidth");
	rb_define_alias( globalImageClass, "get_height", "getHeight");
	rb_define_alias( globalImageClass, "height", "getHeight");
	
	rb_define_alias( globalImageClass, "flip_horizontally", "flipHorizontally" );
	rb_define_alias( globalImageClass, "flip_vertically", "flipVertically" );
}
