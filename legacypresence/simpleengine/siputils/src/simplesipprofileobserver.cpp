/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    sip connection
*
*/




// INCLUDE FILES
#include <sip.h>
#include <sipconnection.h>
#include <sipconnectionobserver.h>
#include <sipresponseelements.h>
#include <sipclienttransaction.h>
#include <sipprofile.h>
#include <sipprofileregistry.h>
#include "simplesipprofileobserver.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleSipProfileObserver::CSimpleSipProfileObserver
// ----------------------------------------------------------
//
CSimpleSipProfileObserver::CSimpleSipProfileObserver( CSimpleSipConnectionObserver& aObs )
: iObs(aObs)
    {
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::~CSimpleSipProfileObserver
// ----------------------------------------------------------
//
CSimpleSipProfileObserver::~CSimpleSipProfileObserver()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: DESTRUCTOR start" ));
#endif
    if ( iProfile && iRegistry )
        {
        iRegistry->Disable( *iProfile );
        }
    delete iProfile;
    delete iRegistry;
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: DESTRUCTOR end" ));
#endif
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::NewL
// ----------------------------------------------------------
//
CSimpleSipProfileObserver* CSimpleSipProfileObserver::NewL(
    CSIP* aSIP,
    CSimpleSipConnectionObserver& aObs )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: NewL" ));
#endif
    CSimpleSipProfileObserver* self = new (ELeave) CSimpleSipProfileObserver( aObs );
    CleanupStack::PushL( self );
    self->ConstructL( aSIP );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::ConstructL
// ----------------------------------------------------------
//
void CSimpleSipProfileObserver::ConstructL( CSIP* aSIP )
    {
    iRegistry = CSIPProfileRegistry::NewL( *aSIP, *this );
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::RegisterDefaultProfileL
// ----------------------------------------------------------
//
void CSimpleSipProfileObserver::RegisterDefaultProfileL( )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: RegisterDefaultProfileL" ));
#endif
    if ( iProfile )
        {
        User::Leave( KErrAlreadyExists );
        }

    // Get the default profile.
    iProfile = iRegistry->DefaultProfileL();

    // Safety check that DefaultProfileL() didn't return NULL pointer.
    if ( !iProfile )
        {
        User::Leave( KErrNotFound );
        }
    if ( !iRegistry->IsEnabled( *iProfile ))
        {
        // Ask Profile API to enable the retrieved profile for our use.
        iRegistry->EnableL( *iProfile, iObs );
        }
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::RegisterGivenProfileL
// ----------------------------------------------------------
//
void CSimpleSipProfileObserver::RegisterGivenProfileL( TUint32 aID )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: RegisterGivenProfileL id=%d" ), aID);
#endif

    RPointerArray<CSIPProfile> profiles;

    if ( iProfile )
        {
        User::Leave( KErrAlreadyExists );
        }

    // Search the profiles until the proper one is found.
    iRegistry->ProfilesL( profiles );

    TInt count = profiles.Count();
    TUint32 val = 0;
    CSIPProfile* sippro = NULL;
    for ( TInt i = 0; i < count; i++ )
        {
        sippro = profiles[i];
        TInt err = sippro->GetParameter( KSIPProfileId, val );
#ifdef _DEBUG
        TSimpleLogger::Log(_L("SipProfileObserver: Profile id=%d" ), val);
#endif
        if ( !err && val == aID)
            {
            // The profile matches for the given search criteria
            iProfile = sippro;
            }
        else
            {
            // delete unnecessary profile entity,
            // the ownership was transferred to us.
            delete sippro;
            }
        }

    // reset array, unnecessary profiles are already deleted
    profiles.Reset();

    // Safety check that DefaultProfileL() didn't return NULL pointer.
    if ( !iProfile )
        {
        User::Leave( KErrNotFound );
        }
    if ( !iRegistry->IsEnabled( *iProfile ))
        {
        // Ask Profile API to enable the retrieved profile for our use.
        iRegistry->EnableL( *iProfile, iObs );
        }

    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::GiveConnectionL
// ----------------------------------------------------------
//
CSIPConnection* CSimpleSipProfileObserver::GiveConnectionL()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: GiveConnectionL" ));
#endif
    // Check if the SIP connection is already enabled or should we
    // wait it for.
    TBool val( EFalse );
    TInt err = iProfile->GetParameter( KSIPProfileRegistered, val );
    User::LeaveIfError( err ); 
    // The pameter val indicates if the profile can be immediately used
    if ( val )
        {
        // get the SIP connection used by the profile
        return iRegistry->ConnectionL( *iProfile );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    return (CSIPConnection*) NULL;
    }
    
// ----------------------------------------------------------
// CSimpleSipProfileObserver::IsProfileActive
// ----------------------------------------------------------
//
TBool CSimpleSipProfileObserver::IsProfileActive()
    {
    // Check if the SIP profile is active
    // wait it for.
    TBool val( EFalse );
    // Profile is created in ConstructL, so it exists.
    if ( iProfile )
        {                
        iProfile->GetParameter( KSIPProfileRegistered, val );
        }
    return val;
    }    

// ----------------------------------------------------------
// CSimpleSipProfileObserver::GiveUserAorL
// ----------------------------------------------------------
//
TPtrC8 CSimpleSipProfileObserver::GiveUserAorL()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: GiveUserAorL" ));
#endif   
    // Check first if registered
    TBool val( EFalse );
    TInt err = iProfile->GetParameter( KSIPProfileRegistered, val );
    User::LeaveIfError( err );    
    if ( !val)
        {
        User::Leave( KErrNotReady );
        }
    // Get the first element in the array         
    const MDesC8Array* aors = 0;
    err=  iProfile->GetParameter( KSIPRegisteredAors, aors );    
    User::LeaveIfError(err);    
    if ( !aors || aors->MdcaCount() == 0 )
        {
        User::Leave( KErrNotReady );
        }
    return aors->MdcaPoint(0);
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::ProfileRegistryEventOccurred
// ----------------------------------------------------------
//
void CSimpleSipProfileObserver::ProfileRegistryEventOccurred(
    TUint32 /*aProfileId*/, TEvent aEvent)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: ProfileRegistryEventOccurred event=%d" ), aEvent );
#endif
    if ( aEvent == EProfileRegistered )
        {
        iObs.ProfileStateChanged( CSIPConnection::EActive, KErrNone );
        }
    else if ( aEvent == EProfileDeregistered )
        {
        iObs.ProfileStateChanged( CSIPConnection::EInactive, KErrNone );
        }     
    else if ( aEvent == EProfileUpdated )
        {
        iObs.ProfileStateChanged( CSIPConnection::EUnavailable, KErrNone );
        }           
    else if ( aEvent == EProfileDestroyed )
        {      
        iObs.ProfileStateChanged( CSIPConnection::EUnavailable, KErrNone );
        }
  }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::ProfileRegistryErrorOccurred
// ----------------------------------------------------------
//
void CSimpleSipProfileObserver::ProfileRegistryErrorOccurred(
    TUint32 /*aProfileId*/, TInt aError)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipProfileObserver: ProfileRegistryErrorOccurred" ));
#endif
    iObs.ProfileStateChanged( CSIPConnection::EUnavailable, aError );
    }

// ----------------------------------------------------------
// CSimpleSipProfileObserver::ProfileContext
// ----------------------------------------------------------
//
MSIPRegistrationContext* CSimpleSipProfileObserver::ProfileContext()
    {
    return iProfile;
    }
