#include <iostream>
#include "whimsybase.h"

using namespace whimsycore;

std::ostream& operator <<(std::ostream& os, Base const& v)
{
    os << v.toString();
    return os;
}

std::string Utility::forceWidth(std::string original, size_t newsize, char fillchar, PaddingOptions po)
{
    int sizedifference;

    if(original.size() == newsize)
        return original;
    else if(original.size() < newsize)
    {
        sizedifference =    newsize - original.size();
        if(po == PadLeft)
            original.insert(original.end(), newsize - original.size(), fillchar);
        else // PadRight
            original.insert(original.begin(), newsize - original.size(), fillchar);
    }
    else // original.size() > newsize
    {
        sizedifference =    original.size() - newsize;
        if(po == PadLeft)
            original.erase(original.end() - sizedifference, original.end());
        else // PadRight
            original.erase(original.begin(), original.begin() + sizedifference);
    }
    return original;
}
