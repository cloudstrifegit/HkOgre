#include "XkRemoteLogChannel.h"
#include "XkDebugStr.h"
#include <Poco/Message.h>
#include <Poco/Timestamp.h>
#include <Poco/Environment.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include "XkClient.h"
#include "XkGC.h"
#include "XkLog.h"

namespace Xk {

RemoteLogChannel::RemoteLogChannel() : m_pClient(NULL)
{
    DBG_BEGIN_END0();
    m_thread.setName(XK_REMOTELOG_CHANNEL_THREAD);
    m_pClient = new Client(this);
    m_pClient->SetName(XK_REMOTELOG_CHANNEL_CLIENT);
    std::string strLogServer = Poco::Environment::get("XK_LOG_CONF_SERVER", "localhost:8888:123");

    std::string strServer, strPasswd;
    int nPort;

    try{
        Poco::StringTokenizer st(strLogServer, ":", 
            Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY
            );
        strServer = st[0];
        nPort = Poco::NumberParser::parse(st[1]);
        strPasswd = st[2];
    }
    catch(Poco::Exception& e)
    {
        DBGSTRING("RemoteLogChannel parse server info failed! exception: %s, use default config", 
            e.displayText().c_str());

        strServer = "localhost";
        nPort = 8888;
        strPasswd = "123";
    }

    XkDebugString("RemoteLogChannel server config: %s:%d:%s", strServer.c_str(),
        nPort,
        strPasswd.c_str());

    if(m_pClient->Connect(strServer.c_str(), strPasswd.c_str(), nPort)) {
        DBGSTRING("RemoteLogChannel Connect to log server successful!");
        m_pClient->Start();
    }else{
        DBGSTRING("RemoteLogChannel Connect to log server failed!");
    }
}

RemoteLogChannel::~RemoteLogChannel()
{
    DBG_BEGIN_END0();
    delete m_pClient;
}

void RemoteLogChannel::log(const Poco::Message& msg)
{
    std::string strOutput;
    if(Xk::XkLog::SearialMessage(msg, strOutput)) {
        m_pClient->Send(Xk::MSGID_INTER_LOG, strOutput.data(), strOutput.length());
    }
}

void RemoteLogChannel::OnNewConnection(XK_GUID guid)
{

}

void RemoteLogChannel::OnDisconnect(XK_GUID guid)
{
    DBG_BEGIN_END0();

    m_thread.start(*this);
}

void RemoteLogChannel::OnLostConnection(XK_GUID guid)
{
    DBG_BEGIN_END0();
    m_thread.start(*this);
}

void RemoteLogChannel::OnMsg(XK_GUID guid, int nID, const void* pBuf, int nLength)
{

}

void RemoteLogChannel::run()
{
    DBG_BEGIN_END0();

    Poco::Thread::sleep(100);
    DBGSTRING("RemoteLogChannel::run reconnecting...");
    int nTryCount = 0;
    while(1) {
        if(m_pClient->Connect()) {
            DBGSTRING("RemoteLogChannel::run reconnect successful! Try count: %d", nTryCount);
            m_pClient->Start();
            break;
        }
        DBGSTRING("RemoteLogChannel::run reconnect failed! later to retry. Try count: %d", nTryCount);
        Poco::Thread::sleep(4000);
        nTryCount++;
    }
}

}//namespace Xk