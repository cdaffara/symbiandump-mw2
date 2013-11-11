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
* Description:  MPXCenRepWatcher testing implementation
*
*/


#include <mpxprivatecrkeys.h>
#include <mpxcenrepobserver.h>
#include <mpxcenrepwatcher.h>

#include "commontestclass.h"

// Begin CMPXCenRepWatcher testing implementation (mpxcenrepwatcher.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCenRepWatcherL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCenRepWatcherL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCenRepWatcherL testing CMPXCenRepWatcher::NewL begin")));
    iLog->Log(_L("CCommonTestClass::CreateCenRepWatcherL testing CMPXCenRepWatcher::NewL begin"));
    TInt err = KErrNone;
    if ( iCenRepWatcher != NULL)
        {
        delete iCenRepWatcher;
        }
    iCenRepWatcher = NULL;
    iCenRepWatcher = CMPXCenRepWatcher::NewL(KCRUidMPXSettings, 
                                             KMPXPlaybackVolume, 
                                             this);
    if ( iCenRepWatcher == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateCenRepWatcherL testing CMPXCenRepWatcher::NewL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCenRepWatcherL testing CMPXCenRepWatcher::NewL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteCenRepWatcherL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCenRepWatcherL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCenRepWatcherL testing CMPXCenRepWatcher::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCenRepWatcherL testing CMPXCenRepWatcher::~ begin"));
    TInt err = KErrNone;
    delete iCenRepWatcher;
    iCenRepWatcher = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCenRepWatcherL testing CMPXCenRepWatcher::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCenRepWatcherL testing CMPXCenRepWatcher::~ end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::HandleSettingChange()
// Returns: None
// -----------------------------------------------------------------------------
void CCommonTestClass::HandleSettingChange(const TUid& /*aRepositoryUid*/,
                                         TUint32 /*aSettingId*/)
    {
    }


TInt CCommonTestClass::CenRepCurrentValueL( CStifItemParser& aItem )
    {
	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL testing CMPXCenRepWatcher::CurrentValueL begin")));
    iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL testing CMPXCenRepWatcher::CurrentValueL begin"));
    TInt mode = 0;
    TRAPD(err, mode = iCenRepWatcher->CurrentValueL());

	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL current value = %d"), mode ));
    iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL current value = %d"), mode );

	TPtrC checkType;
	TPtrC operand;
	TInt  expectedvalue;

	if ( ( KErrNone == err) &&
	        (KErrNone == aItem.GetNextString(checkType) ) &&
        	(KErrNone == aItem.GetNextString(operand) ) &&
        	(KErrNone ==  aItem.GetNextInt(expectedvalue) ) )
    	{
		if (( operand == KTagCompareEqual) && 
		    ( checkType == KTagvalue ) )
    		{
			if ( mode == expectedvalue )
			    {
			    err = KErrNone;
            	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL current value matches expectation")));
                iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL current value matches expectation"));
			    }
			else
			    {
			    err = KErrGeneral;
            	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL current value does not matche expectation")));
                iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL current value does not matche expectation"));
			    }
	    	}
	    else
	        {
            err = KErrBadTestParameter;
        	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL Stif test script is wrong.")));
            iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL Stif test script is wrong."));
	        }
    	}
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL Stif test script is wrong."));
        }


	FTRACE(FPrint(_L("CCommonTestClass::CenRepCurrentValueL testing CMPXCenRepWatcher::CurrentValueL end=%d"), err));
    iLog->Log(_L("CCommonTestClass::CenRepCurrentValueL testing CMPXCenRepWatcher::CurrentValueL end=%d"), err);

    return err;
    }


TInt CCommonTestClass::CenRepSetValueL( CStifItemParser& aItem )
    {
	FTRACE(FPrint(_L("CCommonTestClass::CenRepSetValueL testing CMPXCenRepWatcher::SetValueL begin")));
    iLog->Log(_L("CCommonTestClass::CenRepSetValueL testing CMPXCenRepWatcher::SetValueL begin"));

    TInt mode = 0;
    TRAPD(err, mode = iCenRepWatcher->CurrentValueL());

	FTRACE(FPrint(_L("CCommonTestClass::CenRepSetValueL current value = %d"), mode ));
    iLog->Log(_L("CCommonTestClass::CenRepSetValueL current value = %d"), mode );

	TInt newValue;
    if ( !aItem.GetNextInt(newValue) )
        {
        TRAP(err, iCenRepWatcher->SetValueL(newValue));
        
        if ( ( mode = iCenRepWatcher->CurrentValueL()) != newValue )
            {
            err = KErrGeneral;
            }
    
    	FTRACE(FPrint(_L("CCommonTestClass::CenRepSetValueL after setting value = %d"), mode ));
        iLog->Log(_L("CCommonTestClass::CenRepSetValueL after setting value = %d"), mode );
        }
    else
        {
    	FTRACE(FPrint(_L("CCommonTestClass::CenRepSetValueL failed reading parameter from STIF")));
        iLog->Log(_L("CCommonTestClass::CenRepSetValueL failed reading parameter from STIF"));
        err = KErrBadTestParameter;
        }
    
	FTRACE(FPrint(_L("CCommonTestClass::CenRepSetValueL testing CMPXCenRepWatcher::CenRepSetValueL~ end=%d"), err));
    iLog->Log(_L("CCommonTestClass::CenRepSetValueL testing CMPXCenRepWatcher::CenRepSetValueL end=%d"), err);
    return err;
    }

// end of CMPXCenRepWatcher (mpxcenrepwatcher.h =============================================
