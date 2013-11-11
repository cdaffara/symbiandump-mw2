


// phoneclientengine
#include "cphsrvresourcemanager.h"
#include "cphsrvussdmanager.h"
#include <cphcltussd.h> // USSD API

// qt stuff
#include <QtTest/QtTest>

// unit test
#include "ut_testenv.h"

// globals
TestEnv* TestEnv::m_singleton = 0;

// ====================================================================
// EtelMsgLoop
// ====================================================================
EtelMsgLoop::EtelMsgLoop()
{
    mSendMessageReqStatus = 0;
    
    mReceiveMessageReqStatus = 0;
    mReceiveMessageData = 0;
    mReceiveMessageAttributes = 0;
    
    mNetworkReleaseReqStatus = 0;
    mNetworkReleaseMessageData = 0;
    mNetworkReleaseMessageAttributes = 0;
}

void EtelMsgLoop::receive( const TDesC8& msg, const TDesC8& attr )
{
    qDebug("EtelMsgLoop::receive mReceiveMessageReqStatus in =%d",
        mReceiveMessageReqStatus);
    mReceiveMessageData->Copy( msg );
    mReceiveMessageAttributes->Copy( attr );
    const TestMode& mode = TestEnv::env().testMode();
    User::RequestComplete(mReceiveMessageReqStatus, mode.m_error);
    qDebug("EtelMsgLoop::receive mReceiveMessageReqStatus out =%d",
        mReceiveMessageReqStatus);
    QVERIFY(mReceiveMessageReqStatus == 0);
}

void EtelMsgLoop::networkRelease( const TDesC8& msg, const TDesC8& attr )
{
    qDebug("EtelMsgLoop::networkRelease mNetworkReleaseReqStatus in =%d", 
        mNetworkReleaseReqStatus);
    mNetworkReleaseMessageData->Copy( msg );
    if ( mNetworkReleaseMessageAttributes )
        mNetworkReleaseMessageAttributes->Copy( attr );
        const TestMode& mode = TestEnv::env().testMode();
        User::RequestComplete(mNetworkReleaseReqStatus, mode.m_error);
    qDebug("EtelMsgLoop::networkRelease mNetworkReleaseReqStatus out =%d", 
        mNetworkReleaseReqStatus);
    QVERIFY(mNetworkReleaseReqStatus == 0);
}

// ====================================================================
// TestEnv
// ====================================================================

void TestEnv::create()
{
    QVERIFY(!m_singleton);
    m_singleton = new TestEnv();
    m_singleton->construct();
}

void TestEnv::destroy()
{
    QVERIFY(m_singleton);
    delete m_singleton;
    m_singleton = 0;
}

TestEnv& TestEnv::env()
{
    return *m_singleton;
}

TestEnv::TestEnv()
{
}

void TestEnv::construct()
{
    int leave;
    // create the main USSD resource manager class
    m_resourceManager = new CPhSrvResourceManager(m_fs);
    TRAP(leave, m_resourceManager->ConstructL());
    QVERIFY(leave == 0);
    m_ussdManager = new CPhSrvUssdManager(m_fs,*m_resourceManager);
    TRAP(leave,m_ussdManager->ConstructL(*this));
    QVERIFY(leave == 0);
    m_fakeMessage = new RMessage2();
}

TestEnv::~TestEnv()
{
    delete m_ussdManager;
    m_ussdManager = 0;
    delete m_resourceManager;
    m_resourceManager = 0;
    delete m_fakeMessage;
    m_fakeMessage = 0;
}


#define NULLREF(clz) *reinterpret_cast<clz*>((void*)0)

RTelServer& TestEnv::PhSrvETelServer()
    { return NULLREF(RTelServer); }

RMobilePhone& TestEnv::PhSrvMobilePhone()
    { return NULLREF(RMobilePhone); }

RMmCustomAPI& TestEnv::PhSrvCustomPhone()
    { return NULLREF(RMmCustomAPI); }




//End of file

