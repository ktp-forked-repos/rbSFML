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

#include <ruby.h>
#include "module.hpp"
#include <iostream>

class rbSFML
{
public:
	static void blah()
	{
		std::cout << "EEEY!" << std::endl;
	}

	static std::string say(const std::string& arg)
	{
		std::cout << arg << std::endl;
		return "Leol!";
	}
};

extern "C" void Init_rbsfml() {
	rb::Module<rbSFML> sfml("SFML");
	sfml.defineFunction<0>("blah", &rbSFML::blah);
	sfml.defineFunction<1>("say", &rbSFML::say);
}