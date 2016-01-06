#include "filetocode.h"
#include "toolz.h"
#include <cstdlib>
#include <sstream>

FileToCodeConverter::FileToCodeConverter()
{
    assignDefaultProperties();
}

FileToCodeConverter::FileToCodeConverter(int element_write_mode, int filetype_mode, int layout_mode, int file_read_mode, int special_mode)
{
    create( element_write_mode, filetype_mode, layout_mode, file_read_mode, special_mode );
}

FileToCodeConverter::~FileToCodeConverter()
{
    inp.close();
    outp.close();
    if(fin) fclose(fin);
    if(fout) fclose(fout);
}

void FileToCodeConverter::create(int element_write_mode, int filetype_mode, int layout_mode, int file_read_mode, int special_mode)
{
    elementWriteMode = element_write_mode;
    filetypeMode =     filetype_mode;
    layoutMode =       layout_mode;
    fileReadMode =     file_read_mode;

    getSpecialModes( special_mode );

    getBytesPerElementFromMode();
    chunk_lenght =     BASE_CHUNK_LENGHT_ELEMENTS * bytes_per_element;

    dataType =         getArrayDataType();
}

void FileToCodeConverter::assignDefaultProperties()
{
    elementWriteMode= CONVERTMODE_ELEMENT_WRITE_MODE_NUMBERS;
    filetypeMode=     CONVERTMODE_FILETYPE_BINARY;
    layoutMode=       CONVERTMODE_LAYOUT_VARIABLES;
    fileReadMode=     CONVERTMODE_READ_FILE_CHUNKS;

    bytes_per_element= 1;
    chunk_lenght=     BASE_CHUNK_LENGHT_ELEMENTS;

    dataType=         getArrayDataType();
}

void FileToCodeConverter::getSpecialModes( int sp_mode )
{
    if(filetypeMode == CONVERTMODE_FILETYPE_PICTURE)
    {
        isThisSpecial=true;
        if(sp_mode == CONVERTMODE_PICTURE_RGB || sp_mode == CONVERTMODE_PICTURE_RGBA) specPictureRgbMode = sp_mode;
        else specPictureRgbMode == CONVERTMODE_PICTURE_AUTO;
    }
    if(filetypeMode == CONVERTMODE_FILETYPE_TEXT)
    {
        isThisSpecial=true;
        if(sp_mode == CONVERTMODE_TEXT_ENDL_BR || sp_mode == CONVERTMODE_TEXT_NOENDL) specTextEndlMode = sp_mode;
        else specTextEndlMode = CONVERTMODE_TEXT_ENDL_N;
    }

    isThisSpecial=false;
}

/*void FileToCodeConverter::getBasicModesFromConvertmode() //UNUSED
{
    dataType = getArrayDataType();

    if(dataType == "unsigned char") writingElementsMode=ELEMENT_WRITE_MODE_NUMBERS;
    else writingElementsMode=ELEMENT_WRITE_MODE_CHARS;

    if(convertMode==CONVERTMODE_PICTURE_RGBA_CHARS || convertMode==CONVERTMODE_PICTURE_RGB_CHARS ||
       convertMode==CONVERTMODE_PICTURE_RGBA_NUMBERS || convertMode==CONVERTMODE_PICTURE_RGB_NUMBERS )
        convertFiletypeMode = CONVERTMODE_PICTURE;

    else if(convertMode==CONVERTMODE_TEXT_STRING_NOENDL || convertMode==CONVERTMODE_TEXT_STRING_ENDL_N ||
            convertMode==CONVERTMODE_TEXT_STRING_ENDL_BR) convertFiletypeMode
}*/

int FileToCodeConverter::getBytesPerElementFromMode()
{
    return 1;
}

std::string FileToCodeConverter::getArrayDataType()
{
    if(elementWriteMode==CONVERTMODE_ELEMENT_WRITE_MODE_CHARS) return "char";

    return "unsigned char";
}

void FileToCodeConverter::changeMode(int id, int mode)
{
    if(id==0) elementWriteMode=mode;
    else if(id==1) filetypeMode=mode;
    else if(id==2) layoutMode=mode;
    else if(id==3) fileReadMode=mode;

    else if(id==5) specPictureRgbMode=mode;
    else if(id==6) specTextEndlMode=mode;
}

int FileToCodeConverter::getModeID(int mode_type)
{
    return mode_type/10;
}

std::string FileToCodeConverter::getHeaderIncludeGuard(std::string zam2)
{
    zam2 = Fun::getFilenameFromPath( zam2 );

    for(int i=0; i<zam2.size(); i++)
    {
        if(zam2[i]>='a' && zam2[i]<='z') zam2[i]-=32;
        if(zam2[i]=='.') zam2[i]='_';
    }

    return zam2+"_INCLUDED";
}

std::string FileToCodeConverter::getArrayNameFromFileName(std::string fname, int mode)
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

int FileToCodeConverter::convertFile(std::string inputFile, std::string outputFile, bool useCPP, std::string customArrayName)
{
    if(customArrayName == "...") arrayName = getArrayNameFromFileName(outputFile);
    else arrayName = getArrayNameFromFileName(customArrayName);

    inputFileName = inputFile;
    outputFileName = outputFile;

    int retval=0;

    if(useCPP) retval=convertFileCPP();
    else retval=convertFileC();

    return retval;
}

int FileToCodeConverter::getAdditionalInfo(char* data, int lenght, int howManyChunksPassed) //retval: >0 - writable data beginning in chunk, -1 - no writable data.
{
    //get picture properties and other stuff, do start tasks;
    std::string ext = Fun::getFileExtension(inputFileName);
    std::stringstream ssr;
    int retval = 0;

    if(howManyChunksPassed==0)
    {
        areAdditionalInfo=true;
        ssr <<"long "<< arrayName <<"_filesize = "<< fileSize <<";\n\n";

        if(ext=="bmp" && filetypeMode==CONVERTMODE_FILETYPE_PICTURE)
        {
            if(lenght > 29 )
            {
                add_inf.file_type = FILETYPE_BITMAP;
                add_inf.supported = true;

                int pix = data[10] | ( (int)data[11] << 8 ) | ( (int)data[12] << 16 ) | ( (int)data[13] << 24 );
                int wid = data[18] | ( (int)data[19] << 8 ) | ( (int)data[20] << 16 ) | ( (int)data[21] << 24 );
                int hei = data[22] | ( (int)data[23] << 8 ) | ( (int)data[24] << 16 ) | ( (int)data[25] << 24 );
                int bpp = data[28] | ( (int)data[29] << 8 );

                add_inf.pic.where_pixels_start = pix;
                add_inf.pic.width = wid;
                add_inf.pic.height = hei;
                add_inf.pic.bits_per_pixel = bpp;

                add_inf.till_where_to_read = pix + wid*hei;

                ssr<<"int "<< arrayName <<"_bmp_width = "<< wid <<";\n";
                ssr<<"int "<< arrayName <<"_bmp_height = "<< hei <<";\n";
                ssr<<"int "<< arrayName <<"_bmp_bits_per_pixel = "<< bpp <<";\n\n";

                ssr<< dataType <<" "<< arrayName <<"_data[ "<<fileSize<<" ] = ";
                if(elementWriteMode == CONVERTMODE_ELEMENT_WRITE_MODE_NUMBERS) ssr<<"{\n";
                else ssr<<char(34)<<"\n";

                additInfoStructFull = true;

                retval = pix;
            }
            else retval = -1;
        }

        else if(filetypeMode==CONVERTMODE_FILETYPE_TEXT){  }

        else //if(convertFiletypeMode==CONVERTMODE_BINARY) and others (default)
        {
            add_inf.file_type = FILETYPE_BINARY;
            add_inf.supported = true;
            add_inf.till_where_to_read = fileSize;

            ssr<< dataType <<" "<< arrayName <<"_data[ "<<fileSize<<" ] = ";
            if(elementWriteMode == CONVERTMODE_ELEMENT_WRITE_MODE_NUMBERS) ssr<<"{\n";
            else ssr<<char(34)<<"\n";

            additInfoStructFull = true;
        }
    }

    additional_info_string = ssr.str();

    return retval;
}

void FileToCodeConverter::writeAdditionalInfoToFile(int lang)
{
    if(!areAdditionalInfo) return;

    if(lang==1) fprintf(fout, "%s", additional_info_string.c_str());
    else if(lang==2){ }

    additional_info_string=std::string();
    areAdditionalInfo=false;
}

void FileToCodeConverter::writeFileBeginning(int lang)
{
    std::string ext = Fun::getFileExtension(inputFileName);
    if(lang==1)
    {
        if(ext=="h" || ext=="hpp" || ext=="hxx")
        {
            fprintf(fout, "#ifndef %s\n", getHeaderIncludeGuard(inputFileName).c_str());
            fprintf(fout, "#define %s\n\n", getHeaderIncludeGuard(inputFileName).c_str());
        }
        fprintf(fout, "/*File To Code Converter %s By Hakeris1010. (C) hakeris1010 inc. 2015 \n\n", FILE_TO_CODE_VERSION);
        fprintf(fout, "Input File: %s */\n\n", inputFileName.c_str());
    }
    else if(lang==2){ }
}

void FileToCodeConverter::writeFileEnd(int lang)
{
    std::string ext = Fun::getFileExtension(inputFileName);
    if(lang==1)
    {
        //some string end symbols
        if(ext=="h" || ext=="hpp" || ext=="hxx") fprintf(fout, "#endif // %s\n", getHeaderIncludeGuard(inputFileName).c_str());
    }
    else if(lang==2){ }
}

// C Style
int FileToCodeConverter::convertFileC()
{
    fin = fopen(inputFileName.c_str(), "rb");
    fout = fopen(outputFileName.c_str(), "w");

    if(!fin) return 1;
    if(!fout) return 2;

    long file_lenght=0;
    int err=0;

    if(fseek(fin, 0, SEEK_END)!=0) return 3;
    file_lenght = ftell(fin);
    rewind(fin);

    fileSize = file_lenght; //class var
    writeFileBeginning(1);

    int bytes_passed=0, loop=0, fromWhereToStart=0;
    bool writableDataStarted=false;

    while(bytes_passed <= file_lenght)
    {
        char* buff;
        int blocklen=0;

        if(file_lenght - bytes_passed >= chunk_lenght) //assigning buffer lenghts
        {
            buff = (char*)malloc(chunk_lenght * sizeof(char));
            blocklen= chunk_lenght;
        }
        else
        {
            buff = (char*)malloc( (file_lenght - bytes_passed) * sizeof(char));
            blocklen= file_lenght - bytes_passed;
        }

        if(fread(buff, sizeof(char), blocklen, fin) != sizeof(char)*blocklen) err=4+loop;

        fromWhereToStart = getAdditionalInfo(buff, blocklen, loop);

        writeAdditionalInfoToFile(1);

        if(fromWhereToStart < 0) writableDataStarted=false;
        else writableDataStarted=true;

        if(writableDataStarted) writeDataToFileC(buff, blocklen, fromWhereToStart);

        free(buff);
        bytes_passed = ftell(fin);
        loop++;
    }

    //writeFileEndC();

    fclose(fin);
    fclose(fout);
    fin=NULL;
    fout=NULL;

    return err;
}

void FileToCodeConverter::writeDataToFileC(char* data, int lenght, int offset)
{

}

// C Plus Plus Style
int FileToCodeConverter::convertFileCPP()
{

}

void FileToCodeConverter::writeDataToFileCPP(std::string data, int offset)
{

}

//end;
