/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Interface class for SUPL notifier polymorphic DLL
*
*/



// User Includes
#include "epos_comasuplhomenotifier.h"
#include "epos_comasuplroamingnotifier.h"
#include "epos_comasupltimeoutnotifier.h"

// System Includes
#include <eikenv.h>

// Constant Definitions

// CONSTANTS
const TInt KSuplNotifierArrayIncrement = 3;


// ----------------------- Local Functions -----------------------------------

// ---------------------------------------------------------
// LOCAL_C void CreateSuplNotifiersL
// Instantiates all the SUPL notifiers
// ---------------------------------------------------------
LOCAL_C void CreateSuplNotifiersL( CArrayPtrFlat< MEikSrvNotifierBase2 >* aNotifiers )
    {
    // Create the SUPL notifier for the home network
    CSuplHomeNotifier* homeNotifier = CSuplHomeNotifier::NewL();
    CleanupStack::PushL( homeNotifier );
    aNotifiers->AppendL( homeNotifier );
    CleanupStack::Pop( homeNotifier );
    
    // Create the SUPL notifier for roaming
    CSuplRoamingNotifier* roamingNotifier = CSuplRoamingNotifier::NewL();
    CleanupStack::PushL( roamingNotifier );
    aNotifiers->AppendL( roamingNotifier );
    CleanupStack::Pop( roamingNotifier );    
    
    // Create the SUPL notifier for connection timeout
    CSuplTimeOutNotifier* timeoutNotifier = CSuplTimeOutNotifier::NewL();
    CleanupStack::PushL( timeoutNotifier );
    aNotifiers->AppendL( timeoutNotifier );
    CleanupStack::Pop( timeoutNotifier );  
    }

// ------------------------- Global Functions -----------------------------------

// ---------------------------------------------------------
// EXPORT_C CArrayPtr< MEikSrvNotifierBase2 >* NotifierArray
// Polymorphic DLL entry point
// ---------------------------------------------------------

EXPORT_C CArrayPtr< MEikSrvNotifierBase2 >* NotifierArray()
    {
    CArrayPtrFlat< MEikSrvNotifierBase2 >* notifiers = NULL;
	TRAPD( err, notifiers = 
	           new (ELeave)CArrayPtrFlat< MEikSrvNotifierBase2 >( KSuplNotifierArrayIncrement ));
	if ( !err && notifiers )
    	{
    	TRAPD( err, CreateSuplNotifiersL( notifiers ));
    	if( err )
        	{
        	TInt count = notifiers->Count();
        	while( count-- )
        	    {
        	    ( *notifiers )[ count ]->Release();
        	    }           	
        	delete notifiers;
        	notifiers = NULL;
        	}   	    
    	}           
    return notifiers;
    }
