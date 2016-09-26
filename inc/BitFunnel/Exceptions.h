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

#pragma once

#include <stdexcept>    // Inherits from std::runtime_error.
#include <string>       // std::string parameter.

// TODO: Add logging method.

#ifdef __clang__
// TODO: see https://github.com/BitFunnel/BitFunnel/issues/208
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif


namespace BitFunnel
{
    class RecoverableError : public std::runtime_error
    {
    public:
        RecoverableError(char const * msg);
        RecoverableError(std::string const & msg);
        RecoverableError();
    };


    class FatalError : public std::runtime_error
    {
    public:
        FatalError(char const * msg);
        FatalError(std::string const & msg);
        FatalError();
    };


    class NotImplemented : public FatalError
    {
    public:
        NotImplemented(char const * msg);
        NotImplemented(std::string const & msg);
        NotImplemented();
    };
}


#ifdef __clang__
#pragma clang diagnostic pop
#endif
