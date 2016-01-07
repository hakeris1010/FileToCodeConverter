#include <iostream>
#include <string>
#include "tools/toolz.h"
#include "oldmode/ftc_old.h"
#include "starter.h"

using namespace std;

int StartTheConverter(int mode)
{
    bool q=true;

    while(q)
    {
        string fl,fl2;
        cout<<"File to C code, "<<ALTE_VERSION<<endl<<endl;
        cout<<"Select a file"<<endl<<endl;

        fl=Fun::GetOpenSaveFileWindows(1);

        cout<<fl<<endl<<endl;
        if(fl=="No file selected.") break;

        cout<<"Select where to put a code"<<endl<<endl;

        fl2=Fun::GetOpenSaveFileWindows(2, "C Header Files\0 *.h;*.hpp;*.hxx\0C Code Files\0 *.c;*.cpp;*.cxx\0\0");

        cout<<fl2<<endl<<endl;
        if(fl2=="No file selected.") break;

        cout<<"Starting conversion..."<<endl;

        /*if(C_convert(fl,fl2)!=0) cout<<"Oops! Something went wrong!"<<endl;
        else cout<<endl<<"Operation Successful!"<<endl;*/

        if(FileToCodeProcedural::CPP_convert(fl, fl2, OLDMODE_AUTO, OLDMODE_READ_WHOLE_FILE, OLDMODE_WRITE_CHARS, OLDMODE_ARRAY_NAME_OUTPUT_FILENAME) !=0) cout<<"Oops! Something went wrong!"<<endl;
        else cout<<endl<<"Operation Successful!"<<endl;

        char c;
        cout<<endl<<"Use again? [y/n]   ";
        cin>>c;
        if(c=='n') q=false;
    }

    return 0;
}
