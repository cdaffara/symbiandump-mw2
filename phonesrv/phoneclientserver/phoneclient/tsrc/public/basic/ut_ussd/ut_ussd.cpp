

#include <e32base.h>
#include <exterror.h>
#include <gsmerror.h>

// phoneclientengine
#include <cphcltussd.h> // USSD API
#include "cphcltussdimp.h" // USSD INTERNAL API
#include <cphcltussdsatclient.h> // USSD SAT API
#include <etelsat.h>

// qt stuff
#include <QObject>
#include <QtTest/QtTest>
#include <QMetaType>

// unit test
#include "ut_testenv.h"
#include "ut_ussd.h"

// want to use TestMode class inside QVariant (using QFETCH)
Q_DECLARE_METATYPE(TestMode)


// ====================================================================
// Test cases
// ====================================================================


void Ut_Ussd::testTestEnv()
{
    TestEnv::create();
    QTest::qSleep(200);

    TestEnv* env = &TestEnv::env();
    QVERIFY2(env,"TestEnv null");
    
    CPhSrvUssdManager* m = &TestEnv::env().ussdManager();
    QVERIFY2(m,"TestEnv::ussdManager null");
    
    TestMode tm1(TestMode::NoResp,KErrUnknown,123);
    TestEnv::env().setTestMode(tm1);
    QCOMPARE( (int)TestEnv::env().testMode().m_event, (int)TestMode::NoResp);
    QCOMPARE( TestEnv::env().testMode().m_error, KErrUnknown);
    QCOMPARE( TestEnv::env().testMode().m_timeout, 123);
    TestMode tm2(KErrNotSupported,456);
    TestEnv::env().setTestMode(tm2);
    QCOMPARE( (int)TestEnv::env().testMode().m_event, (int)TestMode::Normal);
    QCOMPARE( TestEnv::env().testMode().m_error, KErrNotSupported);
    QCOMPARE( TestEnv::env().testMode().m_timeout, 456);
    TestMode tm3(TestMode::Normal);
    TestEnv::env().setTestMode(tm3);
    QCOMPARE( (int)TestEnv::env().testMode().m_event, (int)TestMode::Normal);
    QCOMPARE( TestEnv::env().testMode().m_error, KErrNone);
    QCOMPARE( TestEnv::env().testMode().m_timeout, (int)TestMode::Immediate);
    
    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::openAndCloseApi()
{
    TestEnv::create();
    QTest::qSleep(200);
    
    createUssdApi(false);
    
    delete m_ussdApi;
    m_ussdApi = 0;

    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::sendUssd16bit_data()
{
    QTest::addColumn<bool>("ui");
    QTest::addColumn<QString>("message");
    QTest::addColumn<TestMode>("testmode");

    // without ui
    QTest::newRow("1.1") << false << "123" << TestMode(KErrNone);
    QTest::newRow("1.2") << false << "***" << TestMode(KErrNone,3000);
    QTest::newRow("1.3") << false << "qwerty" << TestMode(KErrUnknown);
    // show ui
    QTest::newRow("2.1") << true << "#*555*" << TestMode(KErrNone,20);
    QTest::newRow("2.2") << true << "#*555*" << TestMode(KErrCancel,500);
    QTest::newRow("2.3") << true << "#*555*" << TestMode(KErrInUse,1000);
    QTest::newRow("2.4") << true << "#*555*" << TestMode(KErrAccessDenied,90);
    QTest::newRow("2.5") << true << "#*555*" << TestMode(KErrGsmSSCallBarred,190);
    QTest::newRow("2.6") << true << "#*555*" << TestMode(KErrGsmSMSNoNetworkService,700);
    QTest::newRow("2.7") << true << "#*555*" << TestMode(KErrGsmOfflineOpNotAllowed,1200);
    QTest::newRow("2.8") << true << "#*555*" << TestMode(KErrSatControl,200);
    QTest::newRow("2.9") << true << "#*555*" << TestMode(KErrUnknown,920);

}

void Ut_Ussd::sendUssd16bit()
{
    TestEnv::create();
    QTest::qSleep(200);

    QFETCH(bool, ui);
    QFETCH(QString, message);
    QFETCH(TestMode, testmode);
    TestEnv::env().setTestMode(testmode);

    // create api
    createUssdApi(ui);

    // send USSD
    TBuf<100> buf16;
    buf16.Copy( (const TUint16*)message.constData(), message.length() );
    int result = m_ussdApi->SendUssd( buf16 );
    QCOMPARE(result, testmode.m_error);
    QTest::qSleep(100);

    // close api
    delete m_ussdApi;
    m_ussdApi = NULL;
    QTest::qSleep(100);

    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::sendCancel()
{
    TestEnv::create();
    QTest::qSleep(200);

    // create api
    createUssdApi(false);

    // send USSD
    _LIT( KMsg, "#12345*" );
    TestEnv::env().setTestMode(TestMode(KErrNone));
    int result = m_ussdApi->SendUssd( KMsg );
    QCOMPARE(result, KErrNone);
    m_ussdApi->SendUssdCancel();

    // close api
    delete m_ussdApi;
    m_ussdApi = NULL;
    QTest::qSleep(100);

    TestEnv::destroy();
    QTest::qSleep(200);
}


void Ut_Ussd::sendUssd8bit_data()
{
    QTest::addColumn<bool>("ui");
    QTest::addColumn<QString>("message");
    QTest::addColumn<TestMode>("testmode");

    // without ui
    QTest::newRow("1.1") << false << "8" << TestMode(KErrNone);
    QTest::newRow("1.2") << false << "#*888*" << TestMode(KErrUnknown,600);
    QTest::newRow("1.3") << false << "illegal" << TestMode(KErrNotSupported);
    // show ui
    QTest::newRow("2.1") << true << "#*555*" << TestMode(KErrNone,20);
    QTest::newRow("2.2") << true << "#*555*" << TestMode(KErrCancel,500);
    QTest::newRow("2.3") << true << "#*555*" << TestMode(KErrInUse,1000);
    QTest::newRow("2.4") << true << "#*555*" << TestMode(KErrAccessDenied,90);
    QTest::newRow("2.5") << true << "#*555*" << TestMode(KErrGsmSSCallBarred,190);
    QTest::newRow("2.6") << true << "#*555*" << TestMode(KErrGsmSMSNoNetworkService,700);
    QTest::newRow("2.7") << true << "#*555*" << TestMode(KErrGsmOfflineOpNotAllowed,1200);
    QTest::newRow("2.8") << true << "#*555*" << TestMode(KErrSatControl,200);
    QTest::newRow("2.9") << true << "#*555*" << TestMode(KErrUnknown,920);
}

void Ut_Ussd::sendUssd8bit()
{
    TestEnv::create();
    QTest::qSleep(200);

    QFETCH(bool, ui);
    QFETCH(QString, message);
    QFETCH(TestMode, testmode);
    TestEnv::env().setTestMode(testmode);

    // create api
    createUssdApi(ui);

    // send USSD
    TBuf<100> buf16;
    buf16.Copy( (const TUint16*)message.constData(), message.length() );
    TBuf8<100> buf8;
    buf8.Copy( buf16 );
    int result = m_ussdApi->SendUssd( buf8 );
    QCOMPARE(result, testmode.m_error);
    QTest::qSleep(100);

    // close api
    delete m_ussdApi;
    m_ussdApi = 0;
    QTest::qSleep(100);

    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::openAndCloseSatApi()
{
    TestEnv::create();
    QTest::qSleep(200);
    
    m_ussdSatApi = NULL;
    createUssdSatApi(m_ussdSatApi);
    
    delete m_ussdSatApi;
    m_ussdSatApi = 0;

    TestEnv::destroy();
    QTest::qSleep(200);    
}

void Ut_Ussd::sendSatMessage_data()
{
    QTest::addColumn<bool>("ui");
    QTest::addColumn<QString>("message");
    QTest::addColumn<TestMode>("testmode");

    // without ui
    QTest::newRow("1.1") << false << "123456" << TestMode(KErrNone);
    QTest::newRow("1.2") << false << "*100#" << TestMode(KErrNone,1000);
    QTest::newRow("1.3") << false << "8" << TestMode(KErrNone,400);
    //QTest::newRow("1.4") << false << "illegal" << TestMode(KErrNotSupported);

}

void Ut_Ussd::sendSatMessage()
{
    TestEnv::create();
    QTest::qSleep(200);

    QFETCH(bool, ui);
    QFETCH(QString, message);
    QFETCH(TestMode, testmode);
    TestEnv::env().setTestMode(testmode);

    // create api
    createUssdSatApi(ui);

    TBuf<RSat::KStringMaxSize> receiveMessage;
    TBool sendCompletedFirst( EFalse );
    TUint8 receiveDcs = 0;
    TBuf<100> sendMessage;
    sendMessage.Copy( (const TUint16*)message.constData(), 
                       message.length() );

    TInt ret = m_ussdSatApi->SendSatMessage(
        sendMessage,
        CPhCltUssdSatClient::EPhCltDcsUnknown,
        receiveMessage,
        sendCompletedFirst,
        receiveDcs );
    
    qDebug("receive error=%d", ret);
    qDebug("receive receive msg=%s", &receiveMessage);
    qDebug("receive isCompleteFirst=%d", &sendCompletedFirst);
    qDebug("receive receive Dcs=%d", &receiveDcs);
    
    QVERIFY2(ret == testmode.m_error , "testSendSatMessage failed ");
    
    // close api
    delete m_ussdSatApi;
    m_ussdSatApi = NULL;
    QTest::qSleep(100);

    TestEnv::destroy();
    QTest::qSleep(200);    
}

void Ut_Ussd::sendReceive_data()
{
    QTest::addColumn<bool>("ui");
    QTest::addColumn<QString>("msg1");
    QTest::addColumn<QString>("msg2");
    QTest::addColumn<TestMode>("testmode");

    // without ui, empty resp
    QTest::newRow("1.1") << false << "*100#" << "" << TestMode(KErrNone,1000);
    QTest::newRow("1.2") << false << "*100#" << "" << TestMode(KErrNotSupported,400);
    // without ui, text resp
    QTest::newRow("2.1") << false << "*100#" << "12345" << TestMode(KErrNone,100);
    QTest::newRow("2.2") << false << "*100#" << "12345" << TestMode(KErrUnknown,800);

    // show ui, empty resp
    QTest::newRow("3.1") << true << "*100#" << "" << TestMode(KErrNone,700);
    QTest::newRow("3.2") << true << "*100#" << "" << TestMode(KErrNotFound,400);
    // show ui, text resp
    QTest::newRow("4.1") << true << "*100#" << "12345" << TestMode(KErrNone,2000);
    QTest::newRow("4.2") << true << "*100#" << "12345" << TestMode(-800,800);

}


void Ut_Ussd::sendReceive()
{
    TestEnv::create();
    QTest::qSleep(200);

    QFETCH(bool, ui);
    QFETCH(QString, msg1);
    QFETCH(QString, msg2);
    QFETCH(TestMode, testmode);

    createUssdApi(ui);

    TestEnv::env().setTestMode(TestMode(KErrNone));
    send(msg1);
    TestEnv::env().setTestMode(testmode);
    receive(msg2, RMobileUssdMessaging::EUssdMTReply);
    activeSleep(500);
    networkRelease( KErrNone );
    activeSleep(500);

    // close api
    delete m_ussdApi;
    m_ussdApi = NULL;
    QTest::qSleep(100);

    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::testMultipleIncomingMessages()
{
    TestEnv::create();
    QTest::qSleep(200);

    QString msg1 = "your favorite color?\n1=red\n2=blue";
    QString msg2 = "your favorite food?\n1=rice\n2=potatoes";
    QString msg3 = "thank you";
    receive(msg1,RMobileUssdMessaging::EUssdMTRequest);
    activeSleep(1000);
    receive(msg2,RMobileUssdMessaging::EUssdMTRequest);
    activeSleep(1000);
    receive(msg3,RMobileUssdMessaging::EUssdMTNotify);
    activeSleep(1000);

    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::testUssdInternalApi_data()
{
    QTest::addColumn<bool>("ui");
    QTest::addColumn<QString>("msg1");
    QTest::addColumn<QString>("msg2");
    QTest::addColumn<TestMode>("testmode");

    // without ui
    QTest::newRow("1.1") << false << "1" << "" << TestMode(KErrNone,100);
    QTest::newRow("1.2") << false << "1" << "23" << TestMode(KErrNone,200);
    QTest::newRow("1.3") << false << "1" << "" << TestMode(KErrUnknown,300);
    QTest::newRow("1.4") << false << "1" << "23" << TestMode(KErrNotFound,400);
    // show ui
    QTest::newRow("1.1") << true << "1" << "" << TestMode(KErrNone,100);
    QTest::newRow("1.2") << true << "1" << "23" << TestMode(KErrNone,200);
    QTest::newRow("1.3") << true << "1" << "" << TestMode(KErrUnknown,300);
    QTest::newRow("1.4") << true << "1" << "23" << TestMode(KErrNotFound,400);
}

void Ut_Ussd::testUssdInternalApi()
{
    TestEnv::create();
    QTest::qSleep(200);

    QFETCH(bool, ui);
    QFETCH(QString, msg1); // send
    QFETCH(QString, msg2); // receive1
    QFETCH(TestMode, testmode);

	createUssdInternalApi(ui);

	int err = m_ussdInternalApi->AppStarting();
	QCOMPARE(err,KErrNone);
    QTest::qSleep(100);

	err = m_ussdInternalApi->AppToBackground();
	QCOMPARE(err,KErrNone);
    QTest::qSleep(100);

	err = m_ussdInternalApi->AppToForeground();
    QTest::qSleep(100);

    err = m_ussdInternalApi->AppTerminating(EPhCltUserExit);
	QCOMPARE(err,KErrNone);
    QTest::qSleep(100);

	err = m_ussdInternalApi->AppStarting();
	QCOMPARE(err,KErrNone);
    QTest::qSleep(100);

    TBuf<200> reply_buf16;
    reply_buf16.Copy( (const TUint16*)msg1.constData(), msg1.length() );
    int result = m_ussdInternalApi->SendUssd( reply_buf16 );
    QCOMPARE(result, TestEnv::env().testMode().m_error);
    QTest::qSleep(100);

    receive(msg2, RMobileUssdMessaging::EUssdMTNotify);
    activeSleep(100);

    err = m_ussdInternalApi->AppTerminating(EPhCltSendCompleted);
	QCOMPARE(err,KErrNone);
    QTest::qSleep(100);

    // close internal api
    delete m_ussdInternalApi;
    m_ussdInternalApi = NULL;
    QTest::qSleep(100);

    TestEnv::destroy();
    QTest::qSleep(200);
}

void Ut_Ussd::createUssdApi(bool ui)
{
    TRAPD(leave, m_ussdApi = CPhCltUssd::NewL(ui));
    QTest::qSleep(200);
    if (leave != KErrNone)
        QFAIL("CPhCltUssd::NewL Leave");
    if (NULL == m_ussdApi)
        QFAIL("CPhCltUssd pointer null");
    qDebug("m_ussdApi=%d", m_ussdApi);
}

void Ut_Ussd::createUssdInternalApi(bool ui)
{
    TRAPD(leave, m_ussdInternalApi = CPhCltUssdImp::NewL(ui));
    QTest::qSleep(200);
    if (leave != KErrNone)
        QFAIL("CPhCltUssdInt::NewL Leave");
    if (NULL == m_ussdInternalApi)
        QFAIL("CPhCltUssdInt pointer null");
    qDebug("m_ussdInternalApi=%d", m_ussdInternalApi);
}

void Ut_Ussd::createUssdSatApi(bool ui)
{
    TRAPD(leave, m_ussdSatApi = CPhCltUssdSatClient::NewL(ui));
    QTest::qSleep(200);
    if (leave != KErrNone)
        QFAIL("CPhCltUssdSatClient::NewL Leave");
    if (NULL == m_ussdSatApi)
        QFAIL("CPhCltUssdSatClient pointer null");
    qDebug("m_ussdSatApi=%d", m_ussdSatApi);
}

void Ut_Ussd::send(const QString& msg)
{
    qDebug("Ut_Ussd::send SendUssd in");
    QVERIFY(m_ussdApi);
    TBuf<200> send_buf16;
    send_buf16.Copy( (const TUint16*)msg.constData(), msg.length() );
    qDebug("Ut_Ussd::send SendUssd before");
    int result = m_ussdApi->SendUssd( send_buf16 );
    qDebug("Ut_Ussd::send SendUssd after");
    QCOMPARE(result, TestEnv::env().testMode().m_error);
    qDebug("Ut_Ussd::send SendUssd out");
}

// type = {
// 	RMobileUssdMessaging::EUssdMTRequest
// 	RMobileUssdMessaging::EUssdMTReply
// 	RMobileUssdMessaging::EUssdMTNotify
// }

void Ut_Ussd::receive(const QString& msg, int type)
{
    qDebug("Ut_Ussd::send receive in");
    QTest::qSleep(TestEnv::env().testMode().m_timeout);
    TBuf<200> rcv_buf16;
    rcv_buf16.Copy( (const TUint16*)msg.constData(), msg.length() );
    TBuf8<200> rcv_buf8;
    rcv_buf8.Copy( rcv_buf16 );
	// prepare attributes
    RMobileUssdMessaging::TMobileUssdAttributesV1 attr;
    attr.iFlags = RMobileUssdMessaging::KUssdMessageType;
    attr.iType = (RMobileUssdMessaging::TMobileUssdMessageType)type;
    RMobileUssdMessaging::TMobileUssdAttributesV1Pckg attrpkg(attr);
    
    TestEnv::env().etelMsgLoop().receive( rcv_buf8, attrpkg );
    qDebug("Ut_Ussd::send receive out");
}

void Ut_Ussd::networkRelease(int error)
{
    qDebug("Ut_Ussd::send networkRelease in");
    RMobilePhone::TMobilePhoneSendSSRequestV3 attr;
    attr.iOpCode = error;
    RMobilePhone::TMobilePhoneSendSSRequestV3Pckg attrpkg(attr);
    
    TestEnv::env().etelMsgLoop().networkRelease( _L8(""), attrpkg );
    qDebug("Ut_Ussd::send networkRelease out");
}


// global variables
CPeriodic* activeSleepTimer = 0;

TInt ActiveSleepTimerCallback(TAny* aPtr)
{
    if ( activeSleepTimer ) {
        qDebug("Ut_Ussd::ActiveSleepTimerCallback Cancel");
        activeSleepTimer->Cancel();
     }
    CActiveSchedulerWait* wait = (CActiveSchedulerWait*) aPtr;
    if (wait) {
        qDebug("Ut_Ussd::ActiveSleepTimerCallback AsyncStop");
        wait->AsyncStop();
    }
    return 0;
}

// This is a special SLEEP method that allows active objects to run meanwhile.
// Note: because of its active nature, this sleep method is very inaccurate.
void Ut_Ussd::activeSleep(int ms)
{
    qDebug("Ut_Ussd::activeSleep in");

    CActiveSchedulerWait* wait = new CActiveSchedulerWait;
    activeSleepTimer = CPeriodic::NewL(CActive::EPriorityStandard+1);
    TTimeIntervalMicroSeconds32 delay(1000*ms);
    activeSleepTimer->Start(delay,delay,
        TCallBack(ActiveSleepTimerCallback,(TAny*)wait));
    if ( wait && wait->IsStarted() ) {
        wait->AsyncStop();
        delete activeSleepTimer;
        activeSleepTimer = 0;
        delete wait;
        wait = NULL;
        qDebug("Ut_Ussd::activeSleep out unnormal");
        return;
    }
    
    if (wait) {
         qDebug("Ut_Ussd::activeSleep start snyc before");
         wait->Start();
         qDebug("Ut_Ussd::activeSleep start snyc after");
    }
    delete activeSleepTimer;
    activeSleepTimer = 0;
    delete wait;
    wait = NULL;
    qDebug("Ut_Ussd::activeSleep out");
}

//End of file

