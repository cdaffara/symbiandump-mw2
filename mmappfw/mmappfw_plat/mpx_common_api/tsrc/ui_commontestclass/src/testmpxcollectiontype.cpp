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
* Description: MPXCollectionType testing implementation (mpxcollectiontype.h)
*
*/


#include <s32mem.h>
#include "commontestclass.h"

// Begin CMPXCollectionType testing implementation (mpxcollectiontype.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateMPXCollectionType()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateMPXCollectionType()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() begin")));
    iLog->Log(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() begin"));
    TInt err = KErrNone;
    if ( iType != NULL)
        {
        delete iType;
        }
    iType = NULL;
    TRAP(err , iType = new ( ELeave ) CMPXCollectionType());
    if ( iType == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() end err=%d"), err);
	return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXCollectionTypeDelete()
// Returns: Symbian OS errors.
// add CMPXCollectionTypeDelete to testmpxcollectiontype.cpp 9/19/2008 1:32
// -----------------------------------------------------------------------------  
TInt CCommonTestClass::CMPXCollectionTypeDelete(CStifItemParser&)
    {
    FTRACE(FPrint(_L("CCommonTestClass::CMPXCollectionTypeDelete testing CMPXCollectionType::~ begin")));
    iLog->Log(_L("CCommonTestClass::CMPXCollectionTypeDelete testing CMPXCollectionType::~ begin"));
    TInt err = KErrNone;
    delete iType;
    iType = NULL;
    FTRACE(FPrint(_L("CCommonTestClass::CMPXCollectionTypeDelete testing CMPXCollectionType::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CMPXCollectionTypeDelete testing CMPXCollectionType::~ end err=%d"), err);
    return err;
    }

TInt CCommonTestClass::CreateMPXCollectionTypeWOParam()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() begin")));
    iLog->Log(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() begin"));
    TInt err = KErrNone;
    if ( iType != NULL)
        {
        delete iType;
        }
    iType = NULL;
    TRAP(err , iType = new ( ELeave ) CMPXCollectionType());
    if ( iType == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() end err=%d"), err);
	return err;
    }



// -----------------------------------------------------------------------------
// CCommonTestClass::InternalizeMPXCollectionTypeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::InternalizeMPXCollectionTypeL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXCollectionTypeL testing CMPXCollectionType::InternalizeMPXCollectionTypeL begin")));
    iLog->Log(_L("CCommonTestClass::InternalizeMPXCollectionTypeL testing CMPXCollectionType::InternalizeMPXCollectionTypeL begin"));
    TInt err = KErrNone;

    if ( iType != NULL )
        {
    	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXCollectionTypeL started Internalize")));
        iLog->Log(_L("CCommonTestClass::InternalizeMPXCollectionTypeL started Internalize"));
        CBufBase* buffer = CBufFlat::NewL( 50 );
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        iType->ExternalizeL( writeStream );
        writeStream.CommitL();
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );

        RBufReadStream readStream( *buffer );
        CleanupClosePushL( readStream );                
        iType = new ( ELeave ) CMPXCollectionType();        
        iType->InternalizeL( readStream );
        CleanupStack::PopAndDestroy( &readStream );
        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXCollectionTypeL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::InternalizeMPXCollectionTypeL Stif test script is wrong."));
        }


	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXCollectionTypeL testing CMPXCollectionType::InternalizeMPXCollectionTypeL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::InternalizeMPXCollectionTypeL testing CMPXCollectionType::InternalizeMPXCollectionTypeL end err=%d"), err);
	return err;
    }
    
// -----------------------------------------------------------------------------
// CCommonTestClass::ExternalizeMPXCollectionTypeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ExternalizeMPXCollectionTypeL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL testing CMPXCollectionType::ExternalizeMPXCollectionTypeL begin")));
    iLog->Log(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL testing CMPXCollectionType::ExternalizeMPXCollectionTypeL begin"));
    TInt err = KErrNone;
    if ( iType != NULL )
        {
    	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL started Externalize")));
        iLog->Log(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL started Externalize"));
        CBufBase* buffer = CBufFlat::NewL( 50 );
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        iType->ExternalizeL( writeStream );
        writeStream.CommitL();
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );
        CleanupStack::PopAndDestroy( buffer ); 
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL Stif test script is wrong."));
        }
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL testing CMPXCollectionType::ExternalizeMPXCollectionTypeL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ExternalizeMPXCollectionTypeL testing CMPXCollectionType::ExternalizeMPXCollectionTypeL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CreateMPXCollectionTypeaUid()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

TInt CCommonTestClass::TUidMPXCollectionType()
    {
    TUid* uid=new TUid();
    TUid& aUid=*uid;
    CDesCArray* aMimeTypes(NULL);
    CDesCArray* aExtensions(NULL);
    FTRACE(FPrint(_L("CCommonTestClass::TUidMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() begin")));
    iLog->Log(_L("CCommonTestClass::TUidMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() begin"));
    TInt err = KErrNone;
    if ( iType != NULL)
        {
        delete iType;
        }
    iType = NULL;
    TRAP(err , iType = new ( ELeave ) CMPXCollectionType(aUid ,aMimeTypes,aExtensions));
    if ( iType == NULL)
        {
        err = KErrNotFound;
        }
      FTRACE(FPrint(_L("CCommonTestClass::TUidMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() end err=%d"), err));
      iLog->Log(_L("CCommonTestClass::TUidMPXCollectionType testing CMPXCollectionType::CMPXCollectionType() end err=%d"), err);
      return err;
    }
    

