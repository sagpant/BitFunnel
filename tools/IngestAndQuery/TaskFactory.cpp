// The MIT License (MIT)

// Copyright (c) 2016, Microsoft

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "BitFunnel/Exceptions.h"
#include "BitFunnel/Utilities/Factories.h"
#include "TaskBase.h"
#include "TaskFactory.h"


namespace BitFunnel
{
    //*************************************************************************
    //
    // TaskFactory
    //
    //*************************************************************************

    TaskFactory::TaskFactory(Environment & environment)
      : m_environment(environment),
        m_nextId(0),
        m_maxNameLength(0)
    {
    }


    void TaskFactory::Register(std::unique_ptr<ITask::Descriptor> descriptor)
    {
        std::string s(descriptor->GetName());
        m_maxNameLength = std::max(m_maxNameLength, s.size());
        auto it = m_taskMap.find(s);
        if (it != m_taskMap.end())
        {
            RecoverableError error("TaskFactory::Register: duplicate task name.");
            throw error;
        }

        m_taskMap.insert(std::make_pair(s, std::move(descriptor)));
    }


    std::unique_ptr<ITask> TaskFactory::CreateTask(char const * line)
    {
        auto tokens = Tokenize(line);

        if (tokens.size() == 0)
        {
            RecoverableError error("Expected a command.");
            throw error;
        }
        std::string const & name = tokens[0];

        auto it = m_taskMap.find(name);
        if (it == m_taskMap.end())
        {
            std::stringstream s;
            s << "Unknown command \"" << name << "\"";
            RecoverableError error(s.str().c_str());
            throw error;
        }

        return (*it).second->Create(m_environment, m_nextId++, tokens);
    }


    static void SkipWhite(char const * & text)
    {
        while (isspace(*text))
        {
            ++text;
            if (*text == '\0')
            {
                break;
            }
        }
    }


    static void Consume(char const * & text, char expected)
    {
        if (*text == expected)
        {
            ++text;
        }
        else
        {
            RecoverableError error("Syntax error.");
            throw error;
        }
    }


    ITask::Id TaskFactory::GetNextTaskId() const
    {
        return m_nextId;
    }


    std::vector<std::string> TaskFactory::Tokenize(char const * text)
    {
        std::vector<std::string> tokens;

        while (*text != '\0')
        {
            SkipWhite(text);

            if (*text == '"')
            {
                // Quoted string literal
                std::string s;
                ++text;
                while (*text != '"')
                {
                    if (*text == '\0')
                    {
                        RecoverableError error("Expected closing \" in string literal.");
                        throw error;
                    }
                    s.push_back(*text);
                    ++text;
                }
                Consume(text, '"');
                tokens.push_back(s);
            }
            else if (*text != '\0')
            {
                // Non-quoted literal.
                std::string s;
                while (!isspace(*text) && *text != '\0')
                {
                    s.push_back(*text);
                    ++text;
                }
                tokens.push_back(s);
            }
        }

        return tokens;
    }


    void TaskFactory::Help(std::ostream & output,
                           char const * command) const
    {
        if (command == nullptr)
        {
            output << "Available commands:" << std::endl;
            for (auto & entry : m_taskMap)
            {
                ITask::Descriptor const & descriptor = *entry.second;
                output
                    << "  "
                    << std::setw(m_maxNameLength) << std::left
                    << descriptor.GetName()
                    << "  "
                    << descriptor.GetOneLineDescription()
                    << std::endl;
            }

            output
                << std::endl
                << "Type help <command> for more information on a particular command."
                << std::endl;
        }
        else
        {
            auto it = m_taskMap.find(command);
            if (it != m_taskMap.end())
            {
                ITask::Descriptor const & descriptor = *(*it).second;

                output
                    << descriptor.GetVerboseDescription()
                    << std::endl;
            }
            else
            {
                output
                    << "Unknown command "
                    << command
                    << std::endl;
            }
        }
    }


    //*************************************************************************
    //
    // TaskBase
    //
    //*************************************************************************
    TaskBase::TaskBase(Environment & environment, Id id, Type type)
        : m_environment(environment),
          m_id(id),
          m_type(type)
    {
    }


    ITask::Type TaskBase::GetType() const
    {
        return m_type;
    }


    ITask::Id TaskBase::GetId() const
    {
        return m_id;
    }


    Environment & TaskBase::GetEnvironment() const
    {
        return m_environment;
    }
}