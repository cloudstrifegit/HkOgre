#include "XkLog.h"
#include <stdio.h>
#include <stdarg.h>
#include <sstream>
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/LoggingFactory.h>
#include <Poco/Environment.h>
#include <Poco/BinaryWriter.h>
#include <Poco/BinaryReader.h>
#include "XkDebugStr.h"
#include "XkRemoteLogChannel.h"

namespace Xk {

bool XkLog::SetUp(bool bServer)
{
    DBG_BEGIN_END0();

    using Poco::AutoPtr;
    using Poco::Util::PropertyFileConfiguration;
    using Poco::Util::LoggingConfigurator;

    //注册自定义的Channel到Factory,待实现
    if(!bServer){
        Poco::LoggingFactory& fact = Poco::LoggingFactory::defaultFactory();
        fact.registerChannelClass("RemoteLogChannel", new Poco::Instantiator<RemoteLogChannel, Poco::Channel>);
    }

    static const std::string strDefaultClientConfig = 
            "logging.loggers.root.channel = c1\n"
            "logging.loggers.root.level = trace\n"
            "logging.channels.c1.class = RemoteLogChannel\n";

    static const std::string strDefaultServerConfig = 
        "logging.loggers.root.channel = c1\n"
            "logging.loggers.root.level = trace\n"
            "logging.channels.c1.class = ConsoleChannel\n";

    std::string strDefaultConfig;
    if(bServer){
        strDefaultConfig = strDefaultServerConfig;
    }else{
        strDefaultConfig = strDefaultClientConfig;
    }

    AutoPtr<PropertyFileConfiguration> pConfig;

    std::string strFile = Poco::Environment::get("XK_LOG_CONF_FILE", "");
    if(strFile.empty()) {
        std::istringstream istr(strDefaultConfig);
        pConfig = new PropertyFileConfiguration(istr);
        DBGSTRING("XkLog Use default config");
    }else{
        pConfig = new PropertyFileConfiguration(strFile);
        DBGSTRING("XkLog Use XK_LOG_CONF = %s", strFile.c_str());
    }

    try{
        LoggingConfigurator configurator;
        configurator.configure(pConfig);
    }
    catch(Poco::Exception& e)
    {
        XkDebugString("XkLog init failed! Reason: %s", e.displayText().c_str());
        abort();
    }

    return true;
}

bool XkLog::SearialMessage(const Poco::Message& msg, std::string& strOutput)
{
    std::stringstream ss;
    Poco::BinaryWriter bw(ss);

    try{
        bw << msg.getSource();
        bw << msg.getText();
        bw << (Poco::UInt16)msg.getPriority();
        bw << (Poco::UInt64)msg.getTime().utcTime();
        bw << (Poco::UInt32)msg.getTid();
        bw << (Poco::UInt32)msg.getPid();
        const char* szSourceFile = msg.getSourceFile();
        std::string strSourceFile;
        if(szSourceFile != NULL) strSourceFile = szSourceFile;
        bw << strSourceFile;
        bw << (Poco::UInt32)msg.getSourceLine();
        bw << msg.getThread();
    }
    catch(Poco::Exception& e)
    {
        DBGSTRING("XkLog::SerialMessage Exception: %s", e.displayText().c_str());
        return false;
    }

    strOutput = ss.str();
    return true;
}

bool XkLog::UnSerialMessage(std::string& strInput, Poco::Message& msg)
{
    std::stringstream ss(strInput);
    Poco::BinaryReader br(ss);

    try{
        std::string strTmp;
        br >> strTmp;
        msg.setSource(strTmp);

        br >> strTmp;
        msg.setText(strTmp);

        Poco::UInt16 nPriority;
        br >> nPriority;
        msg.setPriority((Poco::Message::Priority)nPriority);

        Poco::UInt64 utcTime;
        br >> utcTime;
        Poco::Timestamp ts = Poco::Timestamp::fromUtcTime(utcTime);
        msg.setTime(ts);

        Poco::UInt32 nTid;
        br >> nTid;
        msg.setTid(nTid);

        Poco::UInt32 nPid;
        br >> nPid;
        msg.setPid(nPid);

        //Poco::Message的szSourceFile，存储的是静态字符串，反序列化之后得到的字符串，用setSourceFile是无效的，
        //所以把sourcefile保存到随带的Map中去
        br >> strTmp;
        msg["sourcefile"] = strTmp;

        Poco::UInt32 nLine;
        br >> nLine;
        msg.setSourceLine(nLine);

        br >> strTmp;
        msg.setThread(strTmp);
    }
    catch(Poco::Exception& e)
    {
        DBGSTRING("XkLog::UnSerialMessage Exception: %s", e.displayText().c_str());
        return false;
    }

    return true;
}

std::string XkLogHelp::GetFormatString(const char* szFormat, ...)
{
    int nListCount = 0;
	va_list pArgList;

	int nMaxLength = 1024;
	char szBuf[1024];
	memset(szBuf,0,1024);

	va_start(pArgList,szFormat);
#ifdef WIN32
    nListCount += _vsnprintf_s(szBuf+nListCount, 1024, nMaxLength-nListCount, szFormat, pArgList);
#else
	nListCount += vsnprintf(szBuf+nListCount,nMaxLength-nListCount,szFormat,pArgList);
#endif
	va_end(pArgList);

	if(nListCount > (nMaxLength-1)) nListCount = nMaxLength - 1;
	*(szBuf+nListCount) = '\0';

    return szBuf;
}

void Xk::XkLogHelp::LogDebug(const std::string& strLabel, const std::string& strMsg, const char* szFile, int nLine)
{
    Poco::Logger::get(strLabel).debug(strMsg, szFile, nLine);
}

void Xk::XkLogHelp::LogWarning(const std::string& strLabel, const std::string& strMsg, const char* szFile, int nLine)
{
    Poco::Logger::get(strLabel).warning(strMsg, szFile, nLine);
}

void Xk::XkLogHelp::LogError(const std::string& strLabel, const std::string& strMsg, const char* szFile, int nLine)
{
    Poco::Logger::get(strLabel).error(strMsg, szFile, nLine);
}

void Xk::XkLogHelp::LogDump(const std::string& strLabel, const std::string& strMsg, const void* pBuffer, size_t nLen)
{
    Poco::Logger::get(strLabel).dump(strMsg, pBuffer, nLen);
}

}//namespace Xk
