#pragma once

#ifndef INIWIZARD_HPP
#define INIWIZARD_HPP

#include <string>

namespace iniwizard
{
    enum iniwizard_variables
    {
        iniwizard_string
    };

    enum iniwizard_containers
    {
        iniwizard_namespace
    };

    class manipulator
    {
      public:
        manipulator();

        std::string read(std::string string_name);

        std::string read(std::string namespace_name, std::string string_name);

        bool parse(std::string file_path);

        bool contains(std::string name, const iniwizard_variables &type);

        bool contains(std::string name, const iniwizard_containers &type);

        bool contains(std::string namespace_name, std::string string_name);

        void add(std::string string_name, std::string string_value);

        void add(std::string namespace_name);

        void
            add(std::string namespace_name, std::string string_name,
                std::string string_value);

        void remove(std::string name, const iniwizard_variables &type);

        void remove(std::string name, const iniwizard_containers &type);

        void remove(std::string namespace_name, std::string string_name);

        void write(std::string path);
    };
} // namespace iniwizard

#endif // INIWIZARD_HPP