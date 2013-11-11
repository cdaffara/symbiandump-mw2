#include <etelmm.h>
#include <etel.h>
#include <e32std.h>
#include <rsssettings.h>
#include <QtTest/QtTest>

RPhone::RPhone()
{
}

void RPhone::ConstructL()
{   
}

TInt RPhone::Open(RTelServer& /*aSession*/,const TDesC& /*aName*/)
{
    return KErrNone;
}

void RPhone::Close()
{   
}

void RPhone::Destruct()
{
}

RMobilePhone::RMobilePhone()
{
}

void RMobilePhone::ConstructL()
{    
}

RMobilePhone::TMultimodeType::TMultimodeType()
{
}

TInt RMobilePhone::TMultimodeType::ExtensionId() const
{
    return 0;
}

void RMobilePhone::GetMailboxNumbers(
    TRequestStatus& aReqStatus, 
    TDes8& aMailBox) const
{
    RMobilePhone::TMobilePhoneVoicemailIdsV3 info;
    RMobilePhone::TMobilePhoneVoicemailIdsV3Pckg infoPckg(info);
    info.iVoice = 1;
    aMailBox.Copy(infoPckg);
    TRequestStatus* tmp = &aReqStatus;
    User::RequestComplete(tmp, KErrNone);
    qDebug("RMobilePhoneStore::GetMailboxNumbers mock");
}

RMobilePhone::TMobilePhoneVoicemailIdsV3::TMobilePhoneVoicemailIdsV3()
    : iVoice(0),
      iData(0),
      iFax(0),
      iOther(0) 
{  
}

void RMobilePhone::Destruct()
{   
}

TInt RMobilePhone::GetIccAccessCaps(TUint32& /*aCaps*/) const
{   
    return KErrNone;
}

RTelServer::RTelServer()
{ 
}

TInt RTelServer::Connect(TInt /*aMessageSlots*/)
{
    return KErrNone;
}

TInt RTelServer::LoadPhoneModule(const TDesC& /*aFileName*/) const
{
    return KErrNone;
}

TInt RTelServer::SetExtendedErrorGranularity(
    const TErrorGranularity /*aGranularity*/) const
{
    return KErrNone;
}

TInt RTelServer::GetPhoneInfo(
    const TInt /*aIndex*/, TPhoneInfo& /*aInfo*/) const
{
    return KErrNone;
}

RMobilePhoneStore::RMobilePhoneStore()
{
}

void RMobilePhoneStore::GetInfo(
    TRequestStatus& aReqStatus, TDes8& aInfo) const
{
    RMobilePhoneBookStore::TMobilePhoneBookInfoV1 info;
    info.iCaps = 0x10000000;
    RMobilePhoneBookStore::TMobilePhoneBookInfoV1Pckg pckgInfo( info );
    aInfo.Copy( pckgInfo ); 
    TRequestStatus* tmp = &aReqStatus;
    User::RequestComplete(tmp, KErrNone);
    qDebug("RMobilePhoneStore::GetInfo mock");
}

RMobilePhoneBookStore::RMobilePhoneBookStore()
{    
}

TInt RMobilePhoneBookStore::Open(
    RMobilePhone& /*aPhone*/, const TDesC& /*aStore*/)
{
    return KErrNone;
}

RMobilePhoneStore::TMobilePhoneStoreInfoV1::TMobilePhoneStoreInfoV1()
    :iType(RMobilePhoneStore::EPhoneStoreTypeUnknown),
    iTotalEntries(0),
    iUsedEntries(0),
    iCaps(0),
    iName(0)
{   
}

RMobilePhoneBookStore::TMobilePhoneBookInfoV1::TMobilePhoneBookInfoV1()
    :iMaxNumLength(0),
     iMaxTextLength(0),
     iLocation(RMobilePhoneBookStore::ELocationUnknown),
     iChangeCounter(0),
     iIdentity(0) 
{
}

void RMobilePhoneBookStore::Close()
{
}

void RMobilePhoneBookStore::Read(
    TRequestStatus& aReqStatus, TInt /*aIndex*/, 
    TInt /*aNumSlots*/, TDes8& aPBData) const
{ 
    TRequestStatus* tmp = &aReqStatus;
// data ?
    TBuf<5> pbdata(_L("data5"));
    aPBData.Copy(pbdata);
    
    User::RequestComplete(tmp, KErrNone);
}

void RMobilePhoneBookStore::Write(
    TRequestStatus& aReqStatus, 
    const TDesC8& /*aPBData*/, TInt& /*aIndex*/) const
{
    TRequestStatus* tmp = &aReqStatus;
    User::RequestComplete(tmp, KErrNone);
}

void RMobilePhoneStore::NotifyStoreEvent(
    TRequestStatus& aReqStatus, 
    TUint32& /*aEvent*/, TInt& /*aIndex*/) const
{
    TRequestStatus* tmp = &aReqStatus;
    User::RequestComplete(tmp, KErrNone);
}

RTelSubSessionBase::RTelSubSessionBase()
{
}

void RTelSubSessionBase::CancelAsyncRequest(TInt /*aReqToCancel*/) const
{
// refers to ut_ussd
}

TInt RSSSettings::Get( 
    TSSSettingsSetting /*aSetting*/,
    TInt& aValue )
{
	aValue = ESSSettingsAlsPrimary;
	return KErrNone;
}

