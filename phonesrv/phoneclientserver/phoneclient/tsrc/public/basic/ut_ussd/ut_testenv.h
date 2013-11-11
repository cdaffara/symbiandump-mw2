

// include files
#include <e32cmn.h>
#include <e32base.h>
#include <f32file.h>
#include "mphsrvphoneinterface.h" // base class

// forward declarations
class RTelServer;
class RMobilePhone;
class RMmCustomAPI;
class CPhSrvResourceManager;
class CPhSrvUssdManager;

/**
 * Mode for current test execution
 */
class TestMode
{
public:
	enum {Immediate=50};
	enum Event{Normal,NoResp,NetworkRelease};

	TestMode(Event event,int error=KErrNone,int timeout=Immediate)
		{m_event=event;m_error=error;m_timeout=timeout;}
	TestMode(int error,int timeout=Immediate)
		{m_event=Normal;m_error=error;m_timeout=timeout;}
	TestMode()
		{m_event=Normal;m_error=KErrNone;m_timeout=Immediate;}
	Event m_event;
	int m_error;
	int m_timeout;
};

/**
 * ETEL message loop
 */
class EtelMsgLoop
{
public:
	EtelMsgLoop();

	// methods
	void receive( const TDesC8& msg, const TDesC8& attr );
	void networkRelease( const TDesC8& msg, const TDesC8& attr );

	//data
	TRequestStatus* mSendMessageReqStatus;

	TRequestStatus* mReceiveMessageReqStatus;
	TDes8* mReceiveMessageData;
	TDes8* mReceiveMessageAttributes;

	TRequestStatus* mNetworkReleaseReqStatus;
	TDes8* mNetworkReleaseMessageData;
	TDes8* mNetworkReleaseMessageAttributes;

};



/**
 * test environment
 */
class TestEnv: public MPhSrvPhoneInterface
{
	public:

		static void create();
		static void destroy();
		static TestEnv& env(); // the singleton

		TestEnv();
		void construct();
		~TestEnv();

		CPhSrvUssdManager& ussdManager() {return *m_ussdManager;}

		void setTestMode(const TestMode& testMode) {m_testMode = testMode;}
		const TestMode& testMode() {return m_testMode;}

		EtelMsgLoop& etelMsgLoop() {return m_etelMsgLoop;}
		
		RMessage2& fakeMessage() {return *m_fakeMessage;}
		
	protected: // stubbed MPhSrvPhoneInterface

		RTelServer& PhSrvETelServer();
		RMobilePhone& PhSrvMobilePhone();
		RMmCustomAPI& PhSrvCustomPhone();

	private:
		
		// file manager. Never actually connected to
		RFs m_fs;

		// resource manager mock
		CPhSrvResourceManager* m_resourceManager;

		// ussd manager (server main ussd class)
		CPhSrvUssdManager* m_ussdManager;

		// test mode
		TestMode m_testMode;

		// etel msg loop
		EtelMsgLoop m_etelMsgLoop;
		
		// temp message
		RMessage2* m_fakeMessage;

	    static TestEnv* m_singleton;

};





