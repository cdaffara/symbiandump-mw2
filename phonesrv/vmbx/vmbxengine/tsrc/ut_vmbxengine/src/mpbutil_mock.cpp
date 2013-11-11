#include <mpbutil.h>
#include <e32base.h>
#include <e32des8.h>

TInt time = 0;

CPhoneBookBuffer::CPhoneBookBuffer():iMonitor(NULL,0,0)
{
}

void CPhoneBookBuffer::Set(TDes8* /*aData*/){}

TInt CPhoneBookBuffer::AddNewEntryTag()
{ return 0; }

TInt CPhoneBookBuffer::AddNewNumberTag()
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, TUint8 /*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, TUint16 /*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, TUint32 /*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, const TDesC8 &/*aData*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, const TDesC16 &/*aData*/)
{ return 0; }

TInt CPhoneBookBuffer::RemovePartialEntry()
{ return 0; }

void CPhoneBookBuffer::StartRead(){}

TInt CPhoneBookBuffer::GetTagAndType(TUint8 &aTagValue, TPhBkTagType &aDataType)
{ 
	switch(aDataType)
		{
		case CPhoneBookBuffer::EPhBkTypeNoData:
			{
			if(time == 0)
				{
				aTagValue = RMobilePhoneBookStore::ETagPBNewEntry;
				time ++;
				}
			else if (time == 1)
				{
				aTagValue = RMobilePhoneBookStore::ETagPBNumber;
				time ++;
				}
			else if (time == 2)
				{
				aTagValue = RMobilePhoneBookStore::ETagPBText;
				time ++;
				}
			else if (time == 3)
				{
				aTagValue = RMobilePhoneBookStore::ETagPBTonNpi;
				time ++;
				}
			else
				{
				time = 0;
				return KErrNotFound;
				}
			break;
			}
		default:
			aTagValue = 0;
		}
	
	return KErrNone; 
}

TInt CPhoneBookBuffer::GetValue(TUint8 &/*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TUint16 &/*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TUint32 &/*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TPtrC8 &/*aData*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TPtrC16 &/*aData*/)
{ return 0; }

void CPhoneBookBuffer::SkipValue(TPhBkTagType /*aDataType*/){}

TInt CPhoneBookBuffer::BufferLength()
{ return 0; }

TInt CPhoneBookBuffer::RemainingReadLength()
{ return 0; }
