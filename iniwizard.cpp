#include "iniwizard.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <string>
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

const char *iniwizard::manipulator::read(
    const char *string_name
)
{
    return file_content.string_content[string_name].c_str();
}

const char *iniwizard::manipulator::read(
    const char *namespace_name, const char *string_name
)
{
    return file_content.namespace_content[namespace_name][string_name].c_str();
}

bool iniwizard::manipulator::parse(
    const char *file_path
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
    const char *name, const iniwizard::iniwizard_types &type
)
{
    bool output = false;

    if (type == iniwizard::iniwizard_types::iniwizard_string)
        output = file_content.string_content.count(name);
    else if (type == iniwizard::iniwizard_types::iniwizard_namespace)
        output = file_content.namespace_content.count(name);

    return output;
}