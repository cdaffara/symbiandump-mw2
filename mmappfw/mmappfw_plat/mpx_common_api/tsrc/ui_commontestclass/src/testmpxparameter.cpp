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
* Description:  MPXParameter testing implementation (mpxparameter.h)
*
*/


#include <s32mem.h>
#include "commontestclass.h"

#define KMPXPluginTypePlaybackUid           0x101FFCA0

// Begin CMPXParameter testing implementation (mpxparameter.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateMPXParameterL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateMPXParameter()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateMPXParameterL testing CMPXParameter::CMPXParameter() begin")));
    iLog->Log(_L("CCommonTestClass::CreateMPXParameterL testing CMPXParameter::CMPXParameter() begin"));
    TInt err = KErrNone;
    if ( iParameter != NULL)
        {
        delete iParameter;
        }
    iParameter = NULL;
    TRAP(err , iParameter = new ( ELeave ) CMPXParameter());
    if ( iParameter == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateMPXParameterL testing CMPXParameter::CMPXParameter() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateMPXParameterL testing CMPXParameter::CMPXParameter() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteMPXParameterL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteMPXParameter()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeleteMPXParameterL testing CMPXParameter::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteMPXParameterL testing CMPXParameter::~ begin"));
    TInt err = KErrNone;
    delete iParameter;
    iParameter = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::DeleteMPXParameterL testing CMPXParameter::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteMPXParameterL testing CMPXParameter::~ end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::InternalizeMPXParameterL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::InternalizeMPXParameterL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXParameterL testing CMPXParameter::InternalizeMPXParameterL begin")));
    iLog->Log(_L("CCommonTestClass::InternalizeMPXParameterL testing CMPXParameter::InternalizeMPXParameterL begin"));
    TInt err = KErrNone;

    if ( iParameter != NULL )
        {
    	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXParameterL started Internalize")));
        iLog->Log(_L("CCommonTestClass::InternalizeMPXParameterL started Internalize"));
        CBufBase* buffer = CBufFlat::NewL( 50 );
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        iParameter->ExternalizeL( writeStream );
        writeStream.CommitL();
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );

        RBufReadStream readStream( *buffer );
        CleanupClosePushL( readStream );                
        iParameter = new ( ELeave ) CMPXParameter();        
        iParameter->InternalizeL( readStream );
        CleanupStack::PopAndDestroy( &readStream );
        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXParameterL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::InternalizeMPXParameterL Stif test script is wrong."));
        }


	FTRACE(FPrint(_L("CCommonTestClass::InternalizeMPXParameterL testing CMPXParameter::InternalizeMPXParameterL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::InternalizeMPXParameterL testing CMPXParameter::InternalizeMPXParameterL end err=%d"), err);
	return err;
    }
    
// -----------------------------------------------------------------------------
// CCommonTestClass::ExternalizeMPXParameterL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ExternalizeMPXParameterL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXParameterL testing CMPXParameter::ExternalizeMPXParameterL begin")));
    iLog->Log(_L("CCommonTestClass::ExternalizeMPXParameterL testing CMPXParameter::ExternalizeMPXParameterL begin"));
    TInt err = KErrNone;
    if ( iParameter != NULL )
        {
        iParameter->iType.iUid = KMPXPluginTypePlaybackUid;
        iParameter->iCmdForward = EMPXCmdFwdNone;
    
    	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXParameterL started Externalize")));
        iLog->Log(_L("CCommonTestClass::ExternalizeMPXParameterL started Externalize"));
        CBufBase* buffer = CBufFlat::NewL( 50 );
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        iParameter->ExternalizeL( writeStream );
        writeStream.CommitL();
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );
        CleanupStack::PopAndDestroy( buffer ); 
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXParameterL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::ExternalizeMPXParameterL Stif test script is wrong."));
        }
	FTRACE(FPrint(_L("CCommonTestClass::ExternalizeMPXParameterL testing CMPXParameter::ExternalizeMPXParameterL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ExternalizeMPXParameterL testing CMPXParameter::ExternalizeMPXParameterL end err=%d"), err);
	return err;
    }


