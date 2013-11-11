#include <QtTest/QtTest>
#include <cvoicemailboxentry.h>
#include "vmbxpbkstore.h"
#include "ut_pbkstore.h"

Ut_PbkStore::Ut_PbkStore():mPbk(NULL)
{
    createPbk();
}

Ut_PbkStore::~Ut_PbkStore()
{
    deletePbk();
}

void Ut_PbkStore::testCreateAndDelete()
{
    createPbk();
    deletePbk(); 
}

void Ut_PbkStore::testIsWritable()
{
    createPbk();
    QCOMPARE(mPbk->IsWritable(), 1);
    deletePbk();     
}
void Ut_PbkStore::testPhoneBookType()
{
    createPbk();
    QCOMPARE(mPbk->PhoneBookType(), EMBDNPhoneBook);
    deletePbk(); 
}


void Ut_PbkStore::testWrite()
{
    createPbk();
    CVoiceMailboxEntry *entry = NULL;
    TInt err = KErrNotFound;
    
    TRAP(err, entry = CVoiceMailboxEntry::NewL());
    qDebug("CVoiceMailboxEntry::NewL %d", err);
    err = mPbk->Write(*entry);
    qDebug("Write err %d", err);
    QCOMPARE(err, KErrNone);
    
    delete entry;
    entry = NULL;
    deletePbk(); 
}

void Ut_PbkStore::testPhonebookStore()
{
    createPbk();
    mPbk->PhonebookStore();
    deletePbk(); 
}

void Ut_PbkStore::testGetL()
{
    createPbk();
    TInt err = KErrNotFound;
    CVoiceMailboxEntry *entry = NULL;
    
    TRAP(err, entry = CVoiceMailboxEntry::NewL());
    qDebug("CVoiceMailboxEntry::NewL %d", err);
    mPbk->GetL(*entry);
    
    delete entry;
    entry = NULL;
    deletePbk(); 
}

void Ut_PbkStore::createPbk()
{
    if (!mPbk){
        TRAPD(err, mPbk = CVmbxPbkStore::NewL());
        QVERIFY(mPbk);  
        qDebug("Create CVmbxPbkStore", err);
    }
}

void Ut_PbkStore::deletePbk()
{
    delete mPbk;
    mPbk = NULL;
    qDebug("Delete CVmbxPbkStore");
}
