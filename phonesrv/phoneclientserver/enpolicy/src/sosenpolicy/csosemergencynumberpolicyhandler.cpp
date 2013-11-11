/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides SOS emergency number policy API implementation.
*
*/



// INCLUDE FILES
#include    "csosemergencynumberpolicyhandler.h" 
#include    <mmtsy_names.h>
#include    <featmgr.h>
#include    <bldvariant.hrh>
#include    <telservicesvariant.hrh>
#include    <telservicesinternalcrkeys.h>
#include    <centralrepository.h>
#include "csosenpolicysatrefresh.h" 

// CONSTANTS
// The default emergency numbers.
_LIT( KEnPolicyUnivEmergencyNumber, "112" );
_LIT( KEnPolicyUSEmergencyNumber, "911" );      // U.S.A
_LIT( KEnPolicyUKEmergencyNumber, "999" );      // U.K
_LIT( KEnPolicyAUEmergencyNumber, "000" );      // Australian
_LIT( KEnPolicyMXEmergencyNumber, "08" );       // Mexico
_LIT( KEnPolicyAPACEmergencyNumber, "110" );    // APAC countries, Germany
_LIT( KEnPolicyJapanEmergencyNumber, "118" );   // Japan
_LIT( KEnPolicyJapanEmergencyNumber2, "119" );  // Japan

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSosEnPolicyActive::CSosEnPolicyActive()
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CSosEnPolicyActive::CSosEnPolicyActive( 
    CSosEmergencyNumberPolicyHandler& aEmergNumHandler )
    : CActive( CActive::EPriorityStandard ), iEmergNumHandler ( aEmergNumHandler )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyActive::~CSosEnPolicyActive()
// -----------------------------------------------------------------------------
//
CSosEnPolicyActive::~CSosEnPolicyActive()
    {    
    Cancel();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyActive::RunL()
// 
// -----------------------------------------------------------------------------
//
void CSosEnPolicyActive::RunL()
    {
    iEmergNumHandler.CpsssmHandleSimStatusEvent( 
        CSosEmergencyNumberPolicyHandler::ESosEnPolicySimEmergencyNumberRead );
    }

// -----------------------------------------------------------------------------
// CSosEnPolicyActive::DoCancel()
// 
// -----------------------------------------------------------------------------
//
void CSosEnPolicyActive::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::CSosEmergencyNumberPolicyHandler()
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CSosEmergencyNumberPolicyHandler::CSosEmergencyNumberPolicyHandler() 
    : CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyImp::NewL()
// 
// -----------------------------------------------------------------------------
//
CSosEmergencyNumberPolicyHandler* CSosEmergencyNumberPolicyHandler::NewL( 
    RTelServer& aETelServer )
    {
    CSosEmergencyNumberPolicyHandler* self = 
        new(ELeave) CSosEmergencyNumberPolicyHandler();
    CleanupStack::PushL( self );
    self->ConstructL( aETelServer );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::~CSosEmergencyNumberPolicyHandler()
// -----------------------------------------------------------------------------
//
CSosEmergencyNumberPolicyHandler::~CSosEmergencyNumberPolicyHandler()
    {    
    Cancel(); // also deletes requester if there is one
    
    delete iSatRefresh;
    // both cancel operations in destructors.
    delete iSimMonitor;
    delete iENListHandler;

    iENStore.Close();

    FeatureManager::UnInitializeLib();
    iMobilePhone.Close();
    
    delete iCenRepSession;
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::ConstructL()
// -----------------------------------------------------------------------------
//
void CSosEmergencyNumberPolicyHandler::ConstructL( RTelServer& aETelServer )
    {
    FeatureManager::InitializeLibL();

    User::LeaveIfError( iMobilePhone.Open( aETelServer , KMmTsyPhoneName ) );

    // Create sat refresh listener
    iSatRefresh = CSosEnPolicySatRefresh::NewL( *this );
    
    iSimMonitor = CSosEnPolicySimStatusMonitor::NewL( *this );

    iENListHandler =  CSosEnPolicyENListHandler::NewL() ;

    User::LeaveIfError( iENStore.Open( iMobilePhone ) );

    InitializeCentralRepositoryList();
    }


// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::IsEmergencyNumberL 
// 
// -----------------------------------------------------------------------------
//
TInt CSosEmergencyNumberPolicyHandler::IsEmergencyNumber ( 
                                    TPhCltTelephoneNumber& aTelNumber, 
                                    TPhCltEmergencyNumberQueryMode aMode,
                                    TBool& aIsEmergencyNumber )
                                  
    {
    if ( aMode == EPhCltEmergencyNumberQueryModeAdvanced )
        {
        aIsEmergencyNumber = iENListHandler->IsInListByAdvancedMode( aTelNumber );
        }
    else
        {
        aIsEmergencyNumber = iENListHandler->IsInListByNormalMode( aTelNumber ); 
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::CpsssmHandleSimStatusEvent()
// 
// SIM event occurred, it's either for adding or removing
// SIM emergency numbers. If SIM is rejected or removed
// SIM emergency numbers are removed.
//
// Old list is cleared only if new list is received. Otherwise
// it's stays valid.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void  CSosEmergencyNumberPolicyHandler::CpsssmHandleSimStatusEvent(
    TSosEnPolicySimEmergencyNumberAction aAction )
    {
    // If not null there is something ongoing which should be cancelled first.
    if ( iSimEnRetriver )
        {
        Cancel(); // retriever is also deleted.
        }
    
    // We need to read new numbers.
    if ( aAction == ESosEnPolicySimEmergencyNumberRead )
        {
        TInt err = KErrNone;

        // _DPRINT( 4, "ENPolicy.EN.SimNumbers.create" );          // debug print

        // The retrieve may fail, that possibility is TRAPed here.
        TRAP( err, iSimEnRetriver = 
            CRetrieveMobilePhoneENList::NewL( iENStore ) );

        // _DDPRINT( 4, "ENPolicy.EN.SimNumbers.create.ok.", err ); // debug print

        if( err == KErrNone )
            {
            iSimEnRetriver->Start( iStatus );
            SetActive();
            }
        }
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::DoCancel()
// 
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CSosEmergencyNumberPolicyHandler::DoCancel()
    {
    if ( iSimEnRetriver )
        {
        iSimEnRetriver->Cancel();
        delete iSimEnRetriver;
        iSimEnRetriver = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::RunL()
// 
// Number fetch complete. In case of error we stick to that
// what we have, or use default list.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CSosEmergencyNumberPolicyHandler::RunL()
    {
    if ( iStatus.Int() == KErrNone )
        {
        // ownership is transferred!
        CMobilePhoneENList* list = iSimEnRetriver->RetrieveListL();
        CleanupStack::PushL( list );
        if ( list )
            {
            TInt count = list->Enumerate();

            // _DDPRINT( 4, "ENPolicy.EN.SimNumber.count.", count ); // debug print

            // clear old changeable stuff from list.
            iENListHandler->ClearCurrentList();
            for ( TInt i = 0; i < count; i++ )
                {
                const RMobileENStore::TMobileENEntryV1 entry = 
                    list->GetEntryL( i );

                // _DDPRINT( 4, "ENPolicy.EN.SimNumber.number.", entry.iNumber );

                iENListHandler->AddToList( entry.iNumber, EFalse );  
                }
            }
        CleanupStack::PopAndDestroy(); // list
        }
    delete iSimEnRetriver;
    iSimEnRetriver = NULL;
    }


// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::RunError
//
// Called when RunL leaves. 
// The RunL can leave in EN list operations. It is ok to return KErrNone after
// deleting the SimEnRetriever.
//
// -----------------------------------------------------------------------------
//
TInt CSosEmergencyNumberPolicyHandler::RunError( TInt /*aError*/ )
    {
    // delete the SimEnRetriever, because not done in RunL.
    delete iSimEnRetriver;
    iSimEnRetriver = NULL;

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyHandler::InitializeCentralRepositoryList()
// 
// Called once in the startup.
// Adds universal (112) emergency number to Central Repository list,
// and checks possible product specific values. After this,
// these values are not changed.
// -----------------------------------------------------------------------------
//
void CSosEmergencyNumberPolicyHandler::InitializeCentralRepositoryList()
    {
    // (From 3GPP specification 22.101, ftp://ftp.3gpp.org/specs/latest/Rel-8/22_series/22101-880.zip)
    //
    // "The ME shall identify a number dialled by the end user as a valid emergency number if it occurs under one or more 
    //  of the following conditions:
    //      a) 112 and 911 shall always be available. These numbers shall be stored on the ME.
    //      b) Any emergency number stored on a SIM/USIM when the SIM/USIM is present.
    //      c) 000, 08, 110, 999, 118 and 119 when a SIM/USIM is not present. These numbers shall be stored on the ME.
    //      d) Additional emergency numbers that may have been downloaded by the serving network when the SIM/USIM is present."
    
    // Universal, 112
    iENListHandler->AddToList( KEnPolicyUnivEmergencyNumber, ETrue );
    // U.S, 911
    iENListHandler->AddToList( KEnPolicyUSEmergencyNumber, ETrue );

    if ( iSimMonitor->IsReadingAllowed() )
        {
        CpsssmHandleSimStatusEvent( ESosEnPolicySimEmergencyNumberRead );
        }
    else
        {
        iENListHandler->AddToList( KEnPolicyUKEmergencyNumber );
        iENListHandler->AddToList( KEnPolicyAUEmergencyNumber );
        iENListHandler->AddToList( KEnPolicyMXEmergencyNumber );
        iENListHandler->AddToList( KEnPolicyAPACEmergencyNumber );
        iENListHandler->AddToList( KEnPolicyJapanEmergencyNumber );
        iENListHandler->AddToList( KEnPolicyJapanEmergencyNumber2 ); 
        }

    // Product specific numbers.
    iENListHandler->AddToList( iENListHandler->ReadProductSpecificNumbers(), ETrue );
    }

// End of file.
