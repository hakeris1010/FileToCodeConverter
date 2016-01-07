/*File To Code [Procedural] Converter (Der Alte) v0.5.2, by H3nt4iBoY (2015)
- Worx0rin' g00d!
- 2 Procedures, hybrid (legacy) and fully C++ (new, 0.5);
- Supports 3 convert file modes (OLDMODE's) - Picture, Binary, and Text (can be set to Auto)
- Fully supports number write mode (legacy), new char write mode is fuuly supported now!!! (but needz more test1ng)
*/

#ifndef FTC_OLD_H_INCLUDED
#define FTC_OLD_H_INCLUDED

#include <string>

#define ALTE_VERSION "v0.5.2"

#define OLDMODE_BINARY 1
#define OLDMODE_PICTURE 2
#define OLDMODE_TEXT 3
#define OLDMODE_AUTO 4

#define OLDMODE_READ_WHOLE_FILE 10
#define OLDMODE_READ_BY_CHUNKS 11

#define OLDMODE_WRITE_NUMBERS 30
#define OLDMODE_WRITE_CHARS 31

#define OLDMODE_ARRAY_NAME_PREDEFINED 40
#define OLDMODE_ARRAY_NAME_OUTPUT_FILENAME 41
#define OLDMODE_ARRAY_NAME_INPUT_FILENAME 42
#define OLDMODE_ARRAY_NAME_CUSTOM 43

class FileToCodeProcedural
{
private:
    //Funcshons
    static std::string headerTitle(std::string zam2);
    static std::string getArrayName(std::string fname, int mode=1);
    static std::string charToWritable(char ch, int mode, bool onBreak);
public:
    static int CPP_convert(std::string inname, std::string outname, int fileMode, int readMode, int writeMode, int arrayNameMode, std::string customArrayName="...");
    static int C_convert(std::string fname, std::string outname, int mode=1);
};

#endif // FTC_OLD_H_INCLUDED
