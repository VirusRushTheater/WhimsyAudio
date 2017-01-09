#pragma once

#include "whimsybase.h"

#include <cstring>
#include <string>

#define     WHIMSYNOTE_NULL             255
#define     WHIMSYNOTE_OFFSET           0

#define     WHIMSYNOTE_SPECIAL_STOP     254
#define     WHIMSYNOTE_SPECIAL_RELEASE  253

#define     WHIMSYNOTE_MACRO(CHR,OCT)   (CHR + OCT*12 + WHIMSYNOTE_OFFSET)

namespace whimsycore
{

/**
 * @brief NoteProto is provided for compatibility reasons only. *Please use Note instead.*
 */
class NoteProto
{
    friend class Note;

protected:
    unsigned char notedata;

public:
    /**
     * @brief Interprets a 3 character string as a musical note.
     * The recommended way to input a note string is using the following format (as a regular expression): `[A-Ga-g][#bB-]?[0-9]?`
     *
     * ## Which formats are accepted ##
     * - *Single letter*: Defines only the position on the chromatic scale. It's assumed that the note will in the fourth octave. For example, if you supply `A`, the note will be interpreted as an `A-4`.
     * - *Sustained and bemols*: Same as above. `A#` or `Gb` are acceptable inputs. No support for double sustained or bemols though (which can appear in music written in some strange scales).
     * - *Octaves*: Write a number at the right of your note letter to define its octave. `A-2`, `G4`, `F#5`, `Bb0` are all accepted parameters.
     * - *Special commands*: *Stop*, *Release* or *Null* are defined with `xxx`, `===` and `---` respectively.
     * - *None of the above*: Defines this as a null note.
     *
     * @param note  Character string defining the note string.
     */
    void fromString(const char* note);

    /**
     * @brief Overloaded method.
     * @param note
     */
    void fromString(std::string note);

    /**
     * @brief Returns the amounts of semitones away from `C-0`, starting from 128. Or a lower number than 128 in the case of being a special command note.
     * - 0 if it is a Null note.
     * - 1 if it is a Stop command.
     * - 2 if it is a Release command.
     *
     * @return
     */
    unsigned char   value() const;

    /**
     * @brief Returns an editable reference for this note's info. See value() for more info.
     * @return
     */
    unsigned char&  ref();

    /**
     * @brief Returns the note info as a string formatted in the way `[A-Ga-g][#bB-]?[0-9]?`. See fromString() for more info.
     * @return
     */
    std::string     toString() const;

    /**
     * @brief Similar to value(), but returning an int instead.
     * @return
     */
    int             toInt() const;

    /**
     * @brief Tells whether this note is null or not.
     * @return
     */
    bool            isNull() const;
};

/**
 * @brief This class stores a musical note. This note carries information about its position in the chromatic scale and its octave. It can range notes from octave 0 to 9, totalling 120 notes,
 * plus special NULL, STOP and RELEASE note commands.
 *
 * The recommended way to input notes is using the macro WHIMSYNOTE_MACRO, which doesn't use as much CPU processing as the fromString() method.
 * As any method inherited from whimsycore::Base, it's std::iostream friendly. Use it freely on methods like std::cout.
 */
class Note : public NoteProto, public Base
{
public:
    WHIMSY_OBJECT_NAME("Core/Note")

    static const Note   null, stop, release;

    /**
     * @brief NULL note constructor.
     */
    Note(){notedata = WHIMSYNOTE_NULL;}

    /**
     * @brief NoteProto constructor. Intended only for internal use.
     * @param c
     */
    Note(const NoteProto& c){notedata = c.notedata;}

    /**
     * @brief Int constructor for use with WHIMSYNOTE_MACRO numbers.
     * @param note  Number of semitones above `C-0` of this note, plus 128.
     */
    Note(int note){notedata = note & 255;}

    /**
     * @brief Const char* constructor. The string is parsed as a musical note. See NoteProto::fromString() for more info.
     * @param notestr   String to be parsed as a musical note.
     */
    Note(const char* notestr);

    /**
     * @brief Overloaded from the const char* constructr.
     * @param notestr   String to be parsed as a musical note.
     */
    Note(std::string notestr);

    /**
     * @brief Converts this note info in a readable string.
     * @return
     */
    std::string     toString() const;
};
}
