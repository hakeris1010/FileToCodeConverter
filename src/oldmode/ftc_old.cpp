/*File To Code [Procedural] Converter (Der Alte) v0.5.x, by H3nt4iBoY (2015)
* main implementation CPP file
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "tools/toolz.h"
#include "structures.h"
#include "ftc_old.h"
#include "tools/debug.h"
#include "debdefines.h"


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
        if((fname[i]<'a' || fname[i]>'z') && (fname[i]<'A' || fname[i]>'Z') && ((fname[i]<'0' || fname[i]>'9') || (fname[i]>='0' && fname[i]<='9' && i==0))) fname[i]='_';
    }
    if(mode==2) return fname+"_data";
    else if(mode==3) return fname+"_size";

    return fname;
}

std::string FileToCodeProcedural::charToWritable(char ch, int mode, bool onBreak)
{
    std::string rv;
    rv.push_back(ch);

    switch(ch)
    {
        case 0:
            if(onBreak) return "\\0";
            else return "\\0\"\"";
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
    bool dbg = DEBUG_FTCPROCEDURAL_CPP_CONVERT;

    if(dbg) deb<<"\n--------\nFTCProc::CPP_convert():\n\nSetting inp outp...\n";

    std::ifstream input(inname.c_str(), std::fstream::binary);
    std::ofstream outp(outname.c_str());

    if(!input.is_open()) return 1;
    if(!outp.is_open()) return 2;

    if(dbg) deb<<"input - getting file size:\nseekg(end)\n";
    input.seekg(0, input.end);

    if(dbg) deb<<"tellg()\n";
    long fileSize = input.tellg();

    if(dbg) deb<<"seekg(beg)\n";
    input.seekg(0, input.beg);

    if(dbg) deb<<"FileSize="<<fileSize<<"\n";

    if(dbg) deb<<"setting file extensions\n";

    std::string ext1 = Fun::getFileExtension(inname);
    std::string ext2 = Fun::getFileExtension(outname);

    if(dbg) deb<<"setting data buffer [fileSize]\n";
    char *data = new char[ fileSize ]; //mode==WHOLE_FILE

    if(dbg) deb<<"inp: read buffer!\n";
    input.read(data, fileSize);
    if(dbg) deb<<"inp: close()\n";
    input.close();

    bool isHeader=false;

    if(dbg) deb<<"outp: writing header stuff\n";

    if( ext2=="h" || ext2=="hpp" || ext2=="hxx" )
    {
        outp<<"#ifndef "<<headerTitle(outname)<<"\n";
        outp<<"#define "<<headerTitle(outname)<<"\n\n";
        isHeader=true;
    }

    outp<<"/*App Made By Hakeris1010. (C) hakeris1010 inc. 2015 \n\n";
    outp<<"File: "<<inname<<" */\n\n";

    std::cout<<"\nFile size: "<< fileSize <<" bytes.\n";

    if(dbg) deb<<"Start checking for modes, setting start props\n";

    int startPos=0;
    int arrLen=0;
    std::string arrName;
    int identifiedFormat=0;

    if(dbg) deb<<"getting arrName\n";

    if(arrayNameMode==OLDMODE_ARRAY_NAME_OUTPUT_FILENAME) arrName=getArrayName(outname);
    else if(arrayNameMode==OLDMODE_ARRAY_NAME_INPUT_FILENAME) arrName=getArrayName(inname);
    else if(arrayNameMode==OLDMODE_ARRAY_NAME_CUSTOM) arrName=getArrayName(customArrayName);

    outp<<"long "<<arrName<<"_filesize = "<< fileSize <<"; //size in bytes \n";

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

        if(dbg) deb<<"Binary identified\n";
    }

    if(arrLen > fileSize) arrLen=fileSize;

    if(dbg) deb<<"arrLen="<<arrLen<<"\noutping array declaration\n";

    outp<<"long "<< arrName <<"_lenght = "<< arrLen <<";\n\n";

    if(writeMode==OLDMODE_WRITE_NUMBERS) outp<<"unsigned char "<< arrName <<"_data[ "<< arrLen <<" ] = {\n";
    else outp<<"const char "<< arrName <<"_data[] = \n\""; //not fully workz0rin'

    if(dbg) deb<<"Loop start!\n\n";

    const int charBreak=60, numBreak=20;
    int breakPos;
    bool onBreak;
    (writeMode==OLDMODE_WRITE_NUMBERS) ? breakPos=numBreak : breakPos=charBreak;

    //write data
    for(int i=startPos; i<fileSize; i++)
    {
        if(i-startPos >= arrLen) break;

        onBreak=false;
        if( i!=startPos && (i-startPos)<arrLen-1 && Fun::isDivided(i-startPos, breakPos)) onBreak=true;

        if(writeMode==OLDMODE_WRITE_NUMBERS)
        {
            if(i!=startPos) outp<<","<< (int)( (uint8_t)data[i] );
            else outp<< (int)( (uint8_t)data[i] );
        }
        else outp<< charToWritable( data[i], identifiedFormat, onBreak );

        if(writeMode==OLDMODE_WRITE_NUMBERS && onBreak) outp<<"\n";
        else if(writeMode==OLDMODE_WRITE_CHARS && onBreak) outp<<"\"\n\"";
    }

    if(dbg) deb<<"Loop ended. Outp ending stuff.\n";

    if(writeMode==OLDMODE_WRITE_NUMBERS) outp<<" };\n";
    else outp<<"\";";

    if(isHeader) outp<<"\n\n#endif //"<<headerTitle(outname)<<"\n";

    if(dbg) deb<<"Clearing garbage...\n";

    outp.close();
    delete [] data;

    if(dbg) deb<<"Mission complete! Return!\n\n------------\n";

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
