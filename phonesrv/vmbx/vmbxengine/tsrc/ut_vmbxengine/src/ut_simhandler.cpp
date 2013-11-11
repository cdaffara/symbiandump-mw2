#include <QtTest/QtTest>
#include <cvoicemailboxentry.h>
#include "vmbxsimhandler.h"
#include "ut_simhandler.h"

Ut_SimHandler::Ut_SimHandler():mSim(NULL)
{
    createSim();
}

Ut_SimHandler::~Ut_SimHandler()
{
    deleteSim();
}

void Ut_SimHandler::testCreateAndDelete()
{
    createSim();
    deleteSim(); 
}

//void Ut_SimHandler::testGetL()
//{
//    createSim();
//    TInt err = KErrNotFound;
//    CVoiceMailboxEntry *entry = NULL;
//    
//    TRAP(err, entry = CVoiceMailboxEntry::NewL());
//    qDebug("CVoiceMailboxEntry::NewL %d", err);
//    mSim->GetL(*entry);
//    
//    delete entry;
//    entry = NULL;
//    deleteSim(); 
//}
//
//void Ut_SimHandler::testSave()
//{
//    createSim();
//    
//    TInt err = KErrNotFound;
//    CVoiceMailboxEntry *entry = NULL;
//    
//    TRAP(err, entry = CVoiceMailboxEntry::NewL());
//    qDebug("CVoiceMailboxEntry::NewL %d", err);    
//    QCOMPARE(mSim->Save(*entry), KErrNone);
//    
//    deleteSim(); 
//}

void Ut_SimHandler::testIsWritable()
{
    createSim();
    QCOMPARE(mSim->IsWritable(), 1);
    deleteSim();     
}

void Ut_SimHandler::testNotifyStoreEvent()
{
    createSim();
    TRequestStatus status;
    TUint32 event;
    TInt index; 
    mSim->NotifyStoreEvent(status, event, index);
    deleteSim();      
}


void Ut_SimHandler::testPhoneBookType()
{
    createSim();
    QCOMPARE(mSim->PhoneBookType(), EMBDNPhoneBook);
    deleteSim(); 
}

void Ut_SimHandler::testCancelAsyncRequest()
{
    createSim();
    TInt req = -1;
    mSim->CancelAsyncRequest(req);
    deleteSim();  
}

void Ut_SimHandler::createSim()
{
    if (!mSim){
        TRAPD(err, mSim = CVmbxSimHandler::NewL());
        QVERIFY(mSim);  
        qDebug("Create CVmbxSimHandler", err);
    }
}

void Ut_SimHandler::deleteSim()
{
    delete mSim;
    mSim = NULL;
    qDebug("Delete CVmbxSimHandler");
}
