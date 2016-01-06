#ifndef TOOLZ_H_INCLUDED
#define TOOLZ_H_INCLUDED

#include <string>
#include <stdint.h>

//link against comdlg32

class Fun
{
public:
    static std::string GetOpenSaveFileWindows(int mode, const char filterLine[]="All Files\0*.*\0\0"); //mode: 1-open, 2-save
    static bool isDivided(int64_t num, int64_t by_what);
    static std::string getFilenameFromPath(std::string path);
    //Old ones.
    static std::string reverseString(std::string str);
    static std::string getFileExtension(std::string filename);

    template<typename T>
    T getTypeFromBytes(uint8_t bytes[], int byteLenght, T type, bool bigEndian=true);
};

template<typename T>
T Fun::getTypeFromBytes(uint8_t bytes[], int byteLenght, T type, bool bigEndian)
{
    int typeSize = sizeof(T);
    T retval = (T)0;

    uint8_t completeBytes[ typeSize ];

    if(bigEndian)
    {
        for(int i=0; i<typeSize; i++)
        {
            if(i < byteLenght) completeBytes[i] = bytes[i];
            else completeBytes[i] = 0;
        }
    }
    else
    {
        for(int i=typeSize-1; i>=0; i--)
        {
            if(i >= typeSize - byteLenght) completeBytes[i] = bytes[i - typeSize + byteLenght];
            else completeBytes[i] = 0;
        }
    }

    for(int i=0; i<typeSize; i++)
    {
        if(bigEndian) retval |= ( (T)completeBytes[i] << i*8);
        else retval |= ( (T)bytes[i] >> i*8);
    }

    return retval;
}


#endif // TOOLZ_H_INCLUDED
