#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#ifdef WIN32
#include <windows.h>
#include <processenv.h>
#include <shellapi.h>
#endif



namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "No input directory provided" << '\n'
                  << "Usage: htmle2html [directory]" << '\n';
        return EXIT_FAILURE;
    }


    #ifdef WIN32
    LPWSTR cmd = GetCommandLineW();
    LPWSTR *argvw = CommandLineToArgvW(cmd, &argc);
    std::wstring zero_arg(argvw[1]);
    #else
    std::string zero_arg = argv[1];
    #endif


    fs::path arg_path = zero_arg;
    fs::path arg_parent = arg_path.parent_path();
    fs::path absolute_arg_parent = fs::absolute(arg_parent);

    std::vector<fs::path> files;
    for (auto const& dir_entry : fs::recursive_directory_iterator(absolute_arg_parent))
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
        fs::path new_file_path = s;
        new_file_path.replace_extension(".html");

        std::cout << "Compiling file " << s.filename() 
                  << " to " << new_file_path.filename() 
                  << '\n';
        
        std::ifstream source_file(s);
        if (!source_file.is_open())
        {
            std::cerr << "Cant open file " << s.filename() << '\n';
            continue;
        }

        std::ofstream out_file(new_file_path);
        if (!out_file.is_open())
        {
            std::cerr << "Cant open file " << new_file_path.filename() << '\n';
            source_file.close();
            continue;
        }

        std::string line;
        while (std::getline(source_file, line))
        {
            std::string out = line;

            auto is_valid_position = [](std::size_t pos) {return pos != std::string::npos;};

            std::size_t comment_position = line.find("<!--");
            if (not is_valid_position(comment_position))
                comment_position = line.find("-->");

            std::size_t e_begin_position = line.find("<?e");
            std::size_t e_end_position = line.find("?>");
            


            out_file << out << '\n';
        }

        out_file.close();
        source_file.close();
    }


    return EXIT_SUCCESS;
}

