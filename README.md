# iniwizard
### Example
main.cpp(With file parsing):
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
                "string_name", iniwizard::iniwizard_variables::iniwizard_string
            ))
            cout << m.read("string_name") << endl;
        if (m.contains(
                "namespace_name",
                iniwizard::iniwizard_containers::iniwizard_namespace
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
main.cpp(Without file parsing):
```cpp
#include "iniwizard.hpp"

#include <iostream>

using namespace std;

int main()
{
    iniwizard::manipulator m;

    m.add("string_name", "string_value");
    m.add("namespace_name");
    m.add("namespace_name", "string_name", "string_value");

    cout << m.read("string_name") << endl
         << m.read("namespace_name", "string_name") << endl;

    return EXIT_SUCCESS;
}
```
### Writing file
```cpp
#include "iniwizard.hpp"

#include <cstdlib>

using namespace std;

int main()
{
    iniwizard::manipulator m;

    m.add("string_name", "string_value");
    m.add("namespace_name", "string_name", "string_value");
    m.add("namespace_name1", "string_name", "string_value");

    m.write("test.ini");

    return EXIT_SUCCESS;
}
```
### Removing
```cpp
#include "iniwizard.hpp"

#include <cstdlib>

using namespace std;

int main()
{
    iniwizard::manipulator m;

    m.add("string_name", "string_value");
    m.add("namespace_name", "string_name", "string_value");
    m.add("namespace_name1", "string_name", "string_value");

    m.remove("string_name", iniwizard::iniwizard_types::iniwizard_string);
    m.remove(
        "namespace_name1", iniwizard::iniwizard_types::iniwizard_namespace
    );

    m.write("test.ini");

    return EXIT_SUCCESS;
}
```
Output:
```ini
[namespace_name]
    string_name = "string_value"
```
Instead of:
```ini
string_name = "string_value"

[namespace_name]
    string_name = "string_value"

[namespace_name1]
    string_name = "string_value"
```
### Compilation
```
g++ main.cpp iniwizard.cpp -std=c++23
```