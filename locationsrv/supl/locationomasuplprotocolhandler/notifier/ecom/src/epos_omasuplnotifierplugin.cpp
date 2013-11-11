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
* Description:   SUPL Notifiers Entry functions and ECom implementation table
*                definitions
*
*/



// System Includes
#include <ecom/implementationproxy.h>
#include <AknNotifierWrapper.h>

//User Includes
#include "epos_omasuplnotifierpluginuid.hrh"
#include "epos_omasuplnotifierinterface.h"

const MEikSrvNotifierBase2::TNotifierPriority KNotifierPriority = 
                                        MEikSrvNotifierBase2::ENotifierPriorityHigh;
                                        
// Constant Definitions
const TInt KMaxSynchReplyBufLength = 256;                                        
	
// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Helper method to perform cleanup of the CArrayPtrFlat array object.
// ---------------------------------------------------------------------------
//
void CleanupArray( TAny*        aArray )
    {
    CArrayPtrFlat< MEikSrvNotifierBase2 >* subjects = 
            static_cast< CArrayPtrFlat< MEikSrvNotifierBase2 >* >( aArray );
    TInt lastInd = subjects->Count() - 1;
    for ( TInt i = lastInd; i >= 0; i-- )
        {
        subjects->At( i )->Release();
        }    	    
    delete subjects;
    }

// ---------------------------------------------------------------------------
// Creates an array of Notifer objects
// ---------------------------------------------------------------------------
//
CArrayPtr< MEikSrvNotifierBase2 >* DoCreateNotifierArrayL()
    {
    // Create an array of Notifier objects
    CArrayPtrFlat< MEikSrvNotifierBase2 >* subjects =
                    new ( ELeave )CArrayPtrFlat< MEikSrvNotifierBase2 >( 1 );    
    CleanupStack::PushL( TCleanupItem( CleanupArray, subjects ));

    CAknCommonNotifierWrapper* master = 
        CAknCommonNotifierWrapper::NewL( 
                                    KSuplHomeNotifier,
                                    KSuplHomeNotifierChannel,
                                    KNotifierPriority,
                                    _L("epos_omasuplnotifier.dll"),
                                   KMaxSynchReplyBufLength);
	  		   
    subjects->AppendL( master );
    
    // Notifiers using masters session 
    subjects->AppendL( new ( ELeave ) CAknNotifierWrapperLight( *master, 
                                                                KSuplRoamingNotifier,
                                   			 				    KSuplRoamingNotifierChannel,
                                   			 				    KNotifierPriority ));
                                   			 				
    // Notifiers using masters session 
    subjects->AppendL( new ( ELeave ) CAknNotifierWrapperLight( *master, 
                                                                KSuplTimeoutNotifier,
                                   			 				    KSuplTimeoutNotifierChannel,
                                   			 				    KNotifierPriority ));
    
    CleanupStack::Pop( subjects );
    return( subjects );
    }

// ---------------------------------------------------------------------------
// ECOM plugin instantiation function
// ---------------------------------------------------------------------------
//
CArrayPtr< MEikSrvNotifierBase2 >* NotifierArray()
    {
    CArrayPtr< MEikSrvNotifierBase2 >* array = NULL;
    TRAPD( error, array = DoCreateNotifierArrayL());
    if ( error )
        {
        return NULL;
        }
    return array;
    }

/**
 * SUPL Notifier ECom implementation table.
 */
const TImplementationProxy KOmaSuplPosImplTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( KOmaSuplNotPluginImplUid, NotifierArray )
	};

// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
	{
	aTableCount = sizeof( KOmaSuplPosImplTable ) / sizeof( TImplementationProxy );
	return KOmaSuplPosImplTable;
	}
