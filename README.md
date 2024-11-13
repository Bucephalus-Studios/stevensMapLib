# stevensMapLib
A simple C++ map library

# Simple installation and usage:
You can use this map library with the Beldum Package Manager:

# Dependencies
- stevensStringLib

## Install
Import "stevensMapLib.hpp" into your own project

or

### Beldum Package Manager: https://github.com/Nord-Tech-Systems-LLC/beldum_package_manager
```
mkdir new_project
cd new_project
beldum init
beldum install stevensMapLib
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