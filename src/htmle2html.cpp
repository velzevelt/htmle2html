#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

void compile_rec(int argc, char **argv)
{
    fs::path binary_path = argv[0];
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

}



int main(int argc, char **argv)
{
    compile_rec(argc, argv);
    return 0;
}