# stevensMapLib
A single-header C++ library for expanding std::map and std::unordered_map functionality

## Requirements
- C++20 or later (uses `std::string::starts_with()`)
- No external dependencies!

## Install
Simply copy `stevensMapLib.hpp` into your project and include it:

```cpp
#include "stevensMapLib.hpp"
```

Or use the Beldum Package Manager: https://github.com/Nord-Tech-Systems-LLC/beldum_package_manager
```bash
mkdir new_project
cd new_project
beldum init
beldum install stevensMapLib
```

## Building Tests
```bash
cd tests
mkdir build && cd build
cmake ..
cmake --build .
ctest
```

## Building Benchmarks
```bash
cd benchmarks
mkdir build && cd build
cmake ..
cmake --build .
./maplib_benchmarks
```

## Usage
`src/main.cpp`
```cpp
#include "stevensMapLib.hpp"

#include <map>
#include <string>
#include <iostream>

int main()
{
    // Define a map of string keys and double values
    std::map<std::string, double> prices = {
        {"apple", 1.0},
        {"banana", 0.5},
        {"cherry", 2.0}
    };

    // Factor to multiply each price by
    long double factor = 1.1; // For example, a 10% price increase

    // Call the function to multiply each value in the map
    prices = stevensMapLib::multiplyWithValues(prices, factor);

    // Output the updated map
    for (const auto& [key, value] : prices) {
        std::cout << key << ": " << value << std::endl;
    }

    return 0;
}

```