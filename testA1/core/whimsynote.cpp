#include "whimsynote.h"
#include <sstream>

using namespace whimsycore;

const Note Note::null =     Note();
const Note Note::stop =     Note(WHIMSYNOTE_SPECIAL_STOP);
const Note Note::release =  Note(WHIMSYNOTE_SPECIAL_RELEASE);

Note::Note(const char *notestr)
{
    fromString(notestr);
}

Note::Note(std::string notestr)
{
    fromString(notestr.c_str());
}

void NoteProto::fromString(std::string note)
{
    fromString(note.c_str());
}

void NoteProto::fromString(const char *note)
{
    char                chrome, octave = 4;
    char*               octavepos;
    //                                    A, B,  C, D, E, F, G
    uint8_t             chrome_table[] = {9, 11, 0, 2, 4, 5, 7,};

    if(strlen(note) > 3)
    {
        notedata = WHIMSYNOTE_NULL;
        return;
    }

    octavepos = const_cast<char*>(note);
    while(!(*octavepos >= '0' && *octavepos <= '9') && *octavepos != '\0')
        octavepos = &(octavepos[1]);

    octave = strtol(octavepos, NULL, 10);

    if(note[0] >= 'A' && note[0] <= 'G')
        chrome = chrome_table[note[0] - 'A'];
    else if(note[0] >= 'a' && note[0] <= 'g')
        chrome = chrome_table[note[0] - 'a'];
    else
    {
        notedata = WHIMSYNOTE_NULL;
        return;
    }

    if(note[1] == '#')
        chrome++;
    else if(note[1] == 'b' || note[1] == 'B')
        chrome--;

    notedata =     WHIMSYNOTE_OFFSET + chrome + octave*12;
}

std::string NoteProto::toString() const
{   
    std::ostringstream  retstream;
    int chrome, octave, prenote;

    // Normal note - Chromatic scale
    if(notedata >= WHIMSYNOTE_OFFSET)
    {
        prenote =   notedata - WHIMSYNOTE_OFFSET;

        chrome =    prenote % 12;
        octave =    prenote / 12;

        switch(chrome)
        {
            case 0:     retstream << "C-"; break;
            //case 1:     (bemol) ? (retstream << "Db") : (retstream << "C#"); break;
            case 1:     retstream << "C#"; break;
            case 2:     retstream << "D-"; break;
            //case 3:     (bemol) ? (retstream << "Eb") : (retstream << "D#"); break;
            case 3:     retstream << "D#"; break;
            case 4:     retstream << "E-"; break;
            case 5:     retstream << "F-"; break;
            //case 6:     (bemol) ? (retstream << "Gb") : (retstream << "F#"); break;
            case 6:     retstream << "F#"; break;
            case 7:     retstream << "G-"; break;
            //case 8:     (bemol) ? (retstream << "Ab") : (retstream << "G#"); break;
            case 8:     retstream << "G#"; break;
            case 9:     retstream << "A-"; break;
            //case 10:    (bemol) ? (retstream << "Bb") : (retstream << "A#"); break;
            case 10:    retstream << "A#"; break;
            case 11:    retstream << "B-"; break;
            default:    retstream << "---";
        }
        retstream << octave;

        return retstream.str();
    }
    else
    {
        switch(notedata)
        {
            case WHIMSYNOTE_SPECIAL_STOP:
                return std::string("xxx");
            case WHIMSYNOTE_SPECIAL_RELEASE:
                return std::string("===");
            default:
                return std::string("---");
        }
    }
}

unsigned char NoteProto::value() const
{
    return notedata;
}

unsigned char& NoteProto::ref()
{
    return notedata;
}

int NoteProto::toInt() const
{
    return static_cast<int>(notedata);
}

bool NoteProto::isNull() const
{
    return (notedata == WHIMSYNOTE_NULL);
}

std::string Note::toString() const
{
    return NoteProto::toString();
}
