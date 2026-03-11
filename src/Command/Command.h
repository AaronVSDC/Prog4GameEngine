#ifndef COMMAND_H
#define COMMAND_H
#include <glm/glm.hpp>

namespace dae
{
    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void execute() = 0;
    };
    class AnalogCommand
    {
    public:
        virtual ~AnalogCommand() = default;
        virtual void execute(const glm::vec2& value) = 0;
    };
}

#endif