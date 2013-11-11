

// include files
#include <QObject> // base class

// forward declarations
class CPhCltUssd;
class CPhCltUssdImp;
class CPhCltUssdSatClient;

// class declaration
class Ut_Ussd: public QObject
{
    Q_OBJECT

private slots: // test cases
    
    // Test for CPhCltUssd
    void testTestEnv();
    void openAndCloseApi();
    void sendUssd16bit_data();
    void sendUssd16bit();
    void sendUssd8bit_data();
    void sendUssd8bit();
    void sendCancel();

    // Test for CPhCltUssdSatClient
    void openAndCloseSatApi();
    void sendSatMessage_data();
    void sendSatMessage();

    // receiving messages
	void sendReceive_data();
	void sendReceive();
	void testMultipleIncomingMessages();

    // cases for internal api
	void testUssdInternalApi_data();
	void testUssdInternalApi();

private: // tet blocks

    void createUssdApi(bool ui);
    void createUssdInternalApi(bool ui);
    void createUssdSatApi(bool ui);
	void send(const QString& msg);
	void receive(const QString& msg, int type);
	void networkRelease(int error);
	void activeSleep(int ms);


private: // test-related members

    // phone client
    CPhCltUssd *m_ussdApi;
    CPhCltUssdImp *m_ussdInternalApi;
    CPhCltUssdSatClient *m_ussdSatApi;

};

