/*File To Code [Procedural] Converter (Der Alte) v0.5.1, by H3nt4iBoY (2015)
- Worx0rin' g00d!
- 2 Procedures, hybrid (legacy) and fully C++ (new, 0.5);
- Supports 3 convert file modes (OLDMODE's) - Picture, Binary, and Text (can be set to Auto)
- Fully supports number write mode (legacy), new char write mode is partly supported (only on TEXT filemode)
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "toolz.h"
#include "structures.h"
#include "ftc_old.h"


std::string FileToCodeProcedural::headerTitle(std::string zam2)
{
    zam2 = Fun::getFilenameFromPath( zam2 );

    for(int i=0; i<zam2.size(); i++)
    {
        if(zam2[i]>='a' && zam2[i]<='z') zam2[i]-=32;
        if(zam2[i]=='.') zam2[i]='_';
    }

    return zam2+"_INCLUDED";
}

std::string FileToCodeProcedural::getArrayName(std::string fname, int mode)
{
    fname = Fun::getFilenameFromPath( fname );

    for(int i=0; i<fname.size(); i++)
    {
        if((fname[i]<'a' || fname[i]>'z') && (fname[i]<'A' || fname[i]>'Z')) fname[i]='_';
    }
    if(mode==2) return fname+"_data";
    else if(mode==3) return fname+"_size";

    return fname;
}

std::string FileToCodeProcedural::charToWritable(char ch, int mode)
{
    std::string rv;
    rv.push_back(ch);

    switch(ch)
    {
        case 0:  return "\\0";
        case 10: return "\\n";
        case 9:  return "\\t";
        case 11: return "\\v";
        case 8:  return "\\b";
        case 13: return "\\r";
        case 12: return "\\f";
        case 7:  return "\\a";
        case 92: return "\\\\";
        case 63: return "\\?";
        case 39: return "\\'";
        case 34:
            rv="\\";
            rv.push_back(char(34));
    }

    return rv;
}

//New Ones
int FileToCodeProcedural::CPP_convert(std::string inname, std::string outname, int fileMode, int readMode, int writeMode, int arrayNameMode, std::string customArrayName)
{
    std::ifstream input(inname.c_str(), std::fstream::binary);
    std::ofstream outp(outname.c_str());

    if(!input.is_open()) return 1;
    if(!outp.is_open()) return 2;

    input.seekg(0, input.end);
    long fileSize = input.tellg();
    input.seekg(0, input.beg);

    std::string ext1 = Fun::getFileExtension(inname);
    std::string ext2 = Fun::getFileExtension(outname);

    char data[ fileSize ]; //mode==WHOLE_FILE
    input.read(data, fileSize);
    input.close();

    bool isHeader=false;

    if( ext2=="h" || ext2=="hpp" || ext2=="hxx" )
    {
        outp<<"#ifndef "<<headerTitle(outname)<<"\n";
        outp<<"#define "<<headerTitle(outname)<<"\n\n";
        isHeader=true;
    }

    outp<<"/*App Made By Hakeris1010. (C) hakeris1010 inc. 2015 \n\n";
    outp<<"File: "<<inname<<" */\n\n";

    std::cout<<"\nFile size: "<< fileSize <<" bytes.\n";
    outp<<"long filesize = "<< fileSize <<"; //size in bytes \n";

    int startPos=0;
    int arrLen=0;
    std::string arrName;
    int identifiedFormat=0;

    if(arrayNameMode==OLDMODE_ARRAY_NAME_OUTPUT_FILENAME) arrName=getArrayName(outname);
    else if(arrayNameMode==OLDMODE_ARRAY_NAME_INPUT_FILENAME) arrName=getArrayName(inname);
    else if(arrayNameMode==OLDMODE_ARRAY_NAME_CUSTOM) arrName=getArrayName(customArrayName);

    if(ext1=="bmp" && (fileMode==OLDMODE_PICTURE || fileMode==OLDMODE_AUTO) ) //PICTURE
    {
        std::cout<<"\nWOW! It's a Bitmap!\n";
        identifiedFormat=OLDMODE_PICTURE;

        int pix = data[10] | ( (int)data[11] << 8 ) | ( (int)data[12] << 16 ) | ( (int)data[13] << 24 );
        int wid = data[18] | ( (int)data[19] << 8 ) | ( (int)data[20] << 16 ) | ( (int)data[21] << 24 );
        int hei = data[22] | ( (int)data[23] << 8 ) | ( (int)data[24] << 16 ) | ( (int)data[25] << 24 );
        int bpp = data[28] | ( (int)data[29] << 8 );

        std::cout<<"\nWidth: "<<wid<<"\n";
        std::cout<<"Height: "<<hei<<"\n";
        std::cout<<"Bits Per Pixel: "<<bpp<<"\n";

        outp<<"int "<<arrName<<"_pic_width = "<<wid<<";\n";
        outp<<"int "<<arrName<<"_pic_height = "<<hei<<";\n";
        outp<<"int "<<arrName<<"_pic_bits_per_pixel = "<<bpp<<";\n";

        startPos = pix;
        arrName+="_pixel_buffer";

        /*if(writeMode==OLDMODE_WRITE_CHARS) arrLen = wid*hei*(bpp/8) +1;
        else arrLen = wid*hei*(bpp/8);*/

        arrLen = wid*hei*(bpp/8);
    }

    else if((ext1=="txt" && fileMode==OLDMODE_AUTO) || fileMode==OLDMODE_TEXT) //TEXT
    {
        std::cout<<"\nText file identified!\n";
        identifiedFormat=OLDMODE_TEXT;

        startPos = 0;
        arrName+="_text_buffer";

        //arrLen = fileSize +1;
        arrLen = fileSize;

        writeMode = OLDMODE_WRITE_CHARS; //important!
    }

    else //BINARY
    {
        identifiedFormat=OLDMODE_BINARY;
        startPos = 0;
        arrName+="_file_buffer";

        /*if(writeMode==OLDMODE_WRITE_CHARS) arrLen = fileSize + 1;
        else arrLen = fileSize;*/

        arrLen = fileSize;
    }

    if(arrLen > fileSize) arrLen=fileSize;

    outp<<"long "<< arrName <<"_lenght = "<< arrLen <<";\n\n";

    if(writeMode==OLDMODE_WRITE_NUMBERS) outp<<"unsigned char "<< arrName <<"_data[ "<< arrLen <<" ] = {\n";
    else outp<<"const char "<< arrName <<"_data[] = \n\""; //not fully workz0rin'

    //write data
    for(int i=startPos; i<fileSize; i++)
    {
        if(i-startPos >= arrLen) break;

        if( i!=startPos && (i-startPos)<arrLen-1 )
        {
            if(writeMode==OLDMODE_WRITE_NUMBERS && Fun::isDivided(i-startPos, 20) ) outp<<"\n";
            else if(writeMode==OLDMODE_WRITE_CHARS && Fun::isDivided(i-startPos, 60) ) outp<<"\"\n\"";
        }

        if(writeMode==OLDMODE_WRITE_NUMBERS)
        {
            if(i!=startPos) outp<<","<< (int)( (uint8_t)data[i] );
            else outp<< (int)( (uint8_t)data[i] );
        }
        else outp<< charToWritable( data[i], identifiedFormat );
    }

    if(writeMode==OLDMODE_WRITE_NUMBERS) outp<<" };\n";
    else outp<<"\";";

    if(isHeader) outp<<"\n\n#endif //"<<headerTitle(outname)<<"\n";

    outp.close();

    return 0;
}

//Old Ones
int FileToCodeProcedural::C_convert(std::string fname, std::string outname, int mode)
{
    std::ofstream outp;

    FILE *fil;
    unsigned char *bytes;
    long ilgis,i;

    fil=fopen(fname.c_str(),"rb");

    fseek(fil,0,SEEK_END);
    ilgis=ftell(fil);

    rewind(fil);

    bytes = (unsigned char *)malloc((ilgis+1)*sizeof(unsigned char)); // Enough memory for file + \0
    fread(bytes,ilgis,1,fil); // Read in the entire file
    fclose(fil); // Close the file

    outp.open(outname.c_str());
    if(!outp.is_open()) return -1;

    bool isHeader=false;

    if( (outname[outname.size()-1]=='h' && outname[outname.size()-2]=='.') || (outname[outname.size()-1]=='p' &&
         outname[outname.size()-2]=='p' && outname[outname.size()-3]=='h' && outname[outname.size()-4]=='.') )
    {
        outp<<"#ifndef "<<headerTitle(outname)<<"\n";
        outp<<"#define "<<headerTitle(outname)<<"\n\n";
        isHeader=true;
    }

    outp<<"/*App Made By Hakeris1010. (C) hakeris1010 inc. 2015 \n\n";
    outp<<"File: "<<fname<<" */ \n\n";

    std::cout<<"\nFile size: "<<ilgis<<" bytes.\n";

    outp<<"int filesize = "<<ilgis<<"; //size in bytes \n";

    BitMap mypic;

    if(fname[fname.size()-1]=='p' && fname[fname.size()-2]=='m'
       && fname[fname.size()-3]=='b' && fname[fname.size()-4]=='.')
    {
        std::cout<<"WOW!\n";

        unsigned char wd[4]={bytes[18], bytes[19], bytes[20], bytes[21]};
        unsigned char hg[4]={bytes[22], bytes[23], bytes[24], bytes[25]};

        int wid = wd[0] | ( (int)wd[1] << 8 ) | ( (int)wd[2] << 16 ) | ( (int)wd[3] << 24 );
        int hei = hg[0] | ( (int)hg[1] << 8 ) | ( (int)hg[2] << 16 ) | ( (int)hg[3] << 24 );
        int bpp=int(bytes[28]);

        std::cout<<"\nWidth: "<<wid<<"\n";
        std::cout<<"Height: "<<hei<<"\n";
        std::cout<<"Bits Per Pixel: "<<bpp<<"\n";

        mypic.bits_per_pixel=bpp;
        mypic.height=hei;
        mypic.width=wid;

        outp<<"int width = "<<wid<<";\n";
        outp<<"int height = "<<hei<<";\n";
        outp<<"int bits_per_pixel = "<<bpp<<";\n";
        outp<<"unsigned char pixel_data[ "<<wid*hei*(bpp/8)+1<<" ] = \n{ ";

        for(i=ilgis-1; i>(ilgis-1-(wid*hei*bpp/8)); i--)
        {
            if(Fun::isDivided(i,20)) outp<<" \n";

            outp<<int(bytes[i]);

            if(i!=(ilgis-(wid*hei*bpp/8))) outp<<", ";
            else outp<<" }; \n";

            //cout<<int(bytes[i])<<" ";
        }
    }
    else
    {
        outp<<"unsigned char file_data[ "<<ilgis<<" ] = \n{ ";

        for(i=0; i<ilgis; i++)
        {
            if(Fun::isDivided(i,20) && i!=0) outp<<"\n";

            outp<<int(bytes[i]);

            if(i!=ilgis-1) outp<<", ";
            else outp<<" }; \n";

            //cout<<int(bytes[i])<<" ";
        }
    }

    if(isHeader) outp<<"\n\n#endif //"<<headerTitle(outname)<<"\n";

    delete [] bytes;

    outp.close();

    return 0;
}
