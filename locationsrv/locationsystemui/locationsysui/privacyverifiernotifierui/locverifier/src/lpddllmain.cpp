/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Dll entry point and plugin creation functions.
*
*/


// INCLUDE FILES
#include "lpdverifierplugin.h"
#include <e32base.h>
#include <eiknotapi.h>
#include <ecom/implementationproxy.h>

// ============================= LOCAL FUNCTIONS ===============================

LOCAL_C void CreateNotifiersL( 
    CArrayPtrFlat<MEikSrvNotifierBase2>* aNotifiers )
    {
    CLpdVerifierPlugin* notifier = CLpdVerifierPlugin::NewL();
    CleanupStack::PushL( notifier );
    aNotifiers->AppendL( notifier->NotifierBase() );
    CleanupStack::Pop( notifier ); // Do not destroy. This is handled by the FW.
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

LOCAL_C CArrayPtr<MEikSrvNotifierBase2>* NotifierArray()
    {
    CArrayPtrFlat<MEikSrvNotifierBase2>* notifiers =
        new CArrayPtrFlat<MEikSrvNotifierBase2>( 1 );
    if ( notifiers )
        {
        TRAPD( err, CreateNotifiersL( notifiers ) );
        if ( err )
            { // release any notifiers we have created
            TInt count = notifiers->Count();
            while ( --count >= 0 )
                {
                ( *notifiers )[count]->Release();
                }
            delete notifiers;
            notifiers = NULL;
            }
        }
    return notifiers;
    }

const TImplementationProxy ImplementationTable[] =
	{
#ifdef __EABI__
	{{0x10207243},(TFuncPtr)NotifierArray}
#else
	{{0x10207243},NotifierArray}
#endif
	};

// ======== GLOBAL FUNCTIONS ========

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy) ;
	return ImplementationTable;
	}
	
// End of File
