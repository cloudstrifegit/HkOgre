#include "XkGC.h"
#include "XkClient.h"
#include "XkDebugStr.h"
#include <map>
#include <Poco/Mutex.h>

namespace GC {

#if XK_DEBUG_MEMORY == 1

typedef struct{
    void* ptr;
    unsigned int nSize;
    std::string strType;
    std::string strFile;
    unsigned int nLine;
}MEMORY_RECORD;

class MemoryTracker
{
public:
    typedef std::map<void*, MEMORY_RECORD> MAP_MEMORY;
public:
    MemoryTracker();
    ~MemoryTracker();

    void Push(void* ptr, unsigned int size, const char* type, const char* file, unsigned int line);
    void Pop(void* ptr, const char* file, unsigned int line);

    void PrintLeak();

private:
    MAP_MEMORY m_mapMem;
    Poco::Mutex m_mutex;
};

MemoryTracker::MemoryTracker()
{
    DBG_BEGIN_END0();
}

MemoryTracker::~MemoryTracker()
{
    DBG_BEGIN_END0();
    PrintLeak();
}

void MemoryTracker::Push(void* ptr, unsigned int size, const char* type, const char* file, unsigned int line)
{
    DBG_BEGIN_END0();
    Poco::ScopedLock<Poco::Mutex> lock(m_mutex);

    MEMORY_RECORD memRec = { ptr, size, type, file, line };
    m_mapMem.insert(MAP_MEMORY::value_type(ptr, memRec));
}

void MemoryTracker::Pop(void* ptr, const char* file, unsigned int line)
{
    DBG_BEGIN_END0();
    Poco::ScopedLock<Poco::Mutex> lock(m_mutex);

    MAP_MEMORY::iterator it = m_mapMem.find(ptr);
    if(it != m_mapMem.end())
        m_mapMem.erase(it);
}

void MemoryTracker::PrintLeak()
{
    DBG_BEGIN_END0();

    for(MAP_MEMORY::iterator it = m_mapMem.begin();
        it != m_mapMem.end();
        it ++)
    {
        XkDebugString("ptr: 0x%X, %s[%d]", it->second.ptr, it->second.strFile.c_str(), it->second.nLine);
    }
}

namespace{
    static MemoryTracker tracker;
}

void gc_debugger_push_ptr(void* ptr, unsigned int size, const char* type, const char* file, unsigned int line)
{
    tracker.Push(ptr, size, type, file, line);
}

void gc_debugger_pop_ptr(void* ptr, const char* file, unsigned int line)
{
    tracker.Pop(ptr, file, line);
}

#endif//#if XK_DEBUG_MEMORY == 1

}//namespace GC