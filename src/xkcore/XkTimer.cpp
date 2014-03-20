#include "XkTimer.h"
#include "XkDebugStr.h"
#include "XkLog.h"

namespace Xk {

TimerProfile::TimerProfile(const char* func, const char* file, unsigned int line) : szFunc(func),
    szFile(file), nLine(line)
{
    //XkDebugString("TimerProfile for %s begin", szFunc);
    XkLogHelp::LogDebug("timer", 
        XkLogHelp::GetFormatString("TimerProfile for %s begin", szFunc), file, line);
}

TimerProfile::~TimerProfile()
{
    //XkDebugString("TimerProfile for %s end: %lu ms", szFunc, m_t.Elapsed());
    XkLogHelp::LogDebug("timer", 
        XkLogHelp::GetFormatString("TimerProfile for %s end: %lu ms", szFunc, m_t.Elapsed()), szFile, nLine);
}

}//namespace Xk