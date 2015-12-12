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

#include "rbcolor.hpp"
#include "error.hpp"
#include "macros.hpp"

namespace 
{
	constexpr char symVarR[] = "@r";
	constexpr char symVarG[] = "@g";
	constexpr char symVarB[] = "@b";
	constexpr char symVarA[] = "@a";

	constexpr char symInspect[] = "inspect";

	constexpr char symAdd[] = "+";
	constexpr char symSubtract[] = "-";
	constexpr char symMultiply[] = "*";
	constexpr char symDivide[] = "/";

	template<const char* operation, const char* varR = symVarR, const char* varG = symVarG,
			 const char* varB = symVarG, const char* varA = symVarA>
	void doMath(rb::Value result, const rb::Value& left, const rb::Value& right)
	{
		rb::Value leftR = left.getVar<varR>();
		rb::Value leftG = left.getVar<varR>();
		rb::Value leftB = left.getVar<varR>();
		rb::Value leftA = left.getVar<varR>();
		rb::Value rightR = right.getVar<varR>();
		rb::Value rightG = right.getVar<varG>();
		rb::Value rightB = right.getVar<varB>();
		rb::Value rightA = right.getVar<varA>();

		rb::Value r = leftR.call<operation>(rightR);
		rb::Value g = leftG.call<operation>(rightG);
		rb::Value b = leftB.call<operation>(rightB);
		rb::Value a = leftA.call<operation>(rightA);

		result.setVar<varR>(r);
		result.setVar<varG>(g);
		result.setVar<varB>(b);
		result.setVar<varA>(a);
	}

	template<const char* operation, const char* varR = symVarR, const char* varG = symVarG,
			 const char* varB = symVarG, const char* varA = symVarA>
	void doMath(rb::Value result, const rb::Value& left, int right)
	{
		rb::Value leftR = left.getVar<varR>();
		rb::Value leftG = left.getVar<varR>();
		rb::Value leftB = left.getVar<varR>();
		rb::Value leftA = left.getVar<varR>();

		rb::Value rightValue(right);
		
		rb::Value r = leftR.call<operation>(rightValue);
		rb::Value g = leftG.call<operation>(rightValue);
		rb::Value b = leftB.call<operation>(rightValue);
		rb::Value a = leftA.call<operation>(rightValue);

		result.setVar<varR>(r);
		result.setVar<varG>(g);
		result.setVar<varB>(b);
		result.setVar<varA>(a);
	}
}

rbColorClass rbColor::ourDefinition;

void rbColor::defineClass(const rb::Value& sfml)
{
	ourDefinition = rbColorClass::defineClassUnder<rb::RubyObjAllocator>("Color", sfml);
	ourDefinition.defineMethod<0>("initialize", &rbColor::initialize);
	ourDefinition.defineMethod<1>("initialize_copy", &rbColor::initializeCopy);
	ourDefinition.defineMethod<2>("marshal_dump", &rbColor::marshalDump);
	ourDefinition.defineMethod<3>("marshal_load", &rbColor::marshalLoad);
	ourDefinition.defineMethod<4>("to_i", &rbColor::toInteger);
	ourDefinition.defineMethod<5>("+", &rbColor::add);
	ourDefinition.defineMethod<6>("-", &rbColor::subtract);
	ourDefinition.defineMethod<7>("*", &rbColor::multiply);
	ourDefinition.defineMethod<8>("==", &rbColor::equal);
	ourDefinition.defineMethod<9>("eql?", &rbColor::strictEqual);
	ourDefinition.defineMethod<10>("inspect", &rbColor::inspect);

	ourDefinition.defineAttribute("r", true, true);
	ourDefinition.defineAttribute("g", true, true);
	ourDefinition.defineAttribute("b", true, true);
	ourDefinition.defineAttribute("a", true, true);

	ourDefinition.aliasMethod("eql?", "equal?");
	ourDefinition.aliasMethod("inspect", "to_s");

	rb::Value black = ourDefinition.newObject(rb::Value::create(sf::Color::Black));
	rb::Value white = ourDefinition.newObject(rb::Value::create(sf::Color::White));
	rb::Value red = ourDefinition.newObject(rb::Value::create(sf::Color::Red));
	rb::Value green = ourDefinition.newObject(rb::Value::create(sf::Color::Green));
	rb::Value blue = ourDefinition.newObject(rb::Value::create(sf::Color::Blue));
	rb::Value yellow = ourDefinition.newObject(rb::Value::create(sf::Color::Yellow));
	rb::Value magenta = ourDefinition.newObject(rb::Value::create(sf::Color::Magenta));
	rb::Value cyan = ourDefinition.newObject(rb::Value::create(sf::Color::Cyan));
	rb::Value transparent = ourDefinition.newObject(rb::Value::create(sf::Color::Transparent));

	black.freeze(); white.freeze(); red.freeze(); green.freeze();
	blue.freeze(); yellow.freeze(); magenta.freeze(); cyan.freeze();
	transparent.freeze();

	ourDefinition.defineConstant("Black", black);
	ourDefinition.defineConstant("White", white);
	ourDefinition.defineConstant("Red", red);
	ourDefinition.defineConstant("Green", green);
	ourDefinition.defineConstant("Blue", blue);
	ourDefinition.defineConstant("Yellow", yellow);
	ourDefinition.defineConstant("Magenta", magenta);
	ourDefinition.defineConstant("Cyan", cyan);
	ourDefinition.defineConstant("Transparent", transparent);
}

const rbColorClass& rbColor::getDefinition()
{
	return ourDefinition;
}

rb::Value rbColor::initialize(rb::Value self, const std::vector<rb::Value>& args)
{
	switch( args.size() )
    {
        case 0:
        	self.setVar<symVarR>(0);
        	self.setVar<symVarG>(0);
        	self.setVar<symVarB>(0);
        	self.setVar<symVarA>(255);
            break;
        case 1:
        	if(args[0].getType() == rb::ValueType::Array)
        	{
        		const std::vector<rb::Value>& elements = args[0].to<const std::vector<rb::Value>&>();
        		self.setVar<symVarR>(elements[0]);
	        	self.setVar<symVarG>(elements[1]);
	        	self.setVar<symVarB>(elements[2]);
	        	self.setVar<symVarA>(elements[3]);
        	}
        	else if(args[0].getType() == rb::ValueType::Fixnum)
        	{
        		sf::Color color(args[0].to<sf::Uint32>());
        		self.setVar<symVarR>(color.r);
        		self.setVar<symVarG>(color.g);
        		self.setVar<symVarB>(color.b);
        		self.setVar<symVarA>(color.a);
        	}
        	else
        	{
        		rbColor::initializeCopy(self, args[0]);
        	}
            break;
        case 4:
        	self.setVar<symVarA>(args[3]);
        case 3:
        	self.setVar<symVarR>(args[0]);
        	self.setVar<symVarG>(args[1]);
        	self.setVar<symVarB>(args[2]);
        	break;
        default:
        	rb::expectedNumArgs( args.size(), 0, 4 );
        	break;
    }

	return self;
}

rb::Value rbColor::initializeCopy(rb::Value self, const rb::Value& value)
{
	self.setVar<symVarR>(value.getVar<symVarR, rb::Value>());
	self.setVar<symVarG>(value.getVar<symVarG, rb::Value>());
	self.setVar<symVarB>(value.getVar<symVarB, rb::Value>());
	self.setVar<symVarA>(value.getVar<symVarA, rb::Value>());
	return self;
}

std::vector<rb::Value> rbColor::marshalDump(const rb::Value& self)
{
	std::vector<rb::Value> array;
	array.push_back(self.getVar<symVarR, rb::Value>());
    array.push_back(self.getVar<symVarG, rb::Value>());
    array.push_back(self.getVar<symVarB, rb::Value>());
    array.push_back(self.getVar<symVarA, rb::Value>());
    return array;
}

rb::Value rbColor::marshalLoad(rb::Value self, const rb::Value& data)
{
	std::vector<rb::Value> array = data.to<std::vector<rb::Value>>();
	self.setVar<symVarR>(array[0]);
    self.setVar<symVarG>(array[1]);
    self.setVar<symVarB>(array[2]);
    self.setVar<symVarA>(array[3]);
	return rb::Nil;
}

unsigned int rbColor::toInteger(rb::Value self)
{
	sf::Color color = self.to<sf::Color>();
	return color.toInteger();
}

rb::Value rbColor::add(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symAdd>(result, self, other);
	return result;
}

rb::Value rbColor::subtract(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symSubtract>(result, self, other);
	return result;
}

rb::Value rbColor::multiply(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symMultiply>(result, self, other);
	doMath<symDivide>(result, result, 255);
	return result;
}

bool rbColor::equal(const rb::Value& self, const rb::Value& other)
{
	if(	!other.isKindOf(rb::Value(ourDefinition)) && 
		!(other.getType() == rb::ValueType::Array && 
		(other.getArrayLength() == 3 || other.getArrayLength() == 4)))
		return false;

	rb::Value color = ourDefinition.newObject(other);
	if(!self.getVar<symVarR>().equal(color.getVar<symVarR>())) return false;
	if(!self.getVar<symVarG>().equal(color.getVar<symVarG>())) return false;
	if(!self.getVar<symVarB>().equal(color.getVar<symVarB>())) return false;
	if(!self.getVar<symVarA>().equal(color.getVar<symVarA>())) return false;
	return true;
}

bool rbColor::strictEqual(const rb::Value& self, const rb::Value& other)
{
	if(!other.isKindOf(rb::Value(ourDefinition))) return false;
	if(self.getVar<symVarR>().getType() != other.getVar<symVarR>().getType()) return false;
	if(self.getVar<symVarG>().getType() != other.getVar<symVarG>().getType()) return false;
	if(self.getVar<symVarB>().getType() != other.getVar<symVarB>().getType()) return false;
	if(self.getVar<symVarA>().getType() != other.getVar<symVarA>().getType()) return false;
	return equal(self, other);
}

std::string rbColor::inspect(const rb::Value& self)
{
	std::string rStr = self.getVar<symVarR>().call<symInspect, std::string>();
	std::string gStr = self.getVar<symVarG>().call<symInspect, std::string>();
	std::string bStr = self.getVar<symVarB>().call<symInspect, std::string>();
	std::string aStr = self.getVar<symVarA>().call<symInspect, std::string>();
	return ourDefinition.getName() + "(" + rStr + ", " + gStr + ", " + bStr + ", " + aStr + ")";
}

namespace rb
{

template<>
sf::Color Value::to() const
{
	errorHandling(T_OBJECT);
	sf::Color color(getVar<symVarR, unsigned int>(), getVar<symVarG, unsigned int>(),
					 getVar<symVarB, unsigned int>(), getVar<symVarA, unsigned int>());
	return color;
}

template<>
Value Value::create<sf::Color>( sf::Color value )
{
	rb::Value object = rbColor::getDefinition().newObject(value.r, value.g, value.b, value.a);
	return object;
}

template<>
Value Value::create<const sf::Color&>( const sf::Color& value )
{
	rb::Value object = rbColor::getDefinition().newObject(value.r, value.g, value.b, value.a);
	return object;
}

}