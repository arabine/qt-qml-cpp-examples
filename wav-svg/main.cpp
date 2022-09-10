#include <vector>
#include <iostream>
#include <string>

#include "Converter.h"

std::string GetSuffix(const std::string &path)
{
    return path.substr(path.find_last_of(".") + 1);
}

std::string GetFullBaseName(const std::string &path)
{
    std::string suffix = GetSuffix(path);
    std::string basename = path;
    basename.erase(path.size()-(suffix.size()+1));
    return basename;
}

void PrintHelp(const char *exeName)
{
    std::cout << exeName << " - WAV from/to SVG converter" << std::endl;
    std::cout << "Usage: " << exeName << " myfile.[wav|svg]" << std::endl;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    if (argc > 1)
    {
        std::cout << GetFullBaseName(argv[1]) << std::endl;

        std::string baseName = GetFullBaseName(argv[1]);
        std::string suffix = GetSuffix(argv[1]);

        if (suffix == "svg")
        {
            SvgToWav(argv[1], baseName + "_output.wav", 44100, 16);
        }
        else if(suffix == "wav")
        {
            WavToSvg(argv[1], baseName + "_output.svg");
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -2;
    }

    if (ret != 0)
    {
        PrintHelp(argv[0]);
    }

    return ret;
}
