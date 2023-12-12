#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>

#ifdef WIN32
#include <windows.h>
#include <processenv.h>
#include <shellapi.h>
#endif


namespace fs = std::filesystem;

void compile_rec(int argc, char **argv)
{
    #ifdef WIN32
    LPWSTR cmd = GetCommandLineW();
    LPWSTR *argvw = CommandLineToArgvW(cmd, &argc);
    std::wstring zero_arg(argvw[0]);
    #else
    std::string zero_arg = argv[0];
    #endif

    fs::path binary_path = zero_arg;
    fs::path binary_parent = binary_path.parent_path();
    fs::path absolute_binary_parent = fs::absolute(binary_parent);

    std::vector<fs::path> files;
    for (auto const& dir_entry : fs::recursive_directory_iterator(absolute_binary_parent))
    {
        files.push_back(dir_entry.path());
    }

    // for (auto const& s : files)
    // {
    //     std::cout << s << '\n';
    // }

    std::vector<fs::path> htmle_files;
    std::copy_if(files.begin(), files.end(), std::back_inserter(htmle_files), [](fs::path i) {
        return i.extension().compare(".htmle") == 0;
    });


    for (auto const& s : htmle_files)
    {
        std::cout << s << '\n';
    }
}


int main(int argc, char **argv)
{
    compile_rec(argc, argv);
    return 0;
}
