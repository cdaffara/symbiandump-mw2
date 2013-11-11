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
* Description:  MPXDrmMediaUtility testing implementation (mpxdrmmediautility.h)
*
*/


#include <s32mem.h>
#include "commontestclass.h"
#include <mpxmediadrmdefs.h>


// Begin CMPXDrmMediaUtility testing implementation (mpxdrmmediautility.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXDrmMediaUtilityL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXDrmMediaUtilityL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewL() begin"));
    TInt err = KErrNone;
    if ( iDrmMediaUtility != NULL)
        {
        delete iDrmMediaUtility;
        }
    iDrmMediaUtility = NULL;
    iDrmMediaUtility = CMPXDrmMediaUtility::NewL();
    if ( iDrmMediaUtility == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXDrmMediaUtilityLC()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXDrmMediaUtilityLC()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewLC() begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewLC() begin"));
    TInt err = KErrNone;
    CMPXDrmMediaUtility* drmUtil = CMPXDrmMediaUtility::NewLC();
    if ( drmUtil == NULL)
        {
        err = KErrNotFound;
        }
    CleanupStack::PopAndDestroy( drmUtil );
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewLC() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXDrmMediaUtilityL testing CMPXDrmMediaUtility::NewLC() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXDrmMediaUtilityL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCMPXDrmMediaUtilityL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXDrmMediaUtilityL testing CMPXDrmMediaUtilityL::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXDrmMediaUtilityL testing CMPXDrmMediaUtilityL::~ begin"));
    TInt err = KErrNone;
    delete iDrmMediaUtility;
    iDrmMediaUtility = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXDrmMediaUtilityL testing CMPXDrmMediaUtilityL::~ end=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXDrmMediaUtilityL testing CMPXDrmMediaUtilityL::~ end=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::InitFileNameL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::InitFileNameL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::InitFileNameL testing CMPXDrmMediaUtility::InitL begin")));
    iLog->Log(_L("CCommonTestClass::InitFileNameL testing CMPXDrmMediaUtility::InitL begin"));
    TInt err = KErrNone;

    if ( iDrmMediaUtility != NULL )
        {
        iDrmMediaUtility->InitL(_L("c:\\testing\\data\\testdrm.cm"));
        }
    else
        {
        err = KErrBadTestParameter;
        }

	FTRACE(FPrint(_L("CCommonTestClass::InitFileNameL testing CMPXDrmMediaUtility::InitL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::InitFileNameL testing CMPXDrmMediaUtility::InitL end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::InitRFileL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::InitRFileL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::InitRFileL testing CMPXDrmMediaUtility::InitL begin")));
    iLog->Log(_L("CCommonTestClass::InitRFileL testing CMPXDrmMediaUtility::InitL begin"));
    TInt err = KErrNone;

    if ( iDrmMediaUtility != NULL )
        {
        RFile file;
#ifdef __WINSCW__
        _LIT(KFileName, "c:\\testing\\data\\testdrm.cm");
#else
        _LIT(KFileName, "e:\\testing\\data\\testdrm.cm");
#endif
        RFs fs;
        User::LeaveIfError(fs.Connect());
        file.Open(fs,KFileName,EFileStreamText|EFileShareAny);  
        iDrmMediaUtility->InitL(file);
        file.Close();  
        fs.Close();           
        }
    else
        {
        err = KErrBadTestParameter;
        }

	FTRACE(FPrint(_L("CCommonTestClass::InitRFileL testing CMPXDrmMediaUtility::InitL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::InitRFileL testing CMPXDrmMediaUtility::InitL end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::Close()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Close()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Close testing CMPXDrmMediaUtility::Close begin")));
    iLog->Log(_L("CCommonTestClass::Close testing CMPXDrmMediaUtility::Close begin"));
    TInt err = KErrNone;

    if ( iDrmMediaUtility != NULL )
        {
        iDrmMediaUtility->Close();
        }
    else
        {
        err = KErrBadTestParameter;
        }

	FTRACE(FPrint(_L("CCommonTestClass::Close testing CMPXDrmMediaUtility::Close end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Close testing CMPXDrmMediaUtility::Close end err=%d"), err);
	return err;
    }



// -----------------------------------------------------------------------------
// CCommonTestClass::GetMediaL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::GetMediaL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::GetMediaL testing CMPXDrmMediaUtility::GetMediaL begin")));
    iLog->Log(_L("CCommonTestClass::GetMediaL testing CMPXDrmMediaUtility::GetMediaL begin"));
    TInt err = KErrNone;

    if ( iDrmMediaUtility != NULL )
        {
        iDrmMediaUtility->InitL(_L("c:\\testing\\data\\testdrm.cm"));
        const CMPXMedia* media = iDrmMediaUtility->GetMediaL( EMPXMediaDrmProtected );            
        }
    else
        {
        err = KErrBadTestParameter;
        }

	FTRACE(FPrint(_L("CCommonTestClass::GetMediaL testing CMPXDrmMediaUtility::GetMediaL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::GetMediaL testing CMPXDrmMediaUtility::GetMediaL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::ConsumeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ConsumeL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ConsumeL testing CMPXDrmMediaUtility::ConsumeL begin")));
    iLog->Log(_L("CCommonTestClass::ConsumeL testing CMPXDrmMediaUtility::ConsumeL begin"));
    TInt err = KErrNone;

    if ( iDrmMediaUtility != NULL )
        {
        iDrmMediaUtility->InitL(_L("c:\\testing\\data\\testdrm.cm"));
        const CMPXMedia* media = iDrmMediaUtility->GetMediaL( EMPXMediaDrmProtected ); 
        iDrmMediaUtility->ConsumeL( EMPXDrmConsumeNone );
        }
    else
        {
        err = KErrBadTestParameter;
        }

	FTRACE(FPrint(_L("CCommonTestClass::ConsumeL testing CMPXDrmMediaUtility::ConsumeL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ConsumeL testing CMPXDrmMediaUtility::ConsumeL end err=%d"), err);
	return err;
    }
