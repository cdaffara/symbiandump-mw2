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
* Description:  MPXPSKeyWatcher testing implementation (mpxcollectiontype.h)
*
*/


#include <s32mem.h>
#include "commontestclass.h"

// Begin CMPXPSKeyWatcher testing implementation (mpxcollectiontype.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXPSKeyWatcherL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXPSKeyWatcherL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXPSKeyWatcherL testing CMPXPSKeyWatcher::CMPXPSKeyWatcher() begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXPSKeyWatcherL testing CMPXPSKeyWatcher::CMPXPSKeyWatcher() begin"));
    TInt err = KErrNone;
    if ( iPSWatcher != NULL)
        {
        delete iPSWatcher;
        }
    iPSWatcher = NULL;
    TUid  KMmcEjectAppUid = { 0x101FFAFC };
    TUint KKeyMmcEjectOperation = 10;
    iPSWatcher = CMPXPSKeyWatcher::NewL(KMmcEjectAppUid, 
                                            KKeyMmcEjectOperation,
                                            this );
    if ( iPSWatcher == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXPSKeyWatcherL testing CMPXPSKeyWatcher::CMPXPSKeyWatcher() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXPSKeyWatcherL testing CMPXPSKeyWatcher::CMPXPSKeyWatcher() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXPSKeyWatcherL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCMPXPSKeyWatcherL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXPSKeyWatcherL testing CMPXPSKeyWatcherL::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXPSKeyWatcherL testing CMPXPSKeyWatcherL::~ begin"));
    TInt err = KErrNone;
    delete iPSWatcher;
    iPSWatcher = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXPSKeyWatcherL testing CMPXPSKeyWatcherL::~ end=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXPSKeyWatcherL testing CMPXPSKeyWatcherL::~ end=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::GetTntValue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::GetTntValue()
    {
	FTRACE(FPrint(_L("CCommonTestClass::GetTntValue testing CMPXPSKeyWatcher::GetValue begin")));
    iLog->Log(_L("CCommonTestClass::GetTntValue testing CMPXPSKeyWatcher::GetValue begin"));
    TInt err = KErrNone;

    if ( iPSWatcher != NULL )
        {
        TInt value;
        iPSWatcher->GetValue( value );
    	FTRACE(FPrint(_L("CCommonTestClass::GetTntValue %d"), value));
        iLog->Log(_L("CCommonTestClass::GetTntValue %d"), value);
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::GetTntValue Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::GetTntValue Stif test script is wrong."));
        }


	FTRACE(FPrint(_L("CCommonTestClass::GetTntValue testing CMPXPSKeyWatcher::GetValue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::GetTntValue testing CMPXPSKeyWatcher::GetValue end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::GetDes8Value()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::GetDes8Value()
    {
	FTRACE(FPrint(_L("CCommonTestClass::GetDes8Value testing CMPXPSKeyWatcher::GetValue begin")));
    iLog->Log(_L("CCommonTestClass::GetDes8Value testing CMPXPSKeyWatcher::GetValue begin"));
    TInt err = KErrNone;

    if ( iPSWatcher != NULL )
        {
        TBuf8<20> value;
        iPSWatcher->GetValue( value );            
    	FTRACE(FPrint(_L("CCommonTestClass::GetDes8Value")));
        iLog->Log(_L("CCommonTestClass::GetDes8Value"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::GetDes8Value Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::GetDes8Value Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::GetDes8Value testing CMPXPSKeyWatcher::GetValue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::GetDes8Value testing CMPXPSKeyWatcher::GetValue end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::GetDes8Value()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::GetDes16Value()
    {
	FTRACE(FPrint(_L("CCommonTestClass::GetDes16Value testing CMPXPSKeyWatcher::GetValue begin")));
    iLog->Log(_L("CCommonTestClass::GetDes16Value testing CMPXPSKeyWatcher::GetValue begin"));
    TInt err = KErrNone;

    if ( iPSWatcher != NULL )
        {
        TBuf16<20> value;
        iPSWatcher->GetValue( value );            
    	FTRACE(FPrint(_L("CCommonTestClass::GetDes16Value")));
        iLog->Log(_L("CCommonTestClass::GetDes16Value"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::GetDes16Value Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::GetDes16Value Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::GetDes16Value testing CMPXPSKeyWatcher::GetValue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::GetDes16Value testing CMPXPSKeyWatcher::GetValue end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::SetTntValue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SetTntValue()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SetTntValue testing CMPXPSKeyWatcher::SetValue begin")));
    iLog->Log(_L("CCommonTestClass::SetTntValue testing CMPXPSKeyWatcher::SetValue begin"));
    TInt err = KErrNone;

    if ( iPSWatcher != NULL )
        {
        TInt value = 2;
        iPSWatcher->SetValue( value );
    	FTRACE(FPrint(_L("CCommonTestClass::SetTntValue")));
        iLog->Log(_L("CCommonTestClass::SetTntValue"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::SetTntValue Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::SetTntValue Stif test script is wrong."));
        }


	FTRACE(FPrint(_L("CCommonTestClass::SetTntValue testing CMPXPSKeyWatcher::SetValue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SetTntValue testing CMPXPSKeyWatcher::SetValue end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::SetDes8Value()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SetDes8Value()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SetDes8Value testing CMPXPSKeyWatcher::SetValue begin")));
    iLog->Log(_L("CCommonTestClass::SetDes8Value testing CMPXPSKeyWatcher::SetValue begin"));
    TInt err = KErrNone;

    if ( iPSWatcher != NULL )
        {
        TBuf8<20> value = _L8("test");
        iPSWatcher->SetValue( value );            
    	FTRACE(FPrint(_L("CCommonTestClass::SetDes8Value")));
        iLog->Log(_L("CCommonTestClass::SetDes8Value"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::SetDes8Value Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::SetDes8Value Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::SetDes8Value testing CMPXPSKeyWatcher::SetValue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SetDes8Value testing CMPXPSKeyWatcher::SetValue end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::SetDes16Value()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SetDes16Value()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SetDes16Value testing CMPXPSKeyWatcher::SetValue begin")));
    iLog->Log(_L("CCommonTestClass::SetDes16Value testing CMPXPSKeyWatcher::SetValue begin"));
    TInt err = KErrNone;

    if ( iPSWatcher != NULL )
        {
        TBuf16<20> value = _L("test");
        iPSWatcher->SetValue( value );            
    	FTRACE(FPrint(_L("CCommonTestClass::SetDes16Value")));
        iLog->Log(_L("CCommonTestClass::SetDes16Value"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::SetDes16Value Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::SetDes16Value Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::SetDes16Value testing CMPXPSKeyWatcher::SetValue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SetDes16Value testing CMPXPSKeyWatcher::SetValue end err=%d"), err);
	return err;
    }

    
void CCommonTestClass::HandlePSEvent(TUid /*aUid*/, TInt /*aKey*/)
    {
    }
