A very simple header-only CSV reader for C++23.

It exposes just one function template, `csv::parse`. Its template arguments are the column types. The return type is an `std::vector` of `std::tuple`s, with the tuple element types matching the calling template arguments.

The first argument of the function is a file name string *or* an input stream. The second (optional, defaults to `0`) is the number of rows to skip (e.g. if there is a header, use `1`). The third (optional, defaults to `","`) is the delimiter.

Additionally for conveneinece, `csv::column_view` is an alias of `std::views::elements` and can be used with the pipe operator.

Example
-------

This example code reads the data from `iris.csv`, prints out the different species names in the dataset and the average sepal length (second column) for each. The program should make use of `std::ranges::to` and `std::println` but they are not available in GCC 13 yet.

```cpp
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <string>

#include "csv.h"

namespace stdv = std::views;

int main()
{
    auto data {csv::parse<std::string,double,double,double,double>("data/iris.csv")};
    auto species {data | csv::column_view<0>};
    std::set<std::string> species_unique {begin(species), end(species)};
    for (const auto& s : species_unique) {
        auto sepal_length {data
            | stdv::filter([&s](const auto& row){ return std::get<0>(row) == s; })
            | stdv::elements<1>};
        const auto count {std::ranges::distance(sepal_length)};
        const auto sum {std::reduce(begin(sepal_length), end(sepal_length))};
        const double avg = sum/count;
        std::cout << s << " (" << avg << " cm)\n";
    }
}
```

Limitations
-----------

The function works by splitting each row into its components using `std::views::split`, it makes no checks whether the delimiter is escaped or not, or whether a string value is in quotations.