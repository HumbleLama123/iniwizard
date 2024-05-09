#pragma once

#ifndef INIWIZARD_HPP
#define INIWIZARD_HPP

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

        const char *read(const char *string_name);

        const char *read(const char *namespace_name, const char *string_name);

        bool parse(const char *file_path);

        bool contains(const char *name, const iniwizard_variables &type);

        bool contains(const char *name, const iniwizard_containers &type);

        bool contains(const char *namespace_name, const char *string_name);

        void add(const char *string_name, const char *string_value);

        void add(const char *namespace_name);

        void
            add(const char *namespace_name, const char *string_name,
                const char *string_value);

        void remove(const char *name, const iniwizard_variables &type);

        void remove(const char *name, const iniwizard_containers &type);

        void remove(const char *namespace_name, const char *string_name);

        void write(const char *path);
    };
} // namespace iniwizard

#endif // INIWIZARD_HPP