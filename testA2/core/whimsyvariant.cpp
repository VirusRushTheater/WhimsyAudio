#include "whimsyvariant.h"
#include "whimsyexception.h"

#include <sstream>
#include <iomanip>

#define WHIMSYVARIANT_CLEAR       data_type = Null; data_._Long = 0ll;

using namespace whimsycore;

const Variant Variant::null = Variant();

/**
 * @brief Empty constructor. Initializes this variant as a null pointer.
 */
Variant::Variant()
{
    data_type =            Type::Null;
    data_._Integer =         0;
}

/**
 * @brief Copy constructor. Doesn't deep copy memory items if in memory, but passes a reference to it.
 * @param wref      Initializing value.
 */
Variant::Variant(const Variant& wref) :
    data_type(wref.data_type),
    data_(wref.data_)
{
    if(wref.isUsingExtraMemory())
        data_._Pointer->reference();
}

/**
 * @brief Boolean constructor. Makes this variant take the value of a boolean.
 * @param _bool_t   Initializing value.
 */
Variant::Variant(bool _bool)
{
    data_type =            Type::Bool;
    data_._Bool =            _bool;
}

/**
 * @brief Int constructor. Makes this variant take the value of an integer.
 * @param _bool_t   Initializing value.
 */
Variant::Variant(int _int)
{
    data_type =            Type::Integer;
    data_._Integer =         _int;
}

Variant::Variant(long long _long)
{
    data_type =            Type::Long;
    data_._Long =            _long;
}

/**
 * @brief Float constructor. Makes this variant take the value of a float.
 * @param _float    Initializing value.
 */
Variant::Variant(float _float)
{
    data_type =            Type::Float;
    data_._Float =           _float;
}

/**
 * @brief Double constructor. Makes this variant take the value of a double.
 * @param _double    Initializing value.
 */
Variant::Variant(double _double)
{
    data_type =            Type::Double;
    data_._Double =          _double;
}

/**
 * @brief Note constructor. Makes this variant take the value of a note.
 * @param _note     Initializing value.
 */
Variant::Variant(whimsycore::Note _note)
{
    data_type =            Type::Note;
    data_._Note =            _note;
    noteFix();
}

/**
 * @brief String constructor. Makes this variant take the value of a string.
 * @param _cstr     Initializing value.
 */
Variant::Variant(const char *_cstr)
{
    data_type =            Type::String;
    data_._String =          new VDPointer<std::string>(std::string(_cstr));
    //_data.String =          new std::string(_cstr);
}

/**
 * @brief String constructor. Makes this variant take the value of a string.
 * @param _cstr     Initializing value.
 */
Variant::Variant(const std::string& _cstr)
{
    data_type =            Type::String;
    data_._String =          new VDPointer<std::string>(std::string(_cstr));
    //_data.String =          new std::string(_cstr);
}

/**
 * @brief Array constructor. Makes this variant take the value of a dynamic array.
 * @param _array    Initializing value.
 */
Variant::Variant(const std::vector<Variant>& _array)
{
    data_type =            Type::VariantArray;
    data_._VariantArray =    new VDPointer<std::vector<Variant> >(_array);
    //_data.VariantArray =    new std::vector<WhimsyVariant>(_array);
}

Variant::Variant(const std::map<std::string, Variant> &_hashtable)
{
    data_type =             Type::HashTable;
    data_._HashTable =      new VDPointer<std::map<std::string, Variant> >(_hashtable);
}

Variant::Variant(const ByteStream &_binaryblob)
{
    data_type =             Type::BinaryBlob;
    data_._BinaryBlob =     new VDPointer<ByteStream>(_binaryblob);
}

/**
 * @brief Assignment operator. Doesn't deep copy memory items if in memory, but passes a reference to it.
 * @param wref
 * @return
 */
Variant& Variant::operator=(const Variant& wref)
{
    if(isUsingExtraMemory())
        data_._Pointer->dereference();

    data_type = wref.data_type;
    data_ = wref.data_;

    if(wref.isUsingExtraMemory())
        data_._Pointer->reference();

    //std::cout << "Assigning: " << wref.representate() << std::endl;
    return *this;
}

/**
 * @brief Returns the type of this Variant, in a convenient string format.
 * @return
 */
const char* Variant::type() const
{
    return typeToString(data_type);
}

/**
 * @brief Returns the Type ID of this Variant.
 * @return
 */
Variant::Type Variant::typeID() const
{
    return data_type;
}

/**
 * @brief Returns the name of the given type ID as a character string.
 * @param t     Type ID
 * @return
 */
const char* Variant::typeToString(Variant::Type t)
{
    switch(t)
    {
        case Type::Null:
            return "null";
        case Type::Bool:
            return "bool";
        case Type::Nibble:
            return "nibble";
        case Type::Byte:
            return "byte";
        case Type::Word:
            return "word";
        case Type::Integer:
            return "int";
        case Type::Float:
            return "float";
        case Type::Double:
            return "double";
        case Type::Long:
            return "long";
        case Type::Note:
            return "note";
        case Type::String:
            return "string";
        case Type::VariantArray:
            return "array";
        case Type::Effect:
            return "effect";
        case Type::GenericPointer:
            return "pointer";
        case Type::HashTable:
            return "hashtable";
        case Type::BinaryBlob:
            return "binaryblob";
        default:
            return "unknown";
    }

    return "Unknown";
}

/**
 * @brief Returns the type ID given the name of a type.
 * @param s     Type name
 * @return
 */
Variant::Type Variant::typeFromString(const char* s)
{
    // If strcasecmp returns zero, the string matches.
         if(!strcasecmp(s, "null"))     return Type::Null;
    else if(!strcasecmp(s, "bool"))     return Type::Bool;
    else if(!strcasecmp(s, "nibble"))   return Type::Nibble;
    else if(!strcasecmp(s, "byte"))     return Type::Byte;
    else if(!strcasecmp(s, "word"))     return Type::Word;
    else if(!strcasecmp(s, "int"))      return Type::Integer;
    else if(!strcasecmp(s, "float"))    return Type::Float;
    else if(!strcasecmp(s, "double"))   return Type::Double;
    else if(!strcasecmp(s, "long"))     return Type::Long;
    else if(!strcasecmp(s, "note"))     return Type::Note;
    else if(!strcasecmp(s, "string"))   return Type::String;
    else if(!strcasecmp(s, "array"))    return Type::VariantArray;
    else if(!strcasecmp(s, "effect"))   return Type::Effect;
    else if(!strcasecmp(s, "pointer"))  return Type::GenericPointer;
    else if(!strcasecmp(s, "hashtable")) return Type::HashTable;
    else if(!strcasecmp(s, "binaryblob"))  return Type::BinaryBlob;
    else                                return Type::Null;
}

/**
 * @brief Tells whether this variant holds a null value or not.
 * @return
 */
bool Variant::isNull() const
{
    return (data_type == Type::Null);
}

/**
 * @brief Returns the value of the Variant, in the given format.
 */
template<typename T> T Variant::value() const
{
    throw Exception(this, Exception::InvalidConversion, "Cannot cast to this type.");
    return T();
}

namespace whimsycore
{
template<> bool Variant::value<bool>() const
{
    return boolValue();
}

template<> unsigned char Variant::value<unsigned char>() const
{
    return byteValue();
}

template<> unsigned short int Variant::value<unsigned short int>() const
{
    return wordValue();
}

template<> int Variant::value<int>() const
{
    return intValue();
}

template<> long long int Variant::value<long long int>() const
{
    return longValue();
}

template<> float Variant::value<float>() const
{
    return floatValue();
}

template<> double Variant::value<double>() const
{
    return doubleValue();
}

template<> whimsycore::Note Variant::value<whimsycore::Note>() const
{
    return noteValue();
}

template<> std::string Variant::value<std::string>() const
{
    return stringValue();
}

template<> std::vector<Variant> Variant::value<std::vector<Variant> >() const
{
    return arrayValue();
}

template<> std::map<std::string, Variant> Variant::value<std::map<std::string, Variant> >() const
{
    return hashtableValue();
}
}

/**
 * @brief Returns the bool equivalent of this variable's value.
 */
bool Variant::boolValue() const
{
    if(data_type == Bool)
        return data_._Bool;

    switch(data_type)
    {
        case Null:
            return false;
        case Bool:
        case Nibble:
        case Byte:
            return (data_._Byte != 0);
        case Word:
            return (data_._Word != 0);
        case Integer:
            return (data_._Integer != 0);
        case Long:
            return (data_._Long != 0);
        case Float:
            return (data_._Float != 0);
        case Double:
            return (data_._Double != 0);
        case Note:
            return (data_._Note.toInt() < 128);
        case String:
            return (strcasecmp(data_._String->_data->c_str(), "true") == 0 ||
                    strcmp(data_._String->_data->c_str(), "1") == 0);
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
            {
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Bool.");
                return false;
            }
            return data_._VariantArray->_data->at(0).boolValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Bool.");
            return false;
    }
}

/**
 * @brief Returns the nibble equivalent of this variable's value.
 */
unsigned char Variant::nibbleValue() const
{
    if(data_type == Nibble)
        return data_._Byte;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
            return (data_._Byte & 15);
        case Word:
            return static_cast<unsigned char>((data_._Word < 16) ? data_._Word : 15);
        case Integer:
            return static_cast<unsigned char>((data_._Integer < 16) ? ((data_._Integer >= 0) ? data_._Integer : 0) : 15);
        case Long:
            return static_cast<unsigned char>((data_._Long < 16) ? ((data_._Long >= 0) ? data_._Long : 0) : 15);
        case Float:
            return static_cast<unsigned char>((data_._Float < 16) ? ((data_._Float >= 0) ? data_._Float : 0) : 15);
        case Double:
            return static_cast<unsigned char>((data_._Double < 16) ? ((data_._Double >= 0) ? data_._Double : 0) : 15);
        case Note:
            return static_cast<unsigned char>((data_._Note.toInt() - 128) % 12);
        case String:
            return Variant((int) strtol(data_._String->_data->c_str(), NULL, 10)).nibbleValue();
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
            {
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Nibble.");
                return 0;
            }
            return data_._VariantArray->_data->at(0).nibbleValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Nibble.");
            return 0;
    }
}

/**
 * @brief Returns the byte equivalent of this variable's value.
 */
unsigned char Variant::byteValue() const
{
    if(data_type == Byte || data_type == Nibble)
        return data_._Byte;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
        case Word:
            return static_cast<unsigned char>(data_._Word);
        case Integer:
            return static_cast<unsigned char>(data_._Integer);
        case Long:
            return static_cast<unsigned char>(data_._Long);
        case Float:
            return static_cast<unsigned char>(data_._Float);
        case Double:
            return static_cast<unsigned char>(data_._Double);
        case Note:
            return data_._Note.value();
        case String:
            return static_cast<unsigned char>(strtol(data_._String->_data->c_str(), NULL, 10));
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
            {
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Byte.");
                return 0;
            }
            return data_._VariantArray->_data->at(0).byteValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Byte.");
            return 0;
    }
}

/**
 * @brief Returns the word equivalent of this variable's value.
 */
unsigned short int Variant::wordValue() const
{
    if(data_type == Word)
        return data_._Word;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
            return static_cast<unsigned short int>(data_._Byte);
        case Word:
        case Integer:
            return static_cast<unsigned short int>(data_._Integer);
        case Long:
            return static_cast<unsigned short int>(data_._Long);
        case Float:
            return static_cast<unsigned short int>(data_._Float);
        case Double:
            return static_cast<unsigned short int>(data_._Double);
        case Note:
            return data_._Note.value();
        case String:
            return static_cast<unsigned short int>(strtol(data_._String->_data->c_str(), NULL, 10));
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Word.");
            return data_._VariantArray->_data->at(0).wordValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Word.");
            return 0;
    }
}

/**
 * @brief Returns the integer (double word) equivalent of this variable's value.
 */
int Variant::intValue() const
{
    if(data_type == Integer)
        return data_._Integer;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
            return static_cast<int>(data_._Byte);
        case Word:
            return static_cast<int>(data_._Word);
        case Integer:
        case Long:
            return static_cast<int>(data_._Long);
        case Float:
            return static_cast<int>(data_._Float);
        case Double:
            return static_cast<int>(data_._Double);
        case Note:
            return data_._Note.value();
        case String:
            return static_cast<int>(strtol(data_._String->_data->c_str(), NULL, 10));
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Integer.");
            return data_._VariantArray->_data->at(0).intValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Integer.");
            return 0;
    }
}

/**
 * @brief Returns the long long (quad word) equivalent of this variable's value.
 */
long long int Variant::longValue() const
{
    if(data_type == Long)
        return data_._Long;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
            return static_cast<long long int>(data_._Byte);
        case Word:
            return static_cast<long long int>(data_._Word);
        case Integer:
            return static_cast<long long int>(data_._Integer);
        case Long:
        case Float:
            return static_cast<long long int>(data_._Float);
        case Double:
            return static_cast<long long int>(data_._Double);
        case Note:
            return data_._Note.value();
        case String:
            return strtoll(data_._String->_data->c_str(), NULL, 10);
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Long.");
            return data_._VariantArray->_data->at(0).longValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Long.");
            return 0;
    }
}

/**
 * @brief Returns the single-precision floating point number equivalent of this variable's value.
 */
float Variant::floatValue() const
{
    if(data_type == Float)
        return data_._Float;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
            return static_cast<float>(data_._Byte);
        case Word:
            return static_cast<float>(data_._Word);
        case Integer:
            return static_cast<float>(data_._Integer);
        case Long:
            return static_cast<float>(data_._Long);
        case Float:
        case Double:
            return static_cast<float>(data_._Double);
        case Note:
            return data_._Note.value();
        case String:
            return strtof(data_._String->_data->c_str(), NULL);
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Float.");
            return data_._VariantArray->_data->at(0).floatValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Float.");
            return 0;
    }
}

/**
 * @brief Returns the double-precision floating point number equivalent of this variable's value.
 */
double Variant::doubleValue() const
{
    if(data_type == Double)
        return data_._Double;

    switch(data_type)
    {
        case Null:
            return 0;
        case Bool:
            return (data_._Bool) ? 1 : 0;
        case Nibble:
        case Byte:
            return static_cast<double>(data_._Byte);
        case Word:
            return static_cast<double>(data_._Word);
        case Integer:
            return static_cast<double>(data_._Integer);
        case Long:
            return static_cast<double>(data_._Long);
        case Float:
            return static_cast<double>(data_._Float);
        case Double:
        case Note:
            return data_._Note.value();
        case String:
            return strtod(data_._String->_data->c_str(), NULL);
        case VariantArray:
        case Effect:
            if(data_._VariantArray->_data->size() < 1)
                throw Exception(this, Exception::InvalidConversion, "Variant currently an empty array. Can't convert to Double.");
            return data_._VariantArray->_data->at(0).doubleValue();
        default:
            throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Double.");
            return 0;
    }
}

/**
 * @brief Returns the Note equivalent of this variable's value.
 */
whimsycore::Note Variant::noteValue() const
{
    if(data_type == Note)
        return whimsycore::Note(data_._Note);
    else if(data_type == String)
        return whimsycore::Note(*(data_._String->_data));
    else
        return whimsycore::Note(byteValue());
}

/**
 * @brief Returns the String equivalent of this variable's value.
 */
std::string Variant::stringValue() const
{
    std::ostringstream retval;
    std::vector<Variant>::iterator it;
    std::vector<Variant>::iterator almost_end;
    std::map<std::string, Variant>::iterator itmap;

    switch(data_type)
    {
        case Type::Null:
            retval <<   "-";
        break;
        case Type::Bool:
            retval <<   (data_._Bool ? "true" : "false");
        break;
        case Type::Nibble:
        case Type::Byte:
        case Type::Word:
        case Type::Integer:
            retval <<   data_._Integer;
        break;
        case Type::Long:
            retval <<   data_._Long;
        break;
        case Type::Float:
            retval <<   data_._Float;
        break;
        case Type::Double:
            retval <<   data_._Double;
        break;
        case Type::Note:
            retval <<   data_._Note.toString();
        break;
        case Type::String:
            retval <<   *(data_._String->_data);
        break;
        case Type::VariantArray:
            almost_end = data_._VariantArray->_data->end() - 1;

            retval << "[";
            for(it = data_._VariantArray->_data->begin(); it != data_._VariantArray->_data->end(); it++)
                retval <<   it->toString() << ((it != almost_end) ? ", " : "");

            retval << "]";
        break;
        case Type::HashTable:
            retval << "{";
            for(itmap = data_._HashTable->_data->begin();;)
            {
                retval << itmap->first << " : " << itmap->second;
                itmap++;
                if(itmap != data_._HashTable->_data->end())
                    retval << ", ";
                else
                    break;
            }
            retval << "}";
        break;
        case Type::BinaryBlob:
            // ***
            // Convert to base64 before outputting.
        break;
        case Effect:
            // ---------------------------------------> Not yet implemented!
        break;
        default:
            retval << "unknown";
    }

    return retval.str();
}

/**
 * @brief Returns the Variant std::vector equivalent of this variable's value.
 */
std::vector<Variant> Variant::arrayValue() const
{
    std::vector<Variant> retval;
    if(data_type == VariantArray || data_type == Effect)
        retval = std::vector<Variant>(*(data_._VariantArray->_data));
    else
        retval.push_back(*this);

    return retval;
}

std::map<std::string, Variant> Variant::hashtableValue() const
{
    std::map<std::string, Variant> retval;
    if(data_type == HashTable)
        retval = std::map<std::string, Variant>(*(data_._HashTable->_data));

    return retval;
}

ByteStream Variant::binaryblobValue() const
{
    ByteStream retval;
    if(data_type == BinaryBlob)
        return ByteStream(*(data_._BinaryBlob->_data));
    else
    {
        switch(data_type)
        {
            case Bool:
                retval.push_back((byte)data_._Bool);
            case Nibble:
            case Byte:
                retval.push_back(data_._Byte);
            case Word:
                retval.addVariable<unsigned short>(data_._Word);
            case Integer:
                retval.addVariable<int>(data_._Integer);
            case Long:
                retval.addVariable<long long int>(data_._Long);
            case Float:
                retval.addVariable<float>(data_._Float);
            case Double:
                retval.addVariable<double>(data_._Double);
            case Note:
                retval.push_back(data_._Note.value());
            case String:
                retval.addItems(data_._String->_data->c_str());
            case VariantArray:
            case Effect:
                for(std::vector<Variant>::const_iterator cit = data_._VariantArray->_data->begin();
                    cit != data_._VariantArray->_data->end(); cit++)
                retval.addItems(cit->binaryblobValue());
            default:
                throw Exception(this, Exception::InvalidConversion, "Unknown conversion to Binary Blob.");
                return 0;
        }
    }

    return retval;
}

std::string& Variant::stringReference()
{
    return *(data_._String->_data);
}

const std::string& Variant::stringReference() const
{
    return *(data_._String->_data);
}

std::vector<Variant>& Variant::arrayReference()
{
    return *(data_._VariantArray->_data);
}

const std::vector<Variant>& Variant::arrayReference() const
{
    return *(data_._VariantArray->_data);
}

std::map<std::string, Variant>& Variant::hashtableReference()
{
    return *(data_._HashTable->_data);
}

const std::map<std::string, Variant>& Variant::hashtableReference() const
{
    return *(data_._HashTable->_data);
}

ByteStream& Variant::binaryblobReference()
{
    return *(data_._BinaryBlob->_data);
}

const ByteStream& Variant::binaryblobReference() const
{
    return *(data_._BinaryBlob->_data);
}

/**
 * @brief Failsafe class if it doesn't know how to convert to this type.
 */
/*
template<> T Variant::value() const
{
    throw Exception(this, Exception::InvalidConversion, "whimsycore::Variant doesn't know how to convert to this class.");
    return T();
}
*/

/**
 * @brief Tells whether this Variant is using extra memory (i.e. a String or an Array)
 */
bool Variant::isUsingExtraMemory() const
{
    return typeUsesExtraMemory(data_type);
}

/**
 * @brief String representation of this Variant.
 */
std::string Variant::toString() const
{
    return stringValue();
}

/**
 * @brief String representation of this Variant.
 * @param ot    Number representation. Use Variant::Format_Normal to return the decimal representation of this number. Variant::Format_Hex for a hexadecimal representation.
 * @return
 */
std::string Variant::toString(OutputStringFormat ot) const
{
    std::ostringstream retval;
    std::vector<Variant>::iterator it;
    std::vector<Variant>::iterator almost_end;
    std::map<std::string, Variant>::iterator itmap;

    if(ot == Format_Normal)
        return toString();
    else if(ot == Format_Hex)
    {
        switch(data_type)
        {
            // Null, Note and String -> No changes.
            case Type::Null:
            case Type::Note:
            case Type::String:
                return toString();

            // Floating types have now 4 precision points.
            case Type::Float:
                retval.precision(4);
                retval << data_._Float;
            break;
            case Type::Double:
                retval.precision(4);
                retval << data_._Double;
            break;

            // Integer types are converted to hex and padded as necessary.
            case Type::Bool:
                retval <<   (data_._Bool ? "1" : "0");
            break;
            case Type::Nibble:
                retval << std::hex << (int)(data_._Byte & 15);
            break;
            case Type::Byte:
                retval << std::setfill('0') << std::setw(2) << std::hex << (int)data_._Byte;
            break;
            case Type::Word:
                retval << std::setfill('0') << std::setw(4) << std::hex << data_._Word;
            break;
            case Type::Integer:
                retval << std::setfill('0') << std::setw(8) << std::hex << data_._Integer;
            break;
            case Type::Long:
                retval << std::setfill('0') << std::setw(16) << std::hex << data_._Long;
            break;

            // Array recurses this function into each element.
            case Type::VariantArray:
                almost_end = data_._VariantArray->_data->end() - 1;

                retval << "[";
                for(it = data_._VariantArray->_data->begin(); it != data_._VariantArray->_data->end(); it++)
                    retval <<   it->toString(ot) << ((it != almost_end) ? ", " : "");

                retval << "]";
            break;
            case Type::HashTable:
                retval << "{";
                for(itmap = data_._HashTable->_data->begin();;)
                {
                    retval << itmap->first << " : " << itmap->second;
                    itmap++;
                    if(itmap != data_._HashTable->_data->end())
                        retval << ", ";
                    else
                        break;
                }
                retval << "}";
            break;
            case Type::BinaryBlob:
                // ***
                // Convert to base64 before outputting.
            break;
            case Type::Effect:
                // ---------------------------------------> Not yet implemented!
            break;
            default:
                retval << "unknown";
        }
    }
    return retval.str();
}

bool Variant::typeUsesExtraMemory(Variant::Type t)
{
    if(t == Type::String ||
            t == Type::VariantArray ||
            t == Type::Effect ||
            t == Type::HashTable ||
            t == Type::GenericPointer ||
            t == Type::BinaryBlob)
        return true;
    else
        return false;
}

bool Variant::typeIsNumeric(Variant::Type t)
{
    return (typeIsInteger(t) || typeIsFloatingPoint(t));
}

bool Variant::typeIsInteger(Variant::Type t)
{
    if(t == Type::Bool ||
            t == Type::Nibble ||
            t == Type::Byte ||
            t == Type::Word ||
            t == Type::Integer ||
            t == Type::Long)
        return true;
    else
        return false;
}

bool Variant::typeIsFloatingPoint(Variant::Type t)
{
    if(t == Type::Float ||
            t == Type::Double)
        return true;
    else
        return false;
}

/**
 * @brief Destructor. Frees memory if this is a pointer.
 */
Variant::~Variant()
{
    if(isUsingExtraMemory())
        data_._Pointer->dereference();
}

/**
 * @brief Converts this Variant into a given type.
 * @param t
 * @return
 */
Variant& Variant::convert(Variant::Type t)
{
    Variant             rval;

    // Dismiss if no conversion.
    if(t == data_type || data_type == Null)
        return *this;

    // Any type -> Null type = Nullify (and remove extra memory if necessary)
    if(t == Null)
    {
        data_type = Null;
    }

    // Any type -> Generic Pointer = What.
    if(t == GenericPointer)
    {
        throw Exception(this, Exception::InvalidConversion, "GenericPointer isn't meant to be a valid type.");
    }

    switch(t)
    {
        case Null:      break;
        case Bool:      rval = boolValue();     break;
        case Nibble:    rval = nibbleValue();   break;
        case Byte:      rval = byteValue();     break;
        case Word:      rval = wordValue();     break;
        case Integer:   rval = intValue();      break;
        case Long:      rval = longValue();     break;
        case Float:     rval = floatValue();    break;
        case Double:    rval = doubleValue();   break;
        case Note:      rval = noteValue();     break;
        case String:    rval = stringValue();   break;
        case HashTable: rval = hashtableValue(); break;
    case BinaryBlob:    rval = binaryblobValue(); break;
        case VariantArray:
        case Effect:    rval = arrayValue();    break;
    default: break;
    }

    /*
    if(isUsingExtraMemory())
        _data._Pointer->dereference();
        */
    *this =         rval;
    if(rval.data_type != Null)
        data_type =    t;

    return *this;
}

bool Variant::isLowerThan(const Variant &v) const
{
    if(typeIsNumeric(typeID()) && typeIsNumeric(v.typeID()))
        return (this->doubleValue() < v.doubleValue());

    if(typeID() == String && v.typeID() == String)
        return std::strcmp(data_._String->_data->c_str(), v.data_._String->_data->c_str()) < 0;

    else
    {
        throw Exception(this, Exception::NotComparable, "Comparation invalid (isLowerThan)");
        return false;
    }
}

bool Variant::isGreaterThan(const Variant &v) const
{
    if(typeIsNumeric(typeID()) && typeIsNumeric(v.typeID()))
        return (this->doubleValue() > v.doubleValue());

    if(typeID() == String && v.typeID() == String)
        return std::strcmp(data_._String->_data->c_str(), v.data_._String->_data->c_str()) > 0;

    else
    {
        throw Exception(this, Exception::NotComparable, "Comparation invalid (isGreaterThan)");
        return false;
    }
}

bool Variant::isEqualThan(const Variant &v) const
{
    if(typeID() == Null && v.typeID() == Null)
        return true;

    if(typeIsNumeric(typeID()) && typeIsNumeric(v.typeID()))
        return (this->doubleValue() == v.doubleValue());

    if(typeID() == String && v.typeID() == String)
        return std::strcmp(data_._String->_data->c_str(), v.data_._String->_data->c_str()) == 0;

    if(typeID() == VariantArray && v.typeID() == VariantArray)
    {
        std::vector<Variant>::const_iterator    thisit, vit;

        if(data_._VariantArray->_data->size() != v.data_._VariantArray->_data->size())
            return false;

        thisit = this->data_._VariantArray->_data->begin();
        vit = v.data_._VariantArray->_data->begin();

        for(; thisit != this->data_._VariantArray->_data->end(); thisit++, vit++)
        {
            if(!(*thisit).isEqualThan(*vit))
                return false;
        }
        return true;
    }

    else
    {
        //throw Exception(this, Exception::NotComparable, "Comparation invalid (isEqualThan)");
        return false;
    }
}

Variant& Variant::at(size_t pos)
{
    if(typeID() == VariantArray)
        return data_._VariantArray->_data->at(pos);
    else
        return *this;
}

Variant& Variant::at(std::string key)
{
    if(typeID() == HashTable)
        return data_._HashTable->_data->at(key);
    else
        return *this;
}

void Variant::noteFix()
{
    if(data_type == Note)
    {
        if(data_._Note.isNull())
        {
            WHIMSYVARIANT_CLEAR
        }
    }
}

bool Variant::operator == (const Variant& v) const
{
    return isEqualThan(v);
}

bool Variant::operator < (const  Variant& v) const
{
    return isLowerThan(v);
}

bool Variant::operator > (const  Variant& v) const
{
    return isGreaterThan(v);
}

bool Variant::operator <= (const  Variant& v) const
{
    return !isGreaterThan(v);
}

bool Variant::operator >= (const  Variant& v) const
{
    return !isLowerThan(v);
}

Variant& Variant::operator [] (size_t pos)
{
    if(data_type == VariantArray)
    {
        if(pos >= size())
            data_._VariantArray->_data->insert(data_._VariantArray->_data->end(), (pos + 1) - size(), Variant::null);

        return (*data_._VariantArray->_data)[pos];
    }
    else
    {
        *this = Variant(std::vector<Variant>());
        if(pos >= size())
            data_._VariantArray->_data->insert(data_._VariantArray->_data->end(), (pos + 1) - size(), Variant::null);
        return (*data_._VariantArray->_data)[pos];
    }

}

Variant& Variant::operator [] (std::string key)
{
    if(typeID() != HashTable)
        *this = Variant(std::map<std::string, Variant>());

    return (*data_._HashTable->_data)[key];
}

size_t Variant::size() const
{
    if(typeID() == VariantArray)
        return data_._VariantArray->_data->size();
    if(typeID() == HashTable)
        return data_._HashTable->_data->size();
    if(typeID() == String)
        return data_._String->_data->size();
    if(typeID() == BinaryBlob)
        return data_._BinaryBlob->_data->size();
    if(typeID() == Null)
        return 0;
    else
        return 1;
}

std::string Variant::toJSON() const
{
    return toJSON_private(std::string(""), false);
}

std::string Variant::toJSONPretty() const
{
    return toJSON_private(std::string(""), true);
}

std::string Variant::toJSON_private(std::string identation, bool blankspaces) const
{
    const std::string identation_sep = (blankspaces) ? "    " : std::string("");

    std::ostringstream rval;
    if(data_type == Null)
        rval << "null";
    else if(data_type == String)
        rval << "\"" << *(data_._String->_data) << "\"";
    else if(data_type == Variant::Note)
        rval << "\"@" << data_._Note.toString() << "\"";
    else if(data_type == BinaryBlob)
    {
        if(data_._BinaryBlob->_data->getOutputFormat() == ByteStream::OutputFormat_Hex)
            rval << "\"#" <<  data_._BinaryBlob->_data->hexEncode(blankspaces) << "\"";
        else
            rval << "\"=" <<  data_._BinaryBlob->_data->base64Encode(blankspaces) << "\"";
    }
    else if(data_type == Bool)
        rval << ((data_._Bool) ? "true" : "false");
    else if(data_type == Nibble || data_type == Byte || data_type == Word || data_type == Integer ||
            data_type == Long || data_type == Float || data_type == Double)
        rval << toString();
    else if(data_type == VariantArray)
    {
        if(blankspaces)
            rval << std::endl << identation << "[" << std::endl;
        else
            rval << identation << "[";

        for(std::vector<Variant>::const_iterator vit = data_._VariantArray->_data->begin();
            vit != data_._VariantArray->_data->end();
            vit++)
        {
            rval << identation << vit->toJSON_private(identation_sep + identation, blankspaces);
            if(vit != (data_._VariantArray->_data->end() - 1))
            {
                rval << ",";
                if(blankspaces)
                    rval << std::endl;
            }
        }

        if(blankspaces)
            rval << std::endl << identation << "]";
        else
            rval << identation << "]";
    }
    else if(data_type == HashTable)
    {
        if(blankspaces)
            rval << std::endl << identation << "{" << std::endl;
        else
            rval << identation << "{";

        for(std::map<std::string, Variant>::const_iterator mit = data_._HashTable->_data->begin();;)
        {
            if(blankspaces)
                rval << identation << "\"" << mit->first << "\": " << mit->second.toJSON_private(identation_sep + identation, blankspaces);
            else
                rval << identation << "\"" << mit->first << "\":" << mit->second.toJSON_private(identation_sep + identation, blankspaces);

            mit++;
            if(mit != data_._HashTable->_data->end())
            {
                rval << ",";
                if(blankspaces)
                    rval << std::endl;
            }
            else
            {
                break;
            }
        }

        if(blankspaces)
            rval << std::endl << identation << "}";
        else
            rval << identation << "}";

    }

    return rval.str();
}

void Variant::parse(const char *pstr)
{
    char* bufferstr = strdup(pstr);
    char* freevar = bufferstr;
    *this = parse_value(&bufferstr);
    free(freevar);
}

Variant Variant::parse_value(char **pstrptr)
{
    char character;

    // Waiting for a Value.
    for(; **pstrptr != '\0'; (*pstrptr)++)
    {
        character = **pstrptr;

        // Identify the next object.
        // Omit whitespaces.
        if(character == ' ' || character == '\n' || character == '\r' || character == '\t')
            continue;

        // It's a string if a " is found.
        else if(character == '\"')
        {
            return parse_string(pstrptr);
        }

        else if(character == '-' || (character >= '0' && character <= '9'))
        {
            return parse_number(pstrptr);
        }

        else if(character == '{')
        {
            (*pstrptr)++;
            return parse_object(pstrptr);
        }

        else if(character == '[')
        {
            (*pstrptr)++;
            return parse_array(pstrptr);
        }

        else if(character == 't')
        {
            if(strncmp(*pstrptr, "true", 4) == 0)
            {
                (*pstrptr) += 4;
                return Variant(true);
            }
            else
                break;
        }

        else if(character == 'f')
        {
            if(strncmp(*pstrptr, "false", 5) == 0)
            {
                (*pstrptr) += 5;
                return Variant(false);
            }
            else
                break;
        }

        else if(character == 'n')
        {
            if(strncmp(*pstrptr, "null", 4) == 0)
            {
                (*pstrptr) += 4;
                return Variant::null;
            }
            else
                break;
        }

        else
            break;
    }

    throw Exception(NULL, Exception::ParserSyntaxError, "Unexpected expression.");
    return Variant::null;
}

Variant Variant::parse_string(char **pstrptr)
{
    char    character;
    char*   string_stack = ++(*pstrptr);
    Variant retval;

    bool    ignorewhitespaces = (**pstrptr == '#');
    if(ignorewhitespaces)
        ++(*pstrptr);

    // Waiting for a Value.
    for(; **pstrptr != '\0'; (*pstrptr)++)
    {
        character = **pstrptr;

        // It's a string if a " is found. Close it.
        // If prefixed by '@', it's a note.
        // If prefixed by '#', it's a base64-encoded binary. Whitespaces within are omitted.
        if(character == '\"')
        {
            **pstrptr = '\0';
            (*pstrptr)++;
            if(string_stack[0] == '@')
                return Variant(whimsycore::Note(&(string_stack[1])));
            else if(string_stack[0] == '=')
            {
                retval = ByteStream();
                retval.binaryblobReference().base64Decode(&(string_stack[1]));
                return retval;
            }
            else if(string_stack[0] == '#')
            {
                retval = ByteStream();
                retval.binaryblobReference().hexDecode(&(string_stack[1]));
                return retval;
            }
            else
                return Variant(std::string(string_stack));
        }
        else if(character == '\\')
        {
            (*pstrptr)++;
        }
    }

    // If exits this "for", string parsing has failed. Report.
    throw Exception(NULL, Exception::ParserSyntaxError, "Non closed string.");
    return Variant::null;
}

Variant Variant::parse_number(char **pstrptr)
{
    // Followed the diagram at http://www.json.org/number.gif
    char    character;
    char*   string_stack = *pstrptr;
    Variant retval;

    byte    state = 0;

    // Waiting for a Value.
    for(; **pstrptr != '\0'; (*pstrptr)++)
    {
        character = **pstrptr;

        // Start or pre-digit.
        if(state == 0)
        {
            if(character == '-')
                state = 1;
            else if(character == '0')
                state = 2;
            else if(character >= '1' && character <= '9')
                state = 3;
            else if(character == '.')
                state = 4;
            else
                break;
        }
        else if(state == 1)
        {
            if(character == '0')
                state = 2;
            else if(character >= '1' && character <= '9')
                state = 3;
            else
                break;
        }
        else if(state == 2)
        {
            if(character == '.')
                state = 4;
            else if(character == 'e' || character == 'E')
                state = 6;
            else
                break;
        }
        else if(state == 3)
        {
            if(character >= '0' && character <= '9')
                continue;
            else if(character == '.')
                state = 4;
            else if(character == 'e' || character == 'E')
                state = 6;
            else
                break;
        }
        else if(state == 4)
        {
            if(character >= '0' && character <= '9')
                state = 5;
            else
                break;
        }
        else if(state == 5)
        {
            if(character >= '0' && character <= '9')
                continue;
            else if(character == 'e' || character == 'E')
                state = 6;
            else
                break;
        }
        else if(state == 6)
        {
            if(character >= '0' && character <= '9')
                state = 8;
            else if(character == '+' || character == '-')
                state = 7;
            else
                break;
        }
        else if(state == 7)
        {
            if(character >= '0' && character <= '9')
                state = 8;
            else
                break;
        }
        else if(state == 7)
        {
            if(character >= '0' && character <= '9')
                continue;
            else
                break;
        }
    }

    // Integer
    if(state == 2 || state == 3)
    {
        character = **pstrptr;
        **pstrptr = '\0';
        retval = Variant(strtoll(string_stack, NULL, 10));
        **pstrptr = character;

        return retval;
    }
    // Decimal
    if(state == 5 || state == 8)
    {
        character = **pstrptr;
        **pstrptr = '\0';
        retval = Variant(strtod(string_stack, NULL));
        **pstrptr = character;

        return retval;
    }
    else
    {
        throw Exception(NULL, Exception::ParserSyntaxError, "Malformed number.");
        return Variant::null;
    }

}

Variant Variant::parse_array(char **pstrptr)
{
    char    character;
    bool    stacked_element = false;
    Variant stack = Variant::null;

    Variant retval = std::vector<Variant>();

    for(; **pstrptr != '\0'; (*pstrptr)++)
    {
        character = **pstrptr;

        // Omit whitespaces.
        if(character == ' ' || character == '\n' || character == '\r' || character == '\t')
            continue;

        else if(character == ',')
        {
            if(!stacked_element)
            {
                throw Exception(NULL, Exception::ParserSyntaxError, "A comma was not expected.");
                return Variant::null;
            }

            stacked_element = false;
            retval.arrayReference().push_back(Variant(stack));
            stack = Variant::null;
        }

        else if(character == ']')
        {
            if(stacked_element)
            {
                retval.arrayReference().push_back(Variant(stack));
            }
            (*pstrptr)++;
            return retval;
        }

        else
        {
            // If stacked_element is true, a comma is missing. Break and throw exception.
            if(stacked_element)
            {
                throw Exception(NULL, Exception::ParserSyntaxError, "A comma or array closing ] was expected.");
                return Variant::null;
            }

            stacked_element = true;
            stack = parse_value(pstrptr);
            (*pstrptr)--;
        }
    }

    // If it reaches this point, the array never was closed.
    throw Exception(NULL, Exception::ParserSyntaxError, "The array was never closed.");
    return Variant::null;
}

Variant Variant::parse_object(char **pstrptr)
{
    char    character;

    bool    stacked_element = false;
    Variant key;
    Variant stack = Variant::null;

    Variant retval = std::map<std::string, Variant>();

    for(; **pstrptr != '\0'; (*pstrptr)++)
    {
        character = **pstrptr;

        // Omit whitespaces.
        if(character == ' ' || character == '\n' || character == '\r' || character == '\t')
            continue;

        else if(character == '}')
        {
            if(stacked_element)
            {
                retval.hashtableReference()[key.toString()] = Variant(stack);
            }
            (*pstrptr)++;
            return retval;
        }

        else if(character == ',')
        {
            if(!stacked_element)
            {
                throw Exception(NULL, Exception::ParserSyntaxError, "A comma was not expected.");
                return Variant::null;
            }

            stacked_element = false;
            retval.hashtableReference()[key.toString()] = Variant(stack);
            stack = Variant::null;
        }

        else if(character == '\"')
        {
            if(stacked_element)
            {
                throw Exception(NULL, Exception::ParserSyntaxError, "A comma or object closing } was expected.");
                return Variant::null;
            }
            key = parse_string(pstrptr);

            // Found an EOF.
            if(!parser_skipwhitespaces(pstrptr))
            {
                break;
            }

            if((**pstrptr) == ':')
            {
                (*pstrptr)++;
                stack = parse_value(pstrptr);
                (*pstrptr)--;
                stacked_element = true;
            }
        }

        else
        {
            throw Exception(NULL, Exception::ParserSyntaxError, "Expecting Key ID.");
            return Variant::null;
        }
    }

    // If it reaches this point, the array never was closed.
    throw Exception(NULL, Exception::ParserSyntaxError, "The object was never closed.");
    return Variant::null;
}

bool Variant::parser_skipwhitespaces(char **pstrptr)
{
    char character;
    for(; **pstrptr != '\0'; (*pstrptr)++)
    {
        character = **pstrptr;
        if(character == ' ' || character == '\n' || character == '\r' || character == '\t')
            continue;
        else
            return true;
    }
    return false;
}

bool Variant::keyExists(const std::string &key) const
{
    if(data_type == HashTable)
        return data_._HashTable->_data->find(key) != data_._HashTable->_data->end();
    else
        return false;
}

bool Variant::indexExists(const size_t key) const
{
    return (data_type == VariantArray && key < size());
}

Variant& Variant::merge(const Variant &with)
{
    if(data_type == HashTable)
    {
        if(with.data_type == HashTable)
        {
            for(std::map<std::string, Variant>::const_iterator wit = with.hashtableReference().begin();
                wit != with.hashtableReference().end();
                wit++)
            {
                (*this)[wit->first].merge(wit->second);
            }
        }
        else
        {
            *this = with;
        }
    }
    else if(data_type == VariantArray)
    {
        if(with.data_type == VariantArray)
            this->arrayReference().insert(this->arrayReference().end(), with.arrayReference().begin(), with.arrayReference().end());
        else
            this->arrayReference().push_back(with);
    }
    else
    {
        *this = with;
    }

    return *this;
}
