#include <QtTest/QtTest>
#include <mock_centralrepository.h>

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
    aValue = mValue;
    return KErrNone;
}
TInt CRepository::Set(TUint32 /*aKey*/, TInt aValue)
{
    mValue = aValue;
    return KErrNone;
}

TInt CRepository::Get(TUint32 /*aKey*/, TDes16& aValue)
{
    return KErrNone;
}

TInt CRepository::Set(TUint32 /*aKey*/, const TDesC16& /*aValue*/)
{
    return KErrNone;
}
