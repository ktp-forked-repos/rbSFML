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

#ifndef WINDOW_CONTEXT_HPP
#define WINDOW_CONTEXT_HPP

#include <ruby.h>
#include <rbSFML.hpp>
#include <System/SFML.hpp>
#include <Window/ContextSettings.hpp>

#include <SFML/Window/Context.hpp>

namespace rbContext
{
    
    static inline void Free(void* context);
    
    static inline VALUE ToRuby(VALUE other);
    static inline VALUE ToRuby(sf::Context* context);
    static inline VALUE ToRuby(sf::Context& context);
    static inline sf::Context* ToSFML(VALUE context);
    
    static inline VALUE Allocate(VALUE);
    
#if defined(WINDOW_CONTEXT_CPP)
    VALUE Context;
#else
    extern VALUE Context;
#endif
    
#if defined(RBSFML_WINDOW)
    void Init(VALUE SFML);
#endif
    
#if defined(WINDOW_CONTEXT_CPP)
    // Context#initialize
    // Context#initialize(settings, width, height)
    static VALUE Initialize(int argc, VALUE argv[], VALUE self);
    
    // Context#marshal_dump
    static VALUE MarshalDump(VALUE self);
    
    // Context#clone
    static VALUE Clone(VALUE self);
    
    // Context#dup
    static VALUE Dup(VALUE self);
    
    // Context#active=(active)
    // Context#SetActive(active)
    // Context#active(active)
    static VALUE SetActive(VALUE self, VALUE active);
    
    // Context#memory_usage
    static VALUE GetMemoryUsage(VALUE self);
#endif
    
}

void rbContext::Free(void* context)
{
    delete (sf::Context*)context;
}

VALUE rbContext::Allocate(VALUE)
{
    sf::Context* context = new(std::nothrow) sf::Context;
    if (context == NULL) rb_memerror();
    return ToRuby(context);
}

VALUE rbContext::ToRuby(VALUE other)
{
    if (rb_obj_is_instance_of(other, Context))
        return other;
    else
        rb_raise(rb_eTypeError,
                 "can't convert %s into Context", rb_obj_classname(other));
}

VALUE rbContext::ToRuby(sf::Context* context)
{
    return rb_data_object_alloc(Context, context, NULL, Free);
}

VALUE rbContext::ToRuby(sf::Context& context)
{
    return rb_data_object_alloc(Context, &context, NULL, NULL);
}

sf::Context* rbContext::ToSFML(VALUE context)
{
    context = ToRuby(context);
    return (sf::Context*)DATA_PTR(context);
}

#endif // WINDOW_CONTEXT_HPP
