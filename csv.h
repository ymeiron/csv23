#ifndef CSV_READER_H
#define CSV_READER_H

#include <algorithm>
#include <charconv>
#include <fstream>
#include <string>
#include <string_view>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace csv {

namespace detail {
    void cast(std::string_view s, std::integral auto& output) {
        auto result = std::from_chars(begin(s), end(s), output);
        if (result.ptr != end(s))
            throw std::runtime_error("Error casting to an integral type.");
    };

    void cast(std::string_view s, double& output) {
        auto result = std::from_chars(begin(s), end(s), output);
        if (result.ptr != end(s))
            throw std::runtime_error("Error casting to double.");
    };

    static void cast(std::string_view s, std::string& output) {
        output = s;
    };

    template <typename... Args>
    void string_to_tuple(const std::string_view input, std::tuple<Args...>& output, const std::string_view delim=",")
    {
        using std::operator""sv;
        auto split_view = input | std::views::split(delim);
        std::vector<std::string_view> items {begin(split_view), end(split_view)};
        std::ranges::for_each(items, [](std::string_view& s){
            s.remove_prefix(std::min(s.find_first_not_of(" "sv), s.size()));
            s.remove_suffix(s.size()-s.find_last_not_of(" "sv)-1);
        });
        auto lambda = [&items](Args&... args){
            size_t i {};
            ((cast(items[i++], args)  ), ...);
        };
        std::apply(lambda, output);
    }
}

template <typename... Args>
std::vector<std::tuple<Args...>> parse(std::istream& stream, unsigned int skip = 0, const std::string_view delim=",")
{
    std::vector<std::tuple<Args...>> data;
    std::string line;
    size_t line_num {};
    while (std::getline(stream, line)) {
        if (line_num++ < skip) continue;
        std::tuple<Args...> row;
        detail::string_to_tuple(line, row, delim);
        data.emplace_back(row);
    }
    return data;
}

template <typename... Args>
std::vector<std::tuple<Args...>> parse(const std::string& filename, unsigned int skip = 0, const std::string_view delim=",")
{
    std::ifstream file {filename};
    if (!file.is_open())
        throw std::runtime_error("Could not open the file.");
    return parse<Args...>(file, skip, delim);
}

template<std::size_t N> inline constexpr auto column_view = std::views::elements<N>;
}

#endif // CSV_READER_H
