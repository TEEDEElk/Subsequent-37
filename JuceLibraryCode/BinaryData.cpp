/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== knob-template.svg ==================
static const unsigned char temp_binary_data_0[] =
"/*\r\n"
"  ==============================================================================\r\n"
"\r\n"
"    knob-template.svg\r\n"
"    Created: 11 Sep 2025 11:14:24am\r\n"
"    Author:  Tharinda Dhanush\r\n"
"\r\n"
"  ==============================================================================\r\n"
"*/\r\n"
"\r\n"
"#include \"knob-template.h\"\r\n";

const char* knobtemplate_svg = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xe76b3a95:  numBytes = 297; return knobtemplate_svg;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "knobtemplate_svg"
};

const char* originalFilenames[] =
{
    "knob-template.svg"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
