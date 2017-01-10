#pragma once

#include "whimsybase.h"

#include <sstream>
#include <string>
#include <exception>
#include <cstring>
#include <cstdlib>

namespace whimsycore
{

/**
 * @brief A class to throw identifiable and readable exceptions.
 *
 * ## How to throw a whimsycore::Exception ##
 * To throw this custom exception, you need to use one of the two constructors this class is supplied with.
 * - With just a character string. It's not recommended, as it is much more difficult to catch.
 * - With the more complete constructor, indicating the object that threw the exception (granted that it derived from whimsycore::Base), the exception type, and a character string indicating more additional info.
 *
 * ## How to extend whimsycore::Exception ##
 * To extend this class to include new exception types you need to do the following:
 * 1. Edit the `ExceptionType` enum at core/whimsyexception.h to include your custom exception type.
 * 2. Edit the switch structure in `exceptionTypeToString` function at core/whimsyexception.cpp to convert it to a readable string.
 */
class Exception : public std::exception, public Base
{   
public:
    /**
     * @brief Exception type IDs, to identify and catch different types of exceptions.
     */
    enum ExceptionType
    {
        // Core exceptions
        Unknown,
        TestPurposes,
        ArrayOutOfBounds,
        InvalidConversion,
        NameConflict,
        NotImplemented,
        CouldNotOpenFileForWriting,
        CouldNotOpenFileForReading,
        NotComparable,
        MalformedXML,
        IncompleteSongSelector,
        ChannelDoesNotExist,
        FieldDoesNotExist,
        ParserSyntaxError,
        NotFound,

        // Engine exceptions
        PortAudioInitError,
        PortAudioNoDevices
    };

private:
    bool            _identifiable;
    const Base*     _cause;
    ExceptionType   _xctype;

    bool            _uses_additional_info;
    const char*     _additional_info;

public:
    WHIMSY_OBJECT_NAME("Core/Exception")

    /**
     * @brief Basic exception constructor. Throws no more info than a string of text. Use it for unknown exceptions.
     * @param info      Details of the causes of your exception.
     */
    Exception(const char* info = "");

    /**
     * @brief Recommended exception constructor. Supplies adequate information for catching and treating exceptions.
     * @param objref    Pointer to the object that generated the exception. Any Whimsy class derived from whimsycore::Base.
     * @param reasons   Identifier for the type of exception generated. Look Exception::ExceptionType.
     * @param info      Additional information about this exception.
     */
    Exception(const Base* objref, ExceptionType reasons = ExceptionType::Unknown, const char* info = "");

    /**
     * @brief Destructor.
    */
    ~Exception(){}

    /**
     * @brief Throws a brief report about the exception, including pointer address, type of exception and additional info.
     * @return
     */
    std::string toString() const;

    /**
     * @brief Throws the type of this exception as a const char* string. Useful for reports and verbose output.
     * @return
     */
    const char* exceptionType() const;

    /**
     * @brief Throws the type of this exception as an ID number. Useful for catching.
     * @return
     */
    ExceptionType exceptionTypeID() const;

    /**
     * @brief Returns any additional info supplied at the moment this exception was thrown.
     * @return
     */
    const char* additionalInfo() const;

    /**
     * @brief This outputs a report about the exception, should the program not catch it.
     * @return
     */
    virtual const char* what() const throw();

    /**
     * @brief Converts any Exception Type ID to a const char* String.
     * @param t     Exception ID
     * @return      const char* string about the exception type supplied.
     */
    static const char* exceptionTypeToString(ExceptionType t);
};
}
