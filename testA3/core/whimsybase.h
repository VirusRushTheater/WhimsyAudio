#pragma once

#include <string>
#include <iostream>

#define WHIMSY_OBJECT_NAME(A)     const char* className() const {return A;}

// Defines "byte" as an unsigned char. Takes measures if it's already defined by another library.
#ifndef byte
typedef unsigned char       byte;
#endif

namespace whimsycore
{
/**
 * @brief Base class for all whimsycore classes. Defines some common debug and convenience methods, to be inherited
 * and reimplemented in children classes.
 */
class Base
{
public:
    /**
     * @brief Returns the derived class name.
     * Use the macro WHIMSY_OBJECT_NAME inside the public: section of every child class you make.
     * @return  Name of derived class.
     */
    virtual const char* className() const {return "";}

    /**
     * @brief Returns a string representation of the derived class. For debug purposes mostly.
     * @return
     */
    virtual std::string toString() const {return std::string();}

};

class Utility
{
public:
    enum PaddingOptions
    {
        PadLeft,
        PadRight
    };

    static std::string forceWidth(std::string original, size_t newsize, char fillchar = ' ', PaddingOptions po = PadLeft);
};
}

/**
 * @brief Operator overloaded with the purpose of harnessing the STL stream functions (like std::cout or std::ostringstream)
 * @param os    Output Stream
 * @param v     Reference to a WhimsyVariant class
 * @return      Identifier to that output stream
 */
extern std::ostream& operator <<(std::ostream& os, whimsycore::Base const& v);
