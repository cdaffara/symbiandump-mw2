#include <QtTest/QtTest>
#include <centralrepository.h>
#include "voicemailboxdomaincrkeys.h"
#include "voicemailboxprivatecrkeys.h"

_LIT(KStringValue, "123");

// TODO: Add a new class named Ut_Utility to config all CR key
// to simulate 3 different images
CRepository* CRepository::NewL(TUid /*aRepositoryUid*/)
{
    CRepository *self = new (ELeave) CRepository();
    return self;
}

CRepository::~CRepository()
{
    
}

TInt CRepository::Get(TUint32 aKey, TInt& aValue)
{
    if (KVideoMbxSupport == aKey){
        aValue = 1;
        qDebug("KVideoMbxSupport 1");
    } else if (KVmbxNoSimWriting == aKey){
        aValue = 0;
        qDebug("KVmbxNoSimWriting 0");
    } else if (KVmbxUiCaps == aKey){
        aValue = 0;
        qDebug("KVmbxUiCaps 1");
    } else {
        aValue = 0;
        qDebug("default 0");
    }
    
    return KErrNone;
}
TInt CRepository::Set(TUint32 /*aKey*/, TInt /*aValue*/)
{
    return KErrNone;
}

TInt CRepository::Get(TUint32 /*aKey*/, TDes16& aValue)
{
    aValue.Append(KStringValue);
    return KErrNone;
}

TInt CRepository::Set(TUint32 /*aKey*/, const TDesC16& /*aValue*/)
{
    return KErrNone;
}
