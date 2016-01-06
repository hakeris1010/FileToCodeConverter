#include "toolz.h"
#include <windows.h>
#include <limits>

std::string Fun::GetOpenSaveFileWindows(int mode, const char filterLine[])
{
    OPENFILENAME ofn;
    TCHAR fname[ MAX_PATH ];
    std::string filename;

    ZeroMemory( &ofn , sizeof(ofn));

    ofn.lStructSize = sizeof ( ofn );
    ofn.hwndOwner = NULL ;
    ofn.lpstrFile = fname ;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof( fname );
    ofn.lpstrFilter = filterLine;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL ;
    ofn.nMaxFileTitle = 0 ;
    ofn.lpstrInitialDir = NULL ;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

    bool retval=false;

    if(mode==1) retval = GetOpenFileName(&ofn);
    else if(mode==2) retval = GetSaveFileName(&ofn);

    if(retval) filename=fname;
    else filename="No file selected.";

    return filename;
}

bool Fun::isDivided(int64_t num, int64_t by_what)
{
    if(num % by_what == 0) return true;
    return false;
}

std::string Fun::reverseString(std::string str)
{
    std::string zum;
    for(int i=str.size()-1; i>=0; i--) zum.push_back(str[i]);
    return zum;
}

std::string Fun::getFilenameFromPath(std::string fap)
{
    std::string zam;
    for(int i=fap.size()-1; i>=0; i--)
    {
        if(fap[i]!='/' && fap[i]!=char(92))  zam.push_back(fap[i]);
        else break;
    }
    return reverseString(zam);
}

std::string Fun::getFileExtension(std::string filename)
{
    std::string temp;
    bool dotFound=false;

    for(int i=filename.size()-1; i>=0; i--)
    {
        if(filename[i]=='.')
        {
            dotFound=true;
            break;
        }
        temp.push_back(filename[i]);
    }

    if(dotFound) return reverseString(temp);
    return " ";
}

//end;
