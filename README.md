# iniwizard
### Example
main.cpp:
```cpp
#include "iniwizard.hpp"

#include <iostream>

using namespace std;

int main()
{
    iniwizard::manipulator m;

    if (m.parse("test.ini"))
    {
        if (m.contains(
                "string_name", iniwizard::iniwizard_types::iniwizard_string
            ))
            cout << m.read("string_name") << endl;
        if (m.contains(
                "namespace_name",
                iniwizard::iniwizard_types::iniwizard_namespace
            )
            && m.contains("namespace_name", "string_name"))
            cout << m.read("namespace_name", "string_name") << endl;
    }

    return EXIT_SUCCESS;
}
```
test.ini:
```ini
string_name = "string_value"

[namespace_name]
string_name = "string_value"
```
### Compilation
```
g++ main.cpp iniwizard.cpp -std=c++23
```