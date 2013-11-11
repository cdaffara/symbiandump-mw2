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
* Description:  MPXViewPlugin testing implementation (mpxviewplugin.h)
*
*/


#include "commontestclass.h"

// Begin CMPXViewPlugin testing implementation (mpxviewplugin.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXViewPlugin()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXViewPlugin()
    {
//    FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXViewPlugin testing CMPXViewPlugin::NewL begin")));
//    iLog->Log(_L("CCommonTestClass::CreateCMPXViewPlugin testing CMPXViewPlugin::NewL begin"));
//    TInt err = KErrNone;
//    if ( iViewPlugin != NULL)
//        {
//        delete iViewPlugin;
//        }
//    iViewPlugin = NULL;
//
//    TUid aUid = TUid::Uid(270531770);
//
//    TRAP(err,iViewPlugin = CMPXViewPlugin::NewL( aUid, this ));
//    if ( iViewPlugin == NULL)
//        {
//        err = KErrNotFound;
//        }
//
//    FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXViewPlugin testing CMPXViewPlugin::NewL end err=%d"), err));
//    iLog->Log(_L("CCommonTestClass::CreateCMPXViewPlugin testing CMPXViewPlugin::NewL end err=%d"), err);
//    return err;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXViewPlugin()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCMPXViewPlugin()
    {
//    FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXViewPlugin testing CMPXViewPlugin::~ begin")));
//    iLog->Log(_L("CCommonTestClass::DeleteCMPXViewPlugin testing CMPXViewPlugin::~ begin"));
//    TInt err = KErrNone;
//    if ( iViewPlugin )
//        {
//        delete iViewPlugin;
//        REComSession::FinalClose();
//        iViewPlugin = NULL;
//        }
//    FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXViewPlugin testing CMPXViewPlugin::~ end err=%d"), err));
//    iLog->Log(_L("CCommonTestClass::DeleteCMPXViewPlugin testing CMPXViewPlugin::~ end err=%d"), err);
//    return err;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// From MMPXCustomCommandObserver.
// Handle custom commands.
// ---------------------------------------------------------------------------
//
void CCommonTestClass::HandleCustomCommand( 
    const TUid& /*aUid*/,
    const TInt /*aCommand*/,
    const TDesC8& /*aData*/ )
    {
	FTRACE(FPrint(_L("CCommonTestClass::HandleCustomCommand CMPXViewPlugin::HandleCustomCommand is called")));
    iLog->Log(_L("CCommonTestClass::HandleCustomCommand CMPXViewPlugin::HandleCustomCommand is called"));
    }


//end of CMPXViewPlugin (mpxviewplugin.h =============================================

