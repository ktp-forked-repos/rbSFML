/* rbSFML
 * Copyright (c) 2015 Henrik Valter Vogelius Hansson - groogy@groogy.se
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

#ifndef RBSFML_RBBLENDMODE_HPP
#define RBSFML_RBBLENDMODE_HPP

#include <SFML/Graphics/BlendMode.hpp>
#include "class.hpp"
#include "object.hpp"

class rbBlendMode;

typedef rb::Module<sf::BlendMode> rbBlendModeModule;

class rbBlendMode
{
public:
	static void defineModule(const rb::Value& sfml);

private:
	static rbBlendModeModule ourDefinition;
};

namespace rb
{
	template<>
	sf::BlendMode::Factor Value::to() const;

	template<>
	Value Value::create( sf::BlendMode::Factor value );

	template<>
	sf::BlendMode::Equation Value::to() const;

	template<>
	Value Value::create( sf::BlendMode::Equation value );	
}

#endif // RBSFML_RBBLENDMODE_HPP