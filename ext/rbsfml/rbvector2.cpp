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

#include "rbvector2.hpp"
#include "error.hpp"
#include "macros.hpp"

namespace 
{
	constexpr char symVarX[] = "@x";
	constexpr char symVarY[] = "@y";
	constexpr char symNegate[] = "-@";

	constexpr char symInspect[] = "inspect";

	constexpr char symAdd[] = "+";
	constexpr char symSubtract[] = "-";
	constexpr char symMultiply[] = "*";
	constexpr char symDivide[] = "/";

	template<const char* operation, const char* varX = symVarX, const char* varY = symVarY>
	void doMath(rb::Value result, const rb::Value& left, const rb::Value& right)
	{
		rb::Value leftX = left.getVar<varX>();
		rb::Value leftY = left.getVar<varY>();
		rb::Value rightX, rightY;
		if(right.getType() == rb::ValueType::Float || right.getType() == rb::ValueType::Fixnum)
		{
		    rightX = right;
		    rightY = right;
		}
		else
		{
		    rightX = right.getVar<varX>();
            rightY = right.getVar<varY>();
		}

		rb::Value x = leftX.call<operation>(rightX);
		rb::Value y = leftY.call<operation>(rightY);

		result.setVar<varX>(x);
		result.setVar<varY>(y);
	}
}

rbVector2Class rbVector2::ourDefinition;

void rbVector2::defineClass(const rb::Value& sfml)
{
	ourDefinition = rbVector2Class::defineClassUnder<rb::RubyObjAllocator>("Vector2", sfml);
	ourDefinition.defineMethod<0>("initialize", &rbVector2::initialize);
	ourDefinition.defineMethod<1>("initialize_copy", &rbVector2::initializeCopy);
	ourDefinition.defineMethod<2>("marshal_dump", &rbVector2::marshalDump);
	ourDefinition.defineMethod<3>("marshal_load", &rbVector2::marshalLoad);
	ourDefinition.defineMethod<4>("-@", &rbVector2::negate);
	ourDefinition.defineMethod<5>("+", &rbVector2::add);
	ourDefinition.defineMethod<6>("-", &rbVector2::subtract);
	ourDefinition.defineMethod<7>("*", &rbVector2::multiply);
	ourDefinition.defineMethod<8>("/", &rbVector2::divide);
	ourDefinition.defineMethod<9>("==", &rbVector2::equal);
	ourDefinition.defineMethod<10>("eql?", &rbVector2::strictEqual);
	ourDefinition.defineMethod<11>("inspect", &rbVector2::inspect);

	ourDefinition.defineAttribute("x", true, true);
	ourDefinition.defineAttribute("y", true, true);

	ourDefinition.aliasMethod("eql?", "equal?");
	ourDefinition.aliasMethod("inspect", "to_s");

	ourDefinition.defineConstant("Zero", ourDefinition.newObject());
}

const rbVector2Class& rbVector2::getDefinition()
{
	return ourDefinition;
}

rb::Value rbVector2::initialize(rb::Value self, const std::vector<rb::Value>& args)
{
	switch( args.size() )
    {
        case 0:
        	self.setVar<symVarX>(0);
        	self.setVar<symVarY>(0);
            break;
        case 1:
        	if(args[0].getType() == rb::ValueType::Array)
        	{
        		const std::vector<rb::Value>& elements = args[0].to<const std::vector<rb::Value>&>();
        		self.setVar<symVarX>(elements[0]);
	        	self.setVar<symVarY>(elements[1]);
        	}
        	else
        	{
        		rbVector2::initializeCopy(self, args[0]);
        	}
            break;
        case 2:
        	self.setVar<symVarX>(args[0]);
        	self.setVar<symVarY>(args[1]);
            break;
        default:
        	rb::expectedNumArgs( args.size(), 0, 2 );
        	break;
    }

	return self;
}

rb::Value rbVector2::initializeCopy(rb::Value self, const rb::Value& value)
{
	self.setVar<symVarX>(value.getVar<symVarX, rb::Value>());
    self.setVar<symVarY>(value.getVar<symVarY, rb::Value>());
	return self;
}

std::vector<rb::Value> rbVector2::marshalDump(const rb::Value& self)
{
	std::vector<rb::Value> array;
	array.push_back(self.getVar<symVarX, rb::Value>());
    array.push_back(self.getVar<symVarY, rb::Value>());
    return array;
}

rb::Value rbVector2::marshalLoad(rb::Value self, const rb::Value& data)
{
	std::vector<rb::Value> array = data.to<std::vector<rb::Value>>();
	self.setVar<symVarX>(array[0]);
    self.setVar<symVarY>(array[1]);
	return rb::Nil;
}

rb::Value rbVector2::negate(const rb::Value& self)
{
	rb::Value object = ourDefinition.newObject();
	object.setVar<symVarX>(object.getVar<symVarX>().call<symNegate>());
	object.setVar<symVarY>(object.getVar<symVarY>().call<symNegate>());
	return object;
}

rb::Value rbVector2::add(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symAdd>(result, self, other);
	return result;
}

rb::Value rbVector2::subtract(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symSubtract>(result, self, other);
	return result;
}

rb::Value rbVector2::multiply(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symMultiply>(result, self, other);
	return result;
}

rb::Value rbVector2::divide(const rb::Value& self, const rb::Value& other)
{
	rb::Value result = ourDefinition.newObject();
	doMath<symDivide>(result, self, other);
	return result;
}

bool rbVector2::equal(const rb::Value& self, const rb::Value& other)
{
	if(	!other.isKindOf(rb::Value(ourDefinition)) && 
		!(other.getType() == rb::ValueType::Array && other.getArrayLength() == 2))
		return false;

	rb::Value vector = ourDefinition.newObject(other);
	if(!self.getVar<symVarX>().equal(vector.getVar<symVarX>())) return false;
	if(!self.getVar<symVarY>().equal(vector.getVar<symVarY>())) return false;
	return true;
}

bool rbVector2::strictEqual(const rb::Value& self, const rb::Value& other)
{
	if(!other.isKindOf(rb::Value(ourDefinition))) return false;
	if(self.getVar<symVarX>().getType() != other.getVar<symVarX>().getType()) return false;
	if(self.getVar<symVarY>().getType() != other.getVar<symVarY>().getType()) return false;
	return equal(self, other);
}

std::string rbVector2::inspect(const rb::Value& self)
{
	std::string xStr = self.getVar<symVarX>().call<symInspect, std::string>();
	std::string yStr = self.getVar<symVarY>().call<symInspect, std::string>();
	return ourDefinition.getName() + "(" + xStr + ", " + yStr + ")";
}

namespace rb
{

template<>
sf::Vector2f Value::to() const
{
    if(getType() == rb::ValueType::Array && getArrayLength() == 2)
    {
        std::vector<rb::Value> elements = to<std::vector<rb::Value>>();
        sf::Vector2f vector(elements[0].to<float>(), elements[1].to<float>());
        return vector;
    }
    else
    {
        errorHandling(T_OBJECT);
        sf::Vector2f vector(getVar<symVarX, float>(), getVar<symVarY, float>());
        return vector;
    }
}

template<>
sf::Vector2i Value::to() const
{
	if(getType() == rb::ValueType::Array && getArrayLength() == 2)
    {
        std::vector<rb::Value> elements = to<std::vector<rb::Value>>();
        sf::Vector2i vector(elements[0].to<int>(), elements[1].to<int>());
        return vector;
    }
    else
    {
        errorHandling(T_OBJECT);
        sf::Vector2i vector(getVar<symVarX, int>(), getVar<symVarY, int>());
        return vector;
    }
}

template<>
sf::Vector2u Value::to() const
{
	if(getType() == rb::ValueType::Array && getArrayLength() == 2)
    {
        std::vector<rb::Value> elements = to<std::vector<rb::Value>>();
        sf::Vector2u vector(elements[0].to<unsigned int>(), elements[1].to<unsigned int>());
        return vector;
    }
    else
    {
        errorHandling(T_OBJECT);
        sf::Vector2u vector(getVar<symVarX, unsigned int>(), getVar<symVarY, unsigned int>());
        return vector;
    }
}

template<>
Value Value::create<sf::Vector2f>( sf::Vector2f value )
{
	rb::Value object = rbVector2::getDefinition().newObject(value.x, value.y);
	return object;
}

template<>
Value Value::create<const sf::Vector2f&>( const sf::Vector2f& value )
{
	rb::Value object = rbVector2::getDefinition().newObject(value.x, value.y);
	return object;
}

template<>
Value Value::create<sf::Vector2i>( sf::Vector2i value )
{
	rb::Value object = rbVector2::getDefinition().newObject(value.x, value.y);
	return object;
}

template<>
Value Value::create<const sf::Vector2i&>( const sf::Vector2i& value )
{
	rb::Value object = rbVector2::getDefinition().newObject(value.x, value.y);
	return object;
}

template<>
Value Value::create<sf::Vector2u>( sf::Vector2u value )
{
	rb::Value object = rbVector2::getDefinition().newObject(value.x, value.y);
	return object;
}

template<>
Value Value::create<const sf::Vector2u&>( const sf::Vector2u& value )
{
	rb::Value object = rbVector2::getDefinition().newObject(value.x, value.y);
	return object;
}

}