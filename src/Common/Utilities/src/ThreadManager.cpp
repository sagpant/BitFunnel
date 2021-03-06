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


#include <stddef.h>

#include "BitFunnel/Utilities/Factories.h"
#include "ThreadManager.h"


namespace BitFunnel
{
    std::unique_ptr<IThreadManager>
        Factories::CreateThreadManager(const std::vector<std::unique_ptr<IThreadBase>>& threads)
    {
        return std::unique_ptr<IThreadManager>(new ThreadManager(threads));
    }


    ThreadManager::ThreadManager(const std::vector<std::unique_ptr<IThreadBase>>& threads)
    {
        for (size_t i = 0 ; i < threads.size(); ++i)
        {
            m_threads.push_back(std::thread(ThreadEntryPoint, threads[i].get()));
        }
    }


    ThreadManager::~ThreadManager()
    {
        // REVIEW: what should this do if threads are still running?
    }


    void ThreadManager::WaitForThreads()
    {
        for (auto& thread : m_threads)
        {
            thread.join();
        }
    }


    void ThreadManager::ThreadEntryPoint(void* data)
    {
        IThreadBase* thread = static_cast<IThreadBase*>(data);
        thread->EntryPoint();
    }
}
