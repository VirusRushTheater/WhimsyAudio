#include "whimsyexception.h"
#include "whimsybytestream.h"

#define B64ENCODE(N)    b64_encodingtable[N]
#define B64DECODE(C)    b64_decodingtable[(byte)C]

using namespace whimsycore;

void memcpy_reverse(void* dest, const void* src, size_t length)
{
    byte* __dest    = (byte*) dest;
    byte* __src     = (byte*) src;
    size_t cursor, rev_cursor;
    for(cursor = 0, rev_cursor = length - 1; cursor < length; cursor++, rev_cursor--)
        *(__dest + rev_cursor) = *(__src + cursor);
}

ByteStream& ByteStream::addIntReverseEndian(int32_t number)
{
    byte output[4];
    output[0] = number >> 24;   // LSB
    output[1] = number >> 16;
    output[2] = number >> 8;
    output[3] = number;         // MSB

    this->insert(this->end(), &(output[0]), &(output[4]));

    return *this;
}

ByteStream& ByteStream::addWordReverseEndian(uint16_t number)
{
    byte output[2];
    output[0] = number >> 8;    // LSB
    output[1] = number;         // MSB

    this->insert(this->end(), &(output[0]), &(output[2]));

    return *this;
}

ByteStream& ByteStream::getIntReverseEndian(int32_t& number)
{
    if(cursor + 4 > this->size())
        throw Exception(this, Exception::ArrayOutOfBounds, "getIntReverseEndian went out of bounds.");

    byte* input = reinterpret_cast<byte*>(&number);
    input[3] = this->at(cursor++);
    input[2] = this->at(cursor++);
    input[1] = this->at(cursor++);
    input[0] = this->at(cursor++);

    return *this;
}

ByteStream& ByteStream::getWordReverseEndian(uint16_t &number)
{
    if(cursor + 2 > this->size())
        throw Exception(this, Exception::ArrayOutOfBounds, "getWordReverseEndian went out of bounds.");

    byte* input = reinterpret_cast<byte*>(&number);
    input[1] = this->at(cursor++);
    input[0] = this->at(cursor++);

    return *this;
}

ByteStream& ByteStream::addInt(int32_t number)
{
    this->insert(this->end(), (unsigned char*)&number, &(((unsigned char*)&number)[4]));
    return *this;
}

ByteStream& ByteStream::addWord(uint16_t number)
{
    this->insert(this->end(), (unsigned char*)&number, &(((unsigned char*)&number)[2]));
    return *this;
}

ByteStream& ByteStream::addMidiVarLen(uint32_t number)
{
    if(number >> 21)
        this->push_back(0x80 | ((number >> 21) & 0x7F));

    if(number >> 14)
        this->push_back(0x80 | ((number >> 14) & 0x7F));

    if(number >> 7)
        this->push_back(0x80 | ((number >> 7) & 0x7F));

    this->push_back(number & 0x7F);

    return *this;
}

ByteStream& ByteStream::getMidiVarLen(int& number)
{
    number = 0;

    byte byteskip;
    bool continueflag;

    for(byteskip = 0; byteskip < 28; byteskip += 7)
    {
        number |=       (this->at(cursor) & 0x7F);
        continueflag =  (this->at(cursor) & 0x80);

        if(cursor++ >= this->size())
            throw Exception(this, Exception::ArrayOutOfBounds, "getMidiVarLen went out of bounds.");

        if(!continueflag)
            break;
        else
            number <<= 7;
    }

    return *this;
}

ByteStream& ByteStream::getInt(int32_t &number)
{
    if(cursor + 4 > this->size())
        throw Exception(this, Exception::ArrayOutOfBounds, "getInt went out of bounds.");

    std::memcpy(&number, &(this->at(cursor)), 4);
    cursor += 4;
    return *this;
}

ByteStream& ByteStream::getWord(uint16_t &number)
{
    if(cursor + 2 > this->size())
        throw Exception(this, Exception::ArrayOutOfBounds, "getWord went out of bounds.");

    std::memcpy(&number, &(this->at(cursor)), 2);
    cursor += 2;
    return *this;
}
/*
template<typename T> ByteStream& ByteStream::addVariable(T variable)
{
    this->insert(this->end(), (unsigned char*)&variable, &(((unsigned char*)&variable)[sizeof(T)]));
    return *this;
}

template<typename T> ByteStream& ByteStream::getVariable(T& variable)
{
    if(cursor + sizeof(T) >= this->size())
        throw Exception(this, Exception::ArrayOutOfBounds, "getVariable went out of bounds.");

    std::memcpy(&variable, &(this->at(cursor)), sizeof(T));
    cursor += sizeof(T);

    return *this;
}

template<typename T> ByteStream& ByteStream::addVariableReverseEndian(T variable)
{
    byte* revert_pointer = reinterpret_cast<byte*>(&variable);
    byte swap_reserve;

    for(size_t i = 0; i < (sizeof(T) / 2); i++)
    {
        swap_reserve = revert_pointer[sizeof(T) - 1 - i];
        revert_pointer[sizeof(T) - 1 - i] = revert_pointer[i];
        revert_pointer[i] = swap_reserve;
    }

    this->insert(this->end(), (unsigned char*)&variable, &(((unsigned char*)&variable)[sizeof(T)]));

    return *this;
}

template<typename T> ByteStream& ByteStream::getVariableReverseEndian(T& variable)
{
    byte* revert_pointer = reinterpret_cast<byte*>(&variable);
    byte swap_reserve;

    if(cursor + sizeof(T) >= this->size())
        throw Exception(this, Exception::ArrayOutOfBounds, "getVariableReverseEndian went out of bounds.");

    std::memcpy(&variable, &(this->at(cursor)), sizeof(T));
    cursor += sizeof(T);

    for(size_t i = 0; i < (sizeof(T) / 2); i++)
    {
        swap_reserve = revert_pointer[sizeof(T) - 1 - i];
        revert_pointer[sizeof(T) - 1 - i] = revert_pointer[i];
        revert_pointer[i] = swap_reserve;
    }

    return *this;
}
*/

void ByteStream::seekCur(size_t pos)
{
    if((pos + cursor) >= this->size())
    {
        throw Exception(this, Exception::ArrayOutOfBounds, "Seek out of bounds.");
    }
    else
        cursor += pos;
}

void ByteStream::seekSet(size_t pos)
{
    if(pos >= this->size())
    {
        throw Exception(this, Exception::ArrayOutOfBounds, "Seek out of bounds.");
    }
    else
        cursor = pos;
}

size_t ByteStream::tell() const
{
    return cursor;
}

void ByteStream::rewind()
{
    cursor = 0;
}

size_t ByteStream::writeFile(const char *filepath)
{
    size_t retval;

    std::FILE* fhandler = std::fopen(filepath, "wb");
    if(!fhandler)
        throw Exception(this, Exception::CouldNotOpenFileForWriting, filepath);

    retval = std::fwrite(&(this->at(0)), this->size(), 1, fhandler);
    std::fclose(fhandler);

    return retval;
}

size_t ByteStream::readFile(const char *filepath)
{
    size_t retval;

    std::FILE* fhandler = std::fopen(filepath, "rb");
    if(!fhandler)
        throw Exception(this, Exception::CouldNotOpenFileForReading, filepath);

    std::fseek(fhandler, 0, SEEK_END);
    retval = std::ftell(fhandler);
    std::rewind(fhandler);

    this->clear();
    this->resize(retval);

    retval = std::fread(&(this->at(0)), this->size(), 1, fhandler);
    std::fclose(fhandler);

    return retval;
}

size_t ByteStream::base64Decode(const char *b64)
{
    // 4 characters in b64 -> 3 characters in b256 (byte)
    // Characters might be padded with a maximum trailing number of three (3) equals (=)
    // This algorithm doesn't do safety checks. Be aware.

    // 64: Equals (=), 65: Whitespace, 66: Invalid
    const byte b64_decodingtable[] = {
    //              0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
        /* 00 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 65, 65, 66, 66, 65, 66, 66,
        /* 10 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* 20 */    65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 62, 66, 66, 66, 63,
        /* 30 */    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 66, 66, 66, 64, 66, 66,
        /* 40 */    66,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        /* 50 */    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 66, 66, 66, 66, 66,
        /* 60 */    66, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        /* 70 */    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 66, 66, 66, 66, 66,
        /* 80 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* 90 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* A0 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* B0 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* C0 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* D0 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* E0 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
        /* F0 */    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66
    };

    // 4 characters in the buffer = 3 bytes.
    unsigned long int charpack = 0;
    byte* charpack_array = (byte*)&charpack;

    size_t readbytes = 0;
    byte readchars = 0;

    byte padding = 0;
    byte code;

    for(; *b64 != '\0'; b64++)
    {
        code = b64_decodingtable[(byte) *b64];

        // Skip whitespaces.
        if(code == 65)
            continue;
        // Invalid characters make this function halt.
        else if(code == 66)
            return 0;

        charpack <<= 6;
        if(code == 64)
            padding++;
        else
            charpack |= code;

        if(++readchars == 4)
        {
#if LITTLE_ENDIAN
            charpack_array[2] ^= charpack_array[0];
            charpack_array[0] ^= charpack_array[2];
            charpack_array[2] ^= charpack_array[0];
#else
            charpack <<= 8;
#endif
            pushArray(charpack_array, 3 - padding);
            readbytes += (3 - padding);

            readchars = 0;
            charpack = 0;
            if(padding != 0)
                break;
        }
    }

    outputFormat = OutputFormat_Base64;

    return readbytes;
}

std::string ByteStream::base64Encode(bool pretty) const
{
    std::string retval = "";
    const char b64_encodingtable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

    if(size() == 0)
        return std::string();

    size_t cursor = 0;

    char encoded_array[5];
    encoded_array[4] = '\0';

    unsigned int bitmask = 0, encoding_buffer = 0;

    byte readbytes, rbc = 0;

    do
    {
        readbytes = (size() - cursor >= 3) ? 3 : size() - cursor;
#if LITTLE_ENDIAN
        memcpy_reverse(&encoding_buffer, &at(cursor), readbytes);
        encoding_buffer <<= (8 * (3 - readbytes));

        bitmask = 18;
        for(byte i = 0; i < 4; i++, bitmask -= 6)
        {
            if(i > readbytes)
                encoded_array[i] = '=';
            else
                encoded_array[i] = b64_encodingtable[(encoding_buffer >> bitmask) & 63];
        }
#else
        memcpy(&encoding_buffer, &at(cursor), readbytes);
        for(byte i = 0; i < 4; i++, bitmask += 6)
        {
            if(i > readbytes)
                encoded_array[i] = '=';
            else
                encoded_array[i] = b64_encodingtable[(encoding_buffer >> bitmask) & 63];
        }
#endif

        bitmask = 0;
        encoding_buffer = 0;
        cursor += readbytes;

        retval.append(encoded_array, 4);

        if(pretty)
        {
            if((rbc += 4) == 80)
            {
                rbc = 0;
                retval.append("\r\n");
            }
        }
    }
    while(cursor < size());

    return retval;
}

size_t ByteStream::hexDecode(const char *bhex)
{
    // 4 characters in b64 -> 3 characters in b256 (byte)
    // Characters might be padded with a maximum trailing number of three (3) equals (=)
    // This algorithm doesn't do safety checks. Be aware.

    // This isn't too optimized. It pushes things bit by bit.

    byte buffer = 0;
    bool bproc = false;
    byte charvalue;
    char character;
    size_t bytesread = 0;

    const char* bhexptr = bhex;

    for(; *bhexptr != '\0'; bhexptr++)
    {
        character = *bhexptr;
        if(character == ' ' || character == '\n' || character == '\t' || character == '\r')
            continue;
        else if((character >= '0' && character <= '9'))
        {
            charvalue = character - '0';
            goto process_char;
        }
        else if((character >= 'A' && character <= 'F'))
        {
            charvalue = character - 'A' + 10;
            goto process_char;
        }
        else if((character >= 'a' && character <= 'f'))
        {
            charvalue = character - 'a' + 10;
            goto process_char;
        }
        else
            break;

        continue;
        process_char:
        if(!bproc)
        {
            buffer = 0;
            buffer |= (charvalue << 4);
            bproc = true;
        }
        else
        {
            buffer |= charvalue;
            push_back(buffer);
            buffer = 0;
            bytesread++;
            bproc = false;
        }
    }

    outputFormat = OutputFormat_Hex;

    return bytesread;
    return 0;
}

std::string ByteStream::hexEncode(bool pretty) const
{
    std::string retval;

    const char ctable[] = "0123456789ABCDEF";

    if(size() == 0)
        return std::string();

    const byte*   arraycursor = &(at(0));
    size_t  cursor = 0;
    char    encoded_byte[3];

    encoded_byte[2] = '\0';

    for(; cursor < size(); arraycursor++, cursor++)
    {
        encoded_byte[1] = ctable[(*arraycursor) & 0x0F];
        encoded_byte[0] = ctable[(*arraycursor) >> 4];

        retval.append(encoded_byte, 2);
        retval.append(" ");
        if(pretty && (cursor & 31) == 31)
            retval.append("\r\n");
    }

    return retval;
}

std::string ByteStream::toString() const
{
    switch(outputFormat)
    {
        case OutputFormat_Hex:
            return hexEncode(true);
        break;
        case OutputFormat_Base64:
            return base64Encode(true);
        break;
        default:
            return hexEncode(true);
    }
}

ByteStream::output_format_t ByteStream::getOutputFormat() const
{
    return outputFormat;
}
