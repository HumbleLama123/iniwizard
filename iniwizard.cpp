#include "iniwizard.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <vector>

inline struct file_content
{
    std::map<std::string, std::string> string_content;
    std::map<std::string, std::map<std::string, std::string>> namespace_content;
} file_content;

inline std::string trim_whitespaces(
    std::string str
)
{
    str.erase(
        remove_if(
            str.begin(), str.end(),
            [](const unsigned char &c) { return std::isspace(c); }
        ),
        str.end()
    );

    return str;
}

inline struct rules
{
    rules()
    {
        init_string_rules();
        init_namespace_rules();
    }

    bool check(
        const std::vector<std::function<bool(const std::string &line)>> &_rules,
        const std::string &line
    )
    {
        for (const auto &i : _rules)
            if (!i(trim_whitespaces(line))) return false;

        return true;
    }

    std::vector<std::function<bool(const std::string &line)>> string_rules,
        namespace_rules;

  private:
    void init_string_rules()
    {
        string_rules = {
            [&](const std::string &line) { return line.contains('='); },
            [&](std::string line) { return line[0] != '='; },
            [&](std::string line) { return line.back() != '='; },
            [&](std::string line)
            {
                for (size_t i = 0; i < line.size(); i++)
                {
                    if (line[i] == '=')
                    {
                        if (line[i + 1] != '"') return false;

                        break;
                    }
                }

                return true;
            }
        };
    }

    void init_namespace_rules()
    {
        namespace_rules = {
            [&](const std::string &line) { return line[0] == '['; },
            [&](const std::string &line) { return line.back() == ']'; }
        };
    }
} rules;

inline std::string get_string_name(
    const std::string &str
)
{
    std::string output;

    bool b = false;

    for (const char &i : str)
    {
        if (!b && !std::isspace(i)) b = true;

        if ((b && std::isspace(i)) || i == '=') break;

        if (b) output += i;
    }

    return output;
}

inline std::string get_string_value(
    const std::string &str
)
{
    std::string output;

    bool bb = false, b = false;

    for (const char &i : str)
    {
        if (i == '=')
        {
            bb = true;
            continue;
        }

        if (bb)
        {
            if (i == '"')
            {
                b = true;
                continue;
            }

            if (b && i == '"') break;

            if (b) output += i;
        }
    }

    return output;
}

inline std::string get_namespace_name(
    const std::string &ns
)
{
    std::string output;

    bool b = false;

    for (const char &i : ns)
    {
        if (i == '[')
        {
            b = true;
            continue;
        } else if (i == ']') break;

        if (b) output += i;
    }

    return output;
}

iniwizard::manipulator::manipulator() {}

std::string iniwizard::manipulator::read(
    std::string string_name
)
{
    return file_content.string_content[string_name].c_str();
}

std::string iniwizard::manipulator::read(
    std::string namespace_name, std::string string_name
)
{
    return file_content.namespace_content[namespace_name][string_name].c_str();
}

bool iniwizard::manipulator::parse(
    std::string file_path
)
{
    if (std::filesystem::exists(file_path))
    {
        std::ifstream i(file_path);

        std::string temp;

        bool b = false;

        while (std::getline(i, temp))
        {
            if (rules.check(rules.string_rules, temp))
            {
                if (!b)
                    file_content.string_content.insert(std::make_pair(
                        get_string_name(temp), get_string_value(temp)
                    ));
                else
                    file_content.namespace_content.rbegin()->second.insert(
                        std::make_pair(
                            get_string_name(temp), get_string_value(temp)
                        )
                    );
            } else if (rules.check(rules.namespace_rules, temp)) {
                if (!b) b = true;

                file_content.namespace_content.insert(make_pair(
                    get_namespace_name(temp),
                    std::map<std::string, std::string>()
                ));

                continue;
            }
        }
    } else return false;

    return true;
}

bool iniwizard::manipulator::contains(
    std::string name, const iniwizard_variables &type
)
{
    bool output = false;

    if (type == iniwizard_variables::iniwizard_string)
        output = file_content.string_content.count(name);

    return output;
}

bool iniwizard::manipulator::contains(
    std::string name, const iniwizard_containers &type
)
{
    bool output = false;

    if (type == iniwizard_containers::iniwizard_namespace)
        output = file_content.string_content.count(name);

    return output;
}

bool iniwizard::manipulator::contains(
    std::string namespace_name, std::string string_name
)
{
    return file_content.namespace_content.count(namespace_name)
        && file_content.namespace_content[namespace_name].count(string_name);
}

void iniwizard::manipulator::add(
    std::string string_name, std::string string_value
)
{
    file_content.string_content.insert(std::make_pair(string_name, string_value)
    );
}

void iniwizard::manipulator::add(
    std::string namespace_name
)
{
    file_content.namespace_content.insert(
        make_pair(namespace_name, std::map<std::string, std::string>())
    );
}

void iniwizard::manipulator::add(
    std::string namespace_name, std::string string_name,
    std::string string_value
)
{
    if (!contains(namespace_name, iniwizard_containers::iniwizard_namespace))
        add(namespace_name);

    file_content.namespace_content[namespace_name].insert(
        std::make_pair(string_name, string_value)
    );
}

void iniwizard::manipulator::remove(
    std::string name, const iniwizard_variables &type
)
{
    if (type == iniwizard_variables::iniwizard_string)
        file_content.string_content.erase(name);
}

void iniwizard::manipulator::remove(
    std::string name, const iniwizard_containers &type
)
{
    if (type == iniwizard_containers::iniwizard_namespace)
        file_content.string_content.erase(name);
}

void iniwizard::manipulator::remove(
    std::string namespace_name, std::string string_name
)
{
    file_content.namespace_content[namespace_name].erase(string_name);
}

void iniwizard::manipulator::write(
    std::string path
)
{
    std::ofstream o(path);

    for (const auto &i : file_content.string_content)
        o << i.first + " = \"" + i.second + "\"\n";

    if (!file_content.string_content.empty()) o << '\n';

    for (const auto &i : file_content.namespace_content)
    {
        o << '[' + i.first + "]\n";

        for (const auto &j : i.second)
            o << "    " << j.first + " = \"" + j.second + "\"\n";

        o << '\n';
    }
}