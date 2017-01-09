#include "whimsyexception.h"

using namespace whimsycore;

Exception::Exception(const char* info) :
    _identifiable(false),
    _cause(NULL),
    _xctype(Unknown),
    _additional_info(info)
{
    if(strlen(_additional_info) > 0)
        _uses_additional_info = true;
    else
        _uses_additional_info = false;
}

Exception::Exception(const Base* objref, ExceptionType reasons, const char* info) :
    _identifiable(true),
    _cause(objref),
    _xctype(reasons),
    _additional_info(info)
{
    if(objref == NULL)
        _identifiable = false;

    if(strlen(_additional_info) > 0)
        _uses_additional_info = true;
    else
        _uses_additional_info = false;
}

std::string Exception::toString() const
{
    std::ostringstream retval;
    if(_identifiable)
    {
        retval << "[A Whimsy Object @ 0x" << std::hex << reinterpret_cast<long long int>(_cause) << "] ";
        retval << exceptionTypeToString(_xctype);
    }
    else
    {
        retval << "[Exception] ";
        retval << exceptionTypeToString(_xctype);
        //retval << "Cause unknown";
    }

    if(_uses_additional_info)
        retval << " - " << _additional_info;

    return retval.str();
}

const char* Exception::what() const throw()
{
    std::cerr << toString() << std::endl;
    return "An instance of whimsycore::Exception was thrown.\n";
}

const char* Exception::exceptionTypeToString(ExceptionType t)
{
    switch(t)
    {
        // Core exceptions
        case Unknown:
            return "Unknown";
        case TestPurposes:
            return "Testing exception";
        case ArrayOutOfBounds:
            return "Array out of bounds";
        case InvalidConversion:
            return "Invalid conversion";
        case NameConflict:
            return "Name conflict";
        case NotFound:
            return "Not found";
        case CouldNotOpenFileForReading:
            return "Could not open file for reading";
        case CouldNotOpenFileForWriting:
            return "Could not open file for writing";
        case NotComparable:
            return "Variant types not comparable";
        case MalformedXML:
            return "Malformed XML";
        case IncompleteSongSelector:
            return "Incomplete Song Selector";
        case ChannelDoesNotExist:
            return "Channel does not exist";
        case FieldDoesNotExist:
            return "Field does not exist";
        case ParserSyntaxError:
            return "JSON parser syntax error";

        // Engine exceptions
        case PortAudioInitError:
            return "PortAudio initialization error";
        case PortAudioNoDevices:
            return "No audio devices found";

        // Default
        default:
            return "Not specified";
    }
}

Exception::ExceptionType Exception::exceptionTypeID() const
{
    return _xctype;
}

const char* Exception::exceptionType() const
{
    return exceptionTypeToString(exceptionTypeID());
}

const char* Exception::additionalInfo() const
{
    return _additional_info;
}
