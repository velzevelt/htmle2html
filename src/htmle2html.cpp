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

#define USAGE "Usage: htmle2html [htmle src DIR | FILE] [html out DIR] [include files DIR]"

namespace fs = std::filesystem;

template<typename T>
int exit_on_invalid_arg(T arg)
{
    if (not fs::exists(arg))
    {
        #ifdef WIN32
        std::wcerr << "Invalid input provided, directory \"" << arg << "\" not found" << '\n'
                    << USAGE << '\n';
        #else
        std::cerr << "Invalid input provided, directory \"" << arg << "\" not found" << '\n'
                    << USAGE << '\n';
        #endif
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}   

int exit_on_not_directory(fs::path p)
{
    if (not fs::is_directory(p))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "No enough arguments provided, expected 3 got " << argc - 1 << '\n'
                  << USAGE << '\n';
        return EXIT_FAILURE;
    }

    #ifdef WIN32
    LPWSTR cmd = GetCommandLineW();
    LPWSTR *argvw = CommandLineToArgvW(cmd, &argc);
    std::wstring src_arg(argvw[1]);
    std::wstring out_arg(argvw[2]);
    std::wstring inc_arg(argvw[3]);
    #else
    std::string src_arg(argv[1]);
    std::string out_arg(argv[2]);
    std::string inc_arg(argv[3]);
    #endif


    int failure = exit_on_invalid_arg(src_arg) 
                + exit_on_invalid_arg(out_arg) 
                + exit_on_invalid_arg(inc_arg);
    if (failure != EXIT_SUCCESS)
        return failure;


    fs::path src_path = src_arg;
    fs::path out_path = out_arg;
    fs::path inc_path = inc_arg;
    
    fs::path absolute_src_path = fs::absolute(src_path);
    fs::path absolute_out_path = fs::absolute(out_path);
    fs::path absolute_inc_path = fs::absolute(inc_path);


    failure = exit_on_not_directory(absolute_out_path)
            + exit_on_not_directory(absolute_inc_path);
    if (failure != EXIT_SUCCESS)
        return failure;


    std::vector<fs::path> files;
    std::vector<fs::path> include_files;

    if (fs::is_directory(absolute_src_path))
    {
        for (auto const &dir_entry : fs::recursive_directory_iterator(absolute_src_path))
        {
            fs::path p = dir_entry.path();
            if (p.extension().compare(".htmle") == 0)
                files.push_back(p);
        }
    } 
    else
    {
        files.push_back(absolute_src_path);
    }

    for (auto const &dir_entry : fs::recursive_directory_iterator(absolute_inc_path))
    {
        include_files.push_back(dir_entry.path());
    }


    for (auto const &s : files)
    {
        fs::path new_file_path = absolute_out_path;
        new_file_path /= s.filename().replace_extension(".html");

        // std::cout << "NEW PATH " << new_file_path << "\n";

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
                fs::path arg = line.substr(arg_begin_position, distance);
                arg.make_preferred();


                fs::path closest;
                bool found_closest = false;
                for (auto const &p : include_files)
                {
                    bool is_closest = is_valid_position(p.string().find(arg.string()));
                    if (is_closest)
                    {
                        closest = p;
                        found_closest = true;
                        break;
                    }
                }

                if (not found_closest)
                {
                    std::cerr << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << " not found" << '\n';
                    out_file << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << " not found" << '\n';
                    continue;
                }

                std::ifstream include_file(closest);
                if (not include_file.is_open())
                {
                    std::cerr << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << " cant open" << '\n';
                    out_file << "Error " << s.filename() << " " << line_position << ":" << e_begin_position << " include file " << arg << " cant open" << '\n';
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
