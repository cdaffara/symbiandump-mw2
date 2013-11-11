/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  ?Description
*
*/

#include <s32mem.h>
#include "commontestclass.h"
#include <mpxpluginhandlerbase.h>
#include <ecom/ECom.h>

#include <mpxplugininfo.h>


#define KNullUid TUid::Null()
class CDummyMpxPluginHandler : public CMPXPluginHandlerBase
    {
public:  // Constructors and destructor
    CDummyMpxPluginHandler();  
    ~CDummyMpxPluginHandler();
    
    void ResolvePluginL();
    };

class CDummyCMPXPluginInfo : public CMPXPluginInfo
	{
public:
	CDummyCMPXPluginInfo();
	void BaseProcessTaggedDataExL(const TDesC8& aTag, const TDesC8& aData);
	void BaseExtractIntoArrayL(const TDesC8& aText, CDesCArray*& aArray);
	~CDummyCMPXPluginInfo();
	};

// -----------------------------------------------------------------------------
// CDummyCMPXPluginInfo::BaseProcessTaggedDataExL()
// -----------------------------------------------------------------------------
void CDummyCMPXPluginInfo::BaseProcessTaggedDataExL(const TDesC8& aTag, const TDesC8& aData)
	{
	ProcessTaggedDataExL(aTag,aData);
	}

// -----------------------------------------------------------------------------
// CDummyCMPXPluginInfo::BaseExtractIntoArrayL()
// -----------------------------------------------------------------------------
void CDummyCMPXPluginInfo::BaseExtractIntoArrayL(const TDesC8& aText, CDesCArray*& aArray)
	{
	ExtractIntoArrayL(aText,aArray);
	}

// -----------------------------------------------------------------------------
// CDummyCMPXPluginInfo::CDummyCMPXPluginInfo()
// -----------------------------------------------------------------------------
CDummyCMPXPluginInfo::CDummyCMPXPluginInfo():CMPXPluginInfo()
	{
	//do nothing
	}

// -----------------------------------------------------------------------------
// CDummyCMPXPluginInfo::~CDummyCMPXPluginInfo()
// -----------------------------------------------------------------------------
CDummyCMPXPluginInfo::~CDummyCMPXPluginInfo()
	{
	//do nothing
	}



// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXPluginInfoDestructor()
// Returns: Symbian OS errors.
// added
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXPluginInfoDestructor()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoDestructor testing CMPXPluginInfo::~ begin")));
    iLog->Log(_L("CCommonTestClass::CMPXPluginInfoDestructor testing CMPXPluginInfo::~ begin"));
    TInt err = KErrNone;
    delete iPluginInfo;
    iPluginInfo = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoDestructor testing CMPXPluginInfo::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CMPXPluginInfoDestructor testing CMPXPluginInfo::~ end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXPluginInfoImplementationUid()
// Returns: Symbian OS errors.
// added
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXPluginInfoImplementationUid(CStifItemParser& /*aItem*/)
    {
	FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoImplementationUid testing CMPXPluginInfo::ImplementationUid begin")));
    iLog->Log(_L("CCommonTestClass::CMPXPluginInfoImplementationUid testing CMPXPluginInfo::ImplementationUid begin"));
    
    TInt err = KErrNone;
    
    iPluginInfo = new CDummyCMPXPluginInfo();
    TRAP(err,iPluginInfo->ImplementationUid());
    delete iPluginInfo;
    iPluginInfo = NULL;
	
    FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoImplementationUid testing CMPXPluginInfo::ImplementationUid end err=%d"), err));
	iLog->Log(_L("CCommonTestClass::CMPXPluginInfoImplementationUid testing CMPXPluginInfo::ImplementationUid end err=%d"), err);
	
    return err;
    
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXPluginInfoFlags()
// Returns: Symbian OS errors.
// added
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXPluginInfoFlags(CStifItemParser&)
    {//TODO need to update
	FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoFlags testing CMPXPluginInfo::Flags begin")));
    iLog->Log(_L("CCommonTestClass::CMPXPluginInfoFlags testing CMPXPluginInfo::Flags begin"));
    TInt err = KErrNone;
//    TUid iInterfaceUid;
//    CDummyMpxPluginHandler pluginHandler;
//    TRAP(err,pluginHandler.CreatePluginListL());
//    RMPXPointerArray<CImplementationInformation> implInfoArray;
//    TRAP(err,REComSession::ListImplementationsL(iInterfaceUid, implInfoArray));
//    RPointerArray<CMPXPluginInfo>       iPluginInfoArray;   
//    TUint ret( 0 );
//    TInt count = 1;
//    TBool found( EFalse );
//
//    for( TInt i = 0; !found && i < count; ++i )
//          {
//          if (  iPluginInfoArray[i]->ImplementationUid()== KNullUid )
//              {
//              ret = iPluginInfoArray[i]->Flags();
//              found = ETrue;
//              }
//          }
//    if ( !found )
//          {    
//          FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoFlags testing CMPXPluginInfo::Flags end return TUint=%d"), ret));
//   	      iLog->Log(_L("CCommonTestClass::CMPXPluginInfoFlags testing CMPXPluginInfo::Flags end return TUint=%d"), ret);              		         
//          User::Leave( KErrNotFound );
//          }
//    REComSession::FinalClose();

    _LIT8(KPluginData,"Plugin Data");
    _LIT8(KPluginTag,"Plugin Tag");
    //iPluginInfo->ProcessTaggedDataL( KPluginTag,KPluginData );
    iPluginInfo = new CDummyCMPXPluginInfo();
    TUint flag = iPluginInfo->Flags();
    delete iPluginInfo;
    iPluginInfo = NULL;
    
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL()
// Returns: Symbian OS errors.
// added
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL(CStifItemParser& /*aItem*/)
    {
	FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL testing CMPXPluginInfo::ProcessTaggedDataExL begin")));
    iLog->Log(_L("CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL testing CMPXPluginInfo::ProcessTaggedDataExL begin"));
    
    TInt err = KErrNone;
    CDummyCMPXPluginInfo* dummyPluginInfo = new CDummyCMPXPluginInfo();
    _LIT8( KTag,"Tag");
    _LIT8( KData,"Data");
    //dummyPluginInfo->BaseProcessTaggedDataExL( KTag,KData );
    TRAP( err,dummyPluginInfo->BaseProcessTaggedDataExL( KTag,KData ) );
	if( err )
		{
		iLog->Log( _L("CMPXPluginInfo::ProcessTaggedDataExL returned error %d."), err );
		}
	else
		{
		iLog->Log( _L("CMPXPluginInfo::ProcessTaggedDataExL returned no error.") );
		}
	delete dummyPluginInfo;
    FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL testing CMPXPluginInfo::ProcessTaggedDataExL end err=%d"), err));
	iLog->Log(_L("CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL testing CMPXPluginInfo::ProcessTaggedDataExL end err=%d"), err);
	
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXPluginInfoExtractIntoArrayL()
// Returns: Symbian OS errors.
// added
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXPluginInfoExtractIntoArrayL(CStifItemParser& /*aItem*/)
    {
	FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoExtractIntoArrayL testing CMPXPluginInfo::ExtractIntoArrayL begin")));
    iLog->Log(_L("CCommonTestClass::CMPXPluginInfoExtractIntoArrayL testing CMPXPluginInfo::ExtractIntoArrayL begin"));
    
    TInt err = KErrNone;
    CDummyCMPXPluginInfo* dummyPluginInfo = new CDummyCMPXPluginInfo();
    _LIT8( KTag,"Tag");
    //TDesC8 des;
    CDesCArray* array = new(ELeave)CDesCArrayFlat(10);    
    array->AppendL(_L("test"));
    
    //dummyPluginInfo->BaseExtractIntoArrayL( KTag,array );
    
    TRAP( err,dummyPluginInfo->BaseExtractIntoArrayL( KTag,array ) );
	if( err )
		{
		iLog->Log( _L("CMPXPluginInfo::ExtractIntoArrayL returned error %d."), err );
		}
	else
		{
		iLog->Log( _L("CMPXPluginInfo::ExtractIntoArrayL returned no error.") );
		}
	delete dummyPluginInfo;
    FTRACE(FPrint(_L("CCommonTestClass::CMPXPluginInfoExtractIntoArrayL testing CMPXPluginInfo::ExtractIntoArrayL end err=%d"), err));
	iLog->Log(_L("CCommonTestClass::CMPXPluginInfoExtractIntoArrayL testing CMPXPluginInfo::ExtractIntoArrayL end err=%d"), err);
	
    return err;
    }

