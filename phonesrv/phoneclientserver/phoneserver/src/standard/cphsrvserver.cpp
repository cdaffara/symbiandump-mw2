/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Server.
*
*/


// INCLUDE FILES
#include "cphsrvserver.h" 
#include "cphsrvsession.h" 
#include "cphsrvscheduler.h" 
#include "cphsrvussdmanager.h" 
#include "cphsrvresourcemanager.h" 
#include "cphsrvphonecontroller.h" 
#include "cphsrvcallrequestmanager.h" 
#include "cphsrvemergencyrequestmanager.h" 
#include "cphsrvemergencynumbermanager.h" 

#include "phsrvutils.h" 
#include "kphsrvconfigure.h" // debugprints ON/OFF 
#include "phsrvdebuginfo.h" 

#include "mphsrvcomhandnegotiators.h" 
#include "cphsrvcomhandrequestmanager.h" 
#include "mphsrvmessengernegotiators.h" 
#include "cphsrvmessengerrequestmanager.h" 
#include <featmgr.h>



// CONSTANTS
const TInt KPhSrvServerPriority = 100;  // phone server priority


// The create function enumerations.
enum 
    {
    ECreateIdle,             // Create idle.
    ECreateEmergencyNegotiator, // Create Emergency Call negotiator
    ECreateCallNegotiator,   // Create Call Negotiator.
    ECreateUssdManager,      // Create USSD Manager.
    ECreatePhoneInterface,   // Create Phone Interface.
    ECreateEmergencyNumber,  // Create Emergency Number Manager.
    ECreateComHandManager,   // Create command handler manager.
    ECreateMessengerManager  // Create Messenger manager.
    };


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvServer::New
// 
// Create and start a new server.
// -----------------------------------------------------------------------------
//
CPhSrvServer* CPhSrvServer::NewL()
    {
    // Create
    CPhSrvServer* self = new ( ELeave ) CPhSrvServer( KPhSrvServerPriority );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::CPhSrvServer
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CPhSrvServer::CPhSrvServer( TInt  aPriority )
    : CServer2( aPriority ),
    iFBSConnections( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::CPhSrvServer
// 
// C++ Destructor
// -----------------------------------------------------------------------------
//
CPhSrvServer::~CPhSrvServer()
    {
    CompleteCreateAll( KErrCancel );

    delete iCreateAll;
    delete iObjectConIx;
    delete iUssdManager;
    delete iEmergencyNumberManager;
    delete iResourceFileManager;
    delete iCallRequestManager;
    delete iPhoneController;
    delete iComHandRequestManager;
    delete iMessengerRequestManager;

    iFsSession.Close();
    DisconnectFBS( ETrue );


    // Frees the TLS! Must be done after FeatureManager is used.
    FeatureManager::UnInitializeLib();
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhSrvServer::ConstructL()
    {
    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();


    // Start server
    TInt ret = Start( PHONE_SERVER_NAME );
    __ASSERT_ALWAYS( ret == KErrNone || ret == KErrAlreadyExists, 
        PhoneServerUtils::Panic( EPhoneServerPanicCouldNotStartServer ) );
    User::LeaveIfError( ret );
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::NewSessionL
// 
// Create a new session.
// -----------------------------------------------------------------------------
//
CSession2* CPhSrvServer::NewSessionL( 
    const TVersion& aVersion,
    const RMessage2& /*aMessage*/ ) const 
    {
    // Check the right version
    TVersion v( KPhoneServerMajorVersionNumber,
                KPhoneServerMinorVersionNumber,
                KPhoneServerBuildVersionNumber );

    if( !User::QueryVersionSupported( v, aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    // Create a new session
    CSession2* session = CPhSrvSession::NewL( 
        *const_cast< CPhSrvServer* >( this ) );

    return session;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::CreateAllL
// -----------------------------------------------------------------------------
//
void CPhSrvServer::CreateAllL( CPhSrvSession& aSession )
    {
    if ( !iCreateAll )
        {
        iCreateAll = CIdle::NewL( CActive::EPriorityStandard );
        }

    User::LeaveIfError( iCreateAllArray.Append( &aSession ) );

    if ( !iCreateAll->IsActive() )
        {
        iCreateAllStep = ECreateIdle;
        iCreateAllError = KErrNone;

        iCreateAll->Start( TCallBack( DoCreateAllStepL, this ) );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::CancelCreateAll
// -----------------------------------------------------------------------------
//
void CPhSrvServer::CancelCreateAll( CPhSrvSession& aSession )
    {
    TInt pos = iCreateAllArray.Find( &aSession );
    if ( pos != KErrNotFound )
        {
        iCreateAllArray.Remove( pos );
        aSession.CompleteCreateAll( KErrCancel );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::NewContainerL
// 
// Create a new object container for session.
// -----------------------------------------------------------------------------
//
CObjectCon* CPhSrvServer::NewContainerL() const
    {
    if ( !iObjectConIx )
        {
        iObjectConIx = CObjectConIx::NewL();
        }

    return iObjectConIx->CreateL();
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::RemoveContainer
// 
// Remove object container.
// -----------------------------------------------------------------------------
//
void  CPhSrvServer::RemoveContainer( CObjectCon* aCon )
    {
    iObjectConIx->Remove( aCon );
    }
    
// -----------------------------------------------------------------------------
// CPhSrvServer::CallNegotiatorL
// 
// Access the call negotiator
// -----------------------------------------------------------------------------
//
MPhSrvNegotiatorCall& CPhSrvServer::CallNegotiatorL()
    {
    if ( !iCallRequestManager )
        {
        CPhSrvCallRequestManager* self = 
            new (ELeave) CPhSrvCallRequestManager();
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );

        iCallRequestManager = self;
        }
    
    return *iCallRequestManager;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::CallNegotiator
// 
// Access the call negotiator
// -----------------------------------------------------------------------------
//
MPhSrvNegotiatorCall& CPhSrvServer::CallNegotiator() const
    {
    __ASSERT_DEBUG( IsCallNegotiator(), 
        PhoneServerUtils::Panic( EPhoneServerPanicNoCallInitiatorFault ) );
    return *iCallRequestManager;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::EmergencyCallNegotiatorL
// 
// Access the emergency call negotiator
// -----------------------------------------------------------------------------
//
MPhSrvEmergencyNegotiatorCall& CPhSrvServer::EmergencyCallNegotiatorL()
    {
    if ( !iEmergencyRequestManager )
        {
        CPhSrvEmergencyRequestManager* self = 
            new (ELeave) CPhSrvEmergencyRequestManager();
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );

        iEmergencyRequestManager = self;
        }
    
    return *iEmergencyRequestManager;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::EmergencyCallNegotiatorL
// 
// Access the emergency call negotiator
// -----------------------------------------------------------------------------
//
MPhSrvEmergencyNegotiatorCall& CPhSrvServer::EmergencyCallNegotiator() const
    {
    __ASSERT_DEBUG( IsCallNegotiator(), 
        PhoneServerUtils::Panic( EPhoneServerPanicMissingEmerNegotiator ) );
    return *iEmergencyRequestManager;
    }
    
// -----------------------------------------------------------------------------
// CPhSrvServer::IsEmergencyCallNegotiator
// -----------------------------------------------------------------------------
//
TBool CPhSrvServer::IsEmergencyCallNegotiator() const
    {
    return ( iEmergencyRequestManager != NULL );
    }
  
// -----------------------------------------------------------------------------
// CPhSrvServer::UssdManagerL
// -----------------------------------------------------------------------------
//
CPhSrvUssdManager& CPhSrvServer::UssdManagerL()
    {
    if ( !iUssdManager )
        {
        CPhSrvUssdManager* self = 
            new ( ELeave ) CPhSrvUssdManager( 
                FsSessionL(), 
                ResourceManagerL()
               );

        CleanupStack::PushL( self );
        self->ConstructL( PhoneInterfaceL() );
        CleanupStack::Pop( self );

        iUssdManager = self;
        }
   
    return *iUssdManager;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::PhoneInterfaceL
// -----------------------------------------------------------------------------
//
MPhSrvPhoneInterface& CPhSrvServer::PhoneInterfaceL()
    {
    if ( !iPhoneController )
        {
        CPhSrvPhoneController* self = 
            new( ELeave ) CPhSrvPhoneController();
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );

        iPhoneController = self;
        }

    return *iPhoneController;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::EmergencyNumberManagerL
// -----------------------------------------------------------------------------
//
MPhSrvEmergencyNumberManager& CPhSrvServer::EmergencyNumberManagerL()
    {
    if ( !iEmergencyNumberManager )
        {
        PhoneInterfaceL(); // if succeeds iPhoneController ok after this
        
        CPhSrvEmergencyNumberManager* self =
            new ( ELeave ) CPhSrvEmergencyNumberManager( *iPhoneController );

        TBool isDosEnPolicy = 
            FeatureManager::FeatureSupported( KFeatureIdEnPolicyDos );            
        CleanupStack::PushL( self );
        self->ConstructL( isDosEnPolicy );
        CleanupStack::Pop( self );

        iEmergencyNumberManager = self;    
        }
    
    return *iEmergencyNumberManager;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::IsCallNegotiator
// -----------------------------------------------------------------------------
//
TBool CPhSrvServer::IsCallNegotiator() const
    {
    return ( iCallRequestManager != NULL );
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::IsUssdManager
// -----------------------------------------------------------------------------
//
TBool CPhSrvServer::IsUssdManager() const
    {
    return ( iUssdManager != NULL );
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::ComHandNegotiatorL
// 
// Access the command handler negotiator.
// -----------------------------------------------------------------------------
//
MPhSrvComHandNegotiator& CPhSrvServer::ComHandNegotiatorL()
    {
    if ( !iComHandRequestManager )
        {
        CPhSrvComHandRequestManager* self = 
            new ( ELeave ) CPhSrvComHandRequestManager( *this );
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );

        iComHandRequestManager = self;
        }
    
    return *iComHandRequestManager;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::ComHandNegotiatorL
// 
// Access the command handler negotiator.
// -----------------------------------------------------------------------------
//
MPhSrvComHandNegotiator& CPhSrvServer::ComHandNegotiator() const
    {
    __ASSERT_DEBUG( IsComHandNegotiator(), 
            PhoneServerUtils::Panic( EPhoneServerPanicMissingComInitiator ) );
    return *iComHandRequestManager;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::IsComHandNegotiator
// -----------------------------------------------------------------------------
//
TBool CPhSrvServer::IsComHandNegotiator() const
    {
    return ( iComHandRequestManager != NULL );
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::MessengerNegotiatorL
// 
// Access the Messenger negotiator.
// -----------------------------------------------------------------------------
//
MPhSrvMessengerNegotiator& CPhSrvServer::MessengerNegotiatorL()
    {
    if ( !iMessengerRequestManager )
        {
        CPhSrvMessengerRequestManager* self = 
            new ( ELeave ) CPhSrvMessengerRequestManager( *this );
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );

        iMessengerRequestManager = self;
        }
    
    return *iMessengerRequestManager;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::MessengerNegotiator
// 
// Access the Messenger negotiator.
// -----------------------------------------------------------------------------
//
MPhSrvMessengerNegotiator& CPhSrvServer::MessengerNegotiator() const
    {
    __ASSERT_DEBUG( IsMessengerNegotiator(), 
        PhoneServerUtils::Panic( EPhoneServerPanicMissingMessNegotiator ) );
    return *iMessengerRequestManager;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::IsMessengerNegotiator
// -----------------------------------------------------------------------------
//
TBool CPhSrvServer::IsMessengerNegotiator() const
    {
    return ( iMessengerRequestManager != NULL );
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::FsSessionL
// -----------------------------------------------------------------------------
//
RFs& CPhSrvServer::FsSessionL()
    {
    if ( !iFsSession.Handle() )
        {
        User::LeaveIfError( iFsSession.Connect() );
        }

    return iFsSession;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::ResourceManagerL
// -----------------------------------------------------------------------------
//
CPhSrvResourceManager& CPhSrvServer::ResourceManagerL()
    {
    if ( !iResourceFileManager )
        {
        CPhSrvResourceManager* self = 
            new ( ELeave ) CPhSrvResourceManager( FsSessionL() );
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );

        iResourceFileManager = self;
        }
    
    return *iResourceFileManager;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::CreateAllStepL
// -----------------------------------------------------------------------------
//
TInt CPhSrvServer::CreateAllStepL()
    {
    TInt old = iCreateAllStep;
    TRAPD( err, PerformNextStepL() );
    if ( old == iCreateAllStep )
        {
        iCreateAllStep = ECreateIdle;
        err = KErrGeneral;
        }

    iCreateAllError = Min( iCreateAllError, err );
    
    if ( iCreateAllStep == ECreateIdle )
        {
        CompleteCreateAll( iCreateAllError );
        return 0; // To stop idle
        }
    
    return 1; // To continue idle processing
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::PerformNextStepL
// -----------------------------------------------------------------------------
//
void CPhSrvServer::PerformNextStepL()
    {
    switch ( iCreateAllStep )
        {
        case  ECreateEmergencyNegotiator:
            iCreateAllStep = ECreateCallNegotiator;
            EmergencyCallNegotiatorL();
            break;
            
        case ECreateCallNegotiator:
            iCreateAllStep = ECreateUssdManager;
            CallNegotiatorL();
            break;

        case ECreateUssdManager:
            iCreateAllStep = ECreatePhoneInterface;
            UssdManagerL();
            break;

        case ECreatePhoneInterface:
            iCreateAllStep = ECreateEmergencyNumber;
            PhoneInterfaceL();
            break;

        case ECreateEmergencyNumber:
            iCreateAllStep = ECreateComHandManager;
            EmergencyNumberManagerL();
            break;

        case ECreateComHandManager:
            iCreateAllStep = ECreateMessengerManager;
            ComHandNegotiatorL();
            break;

        case ECreateMessengerManager:
            iCreateAllStep = ECreateIdle;
            // Create messenger negotiator only if needed.
            if ( FeatureManager::FeatureSupported( 
                KFeatureIdCsVideoTelephony ) )
                {
                MessengerNegotiatorL();
                }
            break;

        case ECreateIdle:
        default:
            iCreateAllStep = ECreateEmergencyNegotiator;            
            break;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::CompleteCreateAll
// -----------------------------------------------------------------------------
//
void CPhSrvServer::CompleteCreateAll( TInt aError )
    {
    TInt count = iCreateAllArray.Count();
    TInt index;

    for ( index = 0; index < count; index++ )
        {
        iCreateAllArray[ index ]->CompleteCreateAll( aError );
        }

    iCreateAllArray.Close();
    
    delete iCreateAll; 
    iCreateAll = NULL;

    iCreateAllError = KErrNone;
    iCreateAllStep = ECreateIdle;
    }


// -----------------------------------------------------------------------------
// CPhSrvServer::DoCreateAllStepL
// -----------------------------------------------------------------------------
//
TInt CPhSrvServer::DoCreateAllStepL( TAny* aAny )
    {
    return static_cast< CPhSrvServer* >( aAny )->CreateAllStepL();
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::ConnectFBS
// -----------------------------------------------------------------------------
//
TInt CPhSrvServer::ConnectFBS()
    {
    TInt err( KErrNone );
    if ( iFBSConnections == 0 )
        {
        err = RFbsSession::Connect();
        }

    if ( err == KErrNone )
        {
        iFBSConnections++;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CPhSrvServer::DisconnectFBS
// -----------------------------------------------------------------------------
//
void CPhSrvServer::DisconnectFBS( TBool aForce )
    {
    iFBSConnections--;
    if ( iFBSConnections == 0 || aForce )
        {
        RFbsSession::Disconnect();
        }
    }

// End of file
