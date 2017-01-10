#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <map>

#include "whimsynote.h"
#include "whimsybase.h"
#include "whimsybytestream.h"

#define WHIMSYVARIANT_ENABLE_TYPE_CASTING      1

typedef uint64_t            FlagType;

namespace whimsycore
{

class Variant : public Base
{
public:
    WHIMSY_OBJECT_NAME("Core/Variant")

    enum Type
    {
        Null,
        Bool,
        Nibble,
        Byte,
        Word,
        Integer,
        Long,
        Float,
        Double,
        Note,
        String,
        VariantArray,
        HashTable,
        BinaryBlob,
        Effect,
        GenericPointer
    };

    enum OutputStringFormat
    {
        Format_Normal,
        Format_Hex
    };

    template<class T>
    struct VDPointer
    {
        mutable int                 _refcount;
        T*                          _data;

        VDPointer() : _refcount(1){}
        VDPointer(const T& ref) : _refcount(1){_data = new T(ref);}

        virtual ~VDPointer(){
            delete(_data);
        }

        void reference() const
        {
            _refcount++;
        }

        void dereference() const
        {
            //assert(_refcount > 0);
            if(--_refcount == 0)
            {
                //delete(_data);
                delete(this);
            }
        }
    };

    union VariantData
    {
        bool                                    _Bool;
        unsigned char                           _Byte;
        unsigned short int                      _Word;
        int                                     _Integer;
        float                                   _Float;
        double                                  _Double;
        long long                               _Long;
        NoteProto                               _Note;
        VDPointer<std::string>*                 _String;
        VDPointer<std::vector<Variant> >*       _VariantArray;
        VDPointer<char>*                        _Pointer;
        VDPointer<std::map<std::string, Variant> >* _HashTable;
        VDPointer<ByteStream>*                  _BinaryBlob;
    };

    static const Variant            null;

    Variant();
    Variant(const Variant& wref);
    Variant(bool _bool);
    Variant(int _int);
    Variant(long long _long);
    Variant(float _float);
    Variant(double _double);
    Variant(whimsycore::Note _note);
    Variant(const char* _cstr);
    Variant(const std::string& _cstr);
    Variant(const std::vector<Variant>& _array);
    Variant(const std::map<std::string, Variant>& _hashtable);
    Variant(const ByteStream& _binaryblob);

    Variant&                        operator=(const Variant& wref);

    const char*                     type() const;
    Variant::Type                   typeID() const;

    static const char*              typeToString(Variant::Type t);
    static Variant::Type            typeFromString(const char* s);

    bool                            isNull() const;

    Variant&                        convert(Variant::Type t);

    bool                            boolValue() const;
    unsigned char                   nibbleValue() const;
    unsigned char                   byteValue() const;
    unsigned short int              wordValue() const;
    int                             intValue() const;
    long long int                   longValue() const;
    float                           floatValue() const;
    double                          doubleValue() const;
    whimsycore::Note                noteValue() const;
    std::string                     stringValue() const;
    std::vector<Variant>            arrayValue() const;
    std::map<std::string, Variant>  hashtableValue() const;
    ByteStream                      binaryblobValue() const;

    std::string&                    stringReference();
    const std::string&                    stringReference() const;
    std::vector<Variant>&           arrayReference();
    const std::vector<Variant>&           arrayReference() const;
    std::map<std::string, Variant>& hashtableReference();
    const std::map<std::string, Variant>& hashtableReference() const;
    ByteStream&                     binaryblobReference();
    const ByteStream&                     binaryblobReference() const;

    template<typename T>            operator T(){return value<T>();}
    template<typename T> T          value() const;

    std::string                     toString() const;
    std::string                     toString(OutputStringFormat ot) const;

    std::string                     toJSON() const;
    std::string                     toJSONPretty() const;

    bool                            isLowerThan(const Variant& v) const;
    bool                            isGreaterThan(const Variant& v) const;
    bool                            isEqualThan(const Variant& v) const;

    Variant&                        at(size_t pos);
    Variant&                        at(std::string key);
    size_t                          size() const;

    bool                            operator== (const Variant& v) const;
    bool                            operator< (const  Variant& v) const;
    bool                            operator> (const  Variant& v) const;
    bool                            operator<= (const  Variant& v) const;
    bool                            operator>= (const  Variant& v) const;
    Variant&                        operator [] (size_t pos);
    Variant&                        operator [] (std::string key);

    void                            parse(const char* pstr);

    Variant&                        merge(const Variant& with);

    bool                            keyExists(const std::string& key) const;
    bool                            indexExists(const size_t key) const;

    static bool                     typeUsesExtraMemory(Type t);
    static bool                     typeIsNumeric(Type t);
    static bool                     typeIsInteger(Type t);
    static bool                     typeIsFloatingPoint(Type t);

    ~Variant();

protected:
    Type                            data_type;
    VariantData                     data_;

    std::string                     toJSON_private(std::string identation, bool whitespaces) const;

    bool                            isUsingExtraMemory() const;
    void                            noteFix();

private:
    Variant                         parse_value(char** pstrptr);
    Variant                         parse_string(char** pstrptr);
    Variant                         parse_number(char** pstrptr);
    Variant                         parse_object(char** pstrptr);
    Variant                         parse_array(char** pstrptr);
    bool                            parser_skipwhitespaces(char** pstrptr);
};
}

