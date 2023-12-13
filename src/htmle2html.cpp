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


    if (not fs::exists(zero_arg))
    {
        #ifdef WIN32
        std::wcerr << "Invalid input provided, directory \"" << zero_arg << "\" not found" << '\n'
                   << "Usage: htmle2html [directory]" << '\n';
        #else
        std::cerr << "Invalid input provided, directory \"" << zero_arg << "\" not found" << '\n'
                  << "Usage: htmle2html [directory]" << '\n';
        #endif

        return EXIT_FAILURE;
    }


    fs::path arg_path = zero_arg;
    if (not fs::is_directory(arg_path))
    {
        std::cerr << "Invalid input provided, file \"" << arg_path.string() << "\" is not dir" << '\n'
                  << "Usage: htmle2html [directory]" << '\n';

        return EXIT_FAILURE;
    }

    fs::path absolute_arg_parent = fs::absolute(arg_path);


    std::vector<fs::path> files;
    for (auto const &dir_entry : fs::recursive_directory_iterator(absolute_arg_parent))
    {
        files.push_back(dir_entry.path());
    }

    // for (auto const& s : files)
    // {
    //     std::cout << s << '\n';
    // }

    std::vector<fs::path> htmle_files;
    std::copy_if(files.begin(), files.end(), std::back_inserter(htmle_files), [](fs::path i)
                 { return i.extension().compare(".htmle") == 0; });

    for (auto const &s : htmle_files)
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

        size_t line_position = 0;
        std::string line;
        while (std::getline(source_file, line))
        {
            line_position++;
            std::string out = line;

            auto is_valid_position = [](std::size_t pos)
            { return pos != std::string::npos; };

            std::size_t comment_position = line.find("<!--");
            comment_position = not is_valid_position(comment_position) ? line.find("-->") : comment_position;

            bool has_comment = is_valid_position(comment_position);
            if (has_comment)
            {
                out_file << out << '\n';
                continue;
            }

            std::size_t e_begin_position = line.find("<?e");
            std::size_t e_end_position = line.find("?>");

            bool has_begin = is_valid_position(e_begin_position);
            bool has_end = is_valid_position(e_end_position);
            if (has_begin and not has_end)
            {
                std::cerr << "Syntax Error " << s.filename() << " " << line_position << ":" << e_begin_position << " missing ?>" << '\n';
                out_file << "Syntax Error " << line_position << ":" << e_begin_position << " missing ?>" << '\n';
                continue;
            }

            std::size_t include_position = has_begin ? line.find("include", e_begin_position) : std::string::npos;
            bool has_begin_and_include = has_begin and is_valid_position(include_position);

            if (has_begin_and_include)
            {
                std::size_t arg_begin_position = line.find("(\"", include_position);
                std::size_t arg_end_position = line.find("\")", include_position);
                bool has_arg = is_valid_position(arg_begin_position) and is_valid_position(arg_end_position);

                if (not has_arg)
                {
                    std::cerr << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include(\"\") missing argument" << '\n';
                    out_file << "Error " << line_position << ":" << e_begin_position << " include(\"\") missing argument" << '\n';
                    continue;
                }

                arg_begin_position += 2;
                arg_end_position--;

                int distance = arg_end_position - arg_begin_position + 1;
                std::string arg = line.substr(arg_begin_position, distance);

                fs::path closest;
                bool found_closest = false;
                for (auto const &p : files)
                {
                    bool is_closest = p.filename().compare(arg) == 0;
                    if (is_closest)
                    {
                        closest = p;
                        found_closest = true;
                        break;
                    }
                }

                if (not found_closest)
                {
                    std::cerr << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << "not found" << '\n';
                    out_file << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << "not found" << '\n';
                    continue;
                }
                

                std::ifstream include_file(closest);
                if (not include_file.is_open())
                {
                    std::cerr << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << "cant open" << '\n';
                    out_file << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << "cant open" << '\n';
                    continue;
                }

                std::string l;
                while (std::getline(include_file, l))
                {
                    out_file << l << '\n';
                }
                include_file.close();
                continue;
            }

            out_file << out << '\n';
        }

        out_file.close();
        source_file.close();
    }

    return EXIT_SUCCESS;
}
