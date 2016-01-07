/* File To Code Converter class v0.5
Created by H3nt4iBoY a.k.a. Hakeris1010  ((C) 2015)

-NOT COMPLETED
*/

#ifndef FILETOCODE_H_INCLUDED
#define FILETOCODE_H_INCLUDED

#include "structures.h"
#include <string>
#include <fstream>

#define FILE_TO_CODE_VERSION "v0.5"

//base chunk lenght
#define BASE_CHUNK_LENGHT_ELEMENTS 128

//modes

//main modes
//element write modes
#define CONVERTMODE_ELEMENT_WRITE_MODE_CHARS   1
#define CONVERTMODE_ELEMENT_WRITE_MODE_NUMBERS 2

//filetype modes
#define CONVERTMODE_FILETYPE_BINARY    10
#define CONVERTMODE_FILETYPE_TEXT      11
#define CONVERTMODE_FILETYPE_PICTURE   12
#define CONVERTMODE_FILETYPE_AUTO      13

//layout modes
#define CONVERTMODE_LAYOUT_VARIABLES   20
#define CONVERTMODE_LAYOUT_STRUCT      21

//file reading modes
#define CONVERTMODE_READ_FILE_WHOLE    30
#define CONVERTMODE_READ_FILE_CHUNKS   31
#define CONVERTMODE_READ_FILE_WHOLE_BACKWARDS 32

//filetype-specific modes
//picture
#define CONVERTMODE_PICTURE_RGB   50
#define CONVERTMODE_PICTURE_RGBA  51
#define CONVERTMODE_PICTURE_AUTO  52

//text
#define CONVERTMODE_TEXT_NOENDL   60
#define CONVERTMODE_TEXT_ENDL_N   61
#define CONVERTMODE_TEXT_ENDL_BR  62

class FileToCodeConverter
{
private:
    //variables: modes
    //main modes
    int elementWriteMode, filetypeMode, layoutMode, fileReadMode;

    //file-specific modes
    bool isThisSpecial=false;
    int specPictureRgbMode; //pic modes
    int specTextEndlMode; //text file modes

    //file chunk writing properties
    int bytes_per_element, chunk_lenght;
    long fileSize, arrLenght;

    //stuff written to code file
    std::string dataType, arrayName;

    //filenames
    std::string inputFileName, outputFileName;

    //additional info stuff
    bool areAdditionalInfo=false, additInfoStructFull=false;
    std::string additional_info_string;
    AdditionalInfo add_inf; //struct in which info is stored

    //file objects
    std::ifstream inp;
    std::ofstream outp;
    FILE* fin=NULL;
    FILE* fout=NULL;

    //---  functions  ---
    void assignDefaultProperties();
    void getSpecialModes( int sp_mode );
    int getBytesPerElementFromMode();
    std::string getArrayDataType();

    std::string getHeaderIncludeGuard(std::string filename);
    std::string getArrayNameFromFileName(std::string filename, int mode=1);
    int getAdditionalInfo(char* data, int lenght, int howManyChunksPassed);

    void writeAdditionalInfoToFile(int lang); //1- C, 2- CPP
    void writeFileBeginning(int lang);
    void writeFileEnd(int lang);

    void writeDataToFileC(char* data, int lenght, int offset);
    void writeDataToFileCPP(std::string data, int offset);

    int convertFileC();
    int convertFileCPP();

public:
    FileToCodeConverter();
    FileToCodeConverter(int element_write_mode, int filetype_mode, int layout_mode, int file_read_mode, int special_mode);
    ~FileToCodeConverter();

    void create(int element_write_mode, int filetype_mode, int layout_mode, int file_read_mode, int special_mode);
    void changeMode(int id, int newmode);
    int getModeID(int mode_type);

    int convertFile(std::string inputFile, std::string outputFile, bool useCPP=true, std::string customArrayName="...");
};

#endif // FILETOCODE_H_INCLUDED
