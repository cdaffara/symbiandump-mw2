/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsulation of external dependencies of Phone Parser.
*
*/


// INCLUDE FILES
#include    "cphoneparserfeatures.h" 
#include    <featmgr.h>
#include    <centralrepository.h>
#include    <telservicesvariant.hrh>
#include    <telservicesinternalcrkeys.h>
#include    <cphcltemergencycall.h> 

// FORWARD DECLARATION

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::CPhoneParserFeatures
// -----------------------------------------------------------------------------
//
CPhoneParserFeatures::CPhoneParserFeatures()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneParserFeatures::ConstructL()
    {
    // Read local variation settings.
    FeatureManager::InitializeLibL();
    iEmergencyCall = CPhCltEmergencyCall::NewL( this );
    iRepository = CRepository::NewL( KCRUidTelSrvVariation );
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::InstanceL
// -----------------------------------------------------------------------------
//
CPhoneParserFeatures* CPhoneParserFeatures::InstanceL()
    {
    CPhoneParserFeatures* self =
        static_cast<CPhoneParserFeatures*>( Dll::Tls() );

    if( !self )
        {
        self = new (ELeave) CPhoneParserFeatures;
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );
        Dll::SetTls( self );
        }

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::Instance
// -----------------------------------------------------------------------------
//
CPhoneParserFeatures* CPhoneParserFeatures::Instance()
    {
    CPhoneParserFeatures* self( NULL );
    TRAPD( err, self = InstanceL() );
    return err ? NULL : self;
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::Free
// -----------------------------------------------------------------------------
//
void CPhoneParserFeatures::Free()
    {
    CPhoneParserFeatures* self =
        static_cast<CPhoneParserFeatures*>( Dll::Tls() );

    if( self )
        {
        delete self;
        self = NULL;
        Dll::SetTls( NULL );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::SetInstance
// -----------------------------------------------------------------------------
//
#ifdef __WINS__
void CPhoneParserFeatures::SetInstance( 
    CPhoneParserFeatures* aStub )
    {
    Dll::SetTls( aStub );
    }
#endif // __WINS__
// -----------------------------------------------------------------------------
// CPhoneParserFeatures::~CPhoneParserFeatures
// -----------------------------------------------------------------------------
//
CPhoneParserFeatures::~CPhoneParserFeatures()
    {
    delete iEmergencyCall;
    delete iRepository;
    FeatureManager::UnInitializeLib();
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::HandleEmergencyDialL
// -----------------------------------------------------------------------------
//
void CPhoneParserFeatures::HandleEmergencyDialL( const TInt /*aStatus*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::TwoDigitCallingEnabled
// -----------------------------------------------------------------------------
//
TBool CPhoneParserFeatures::TwoDigitCallingEnabled()
    {
    return Instance() ? 
        Instance()->GetTelephoneVariant() & KTelSrvLVFlagTwoDigitDial : 
        EFalse;
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::IsZeroDialEnabled
// -----------------------------------------------------------------------------
//
TBool CPhoneParserFeatures::IsZeroDialEnabled()
    {
    return Instance() ? 
        Instance()->GetTelephoneVariant() & KTelSrvLVFlagZeroDial : EFalse;
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::IsEmergencyNumber
// -----------------------------------------------------------------------------
//
TBool CPhoneParserFeatures::IsEmergencyNumber( 
    const TDesC& aNumber, 
    TDes& aEmergencyNumber )
    {
    return Instance() ? 
        Instance()->IsEmergencyPhoneNumber( aNumber, aEmergencyNumber ) : EFalse; 
    }    

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::IsItEmergencyNumber
// -----------------------------------------------------------------------------
//
TBool CPhoneParserFeatures::IsEmergencyPhoneNumber( 
    const TDesC& aNumber, 
    TDes& aEmergencyNumber )
    {
    TBool isEmergency;

    TInt error = iEmergencyCall->IsEmergencyPhoneNumber( 
        aNumber, 
        aEmergencyNumber, 
        isEmergency );
            
    return error ? EFalse : isEmergency;
    }    

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::GetTelephoneVariant
// -----------------------------------------------------------------------------
//
TInt CPhoneParserFeatures::GetTelephoneVariant()
    {
    TInt telephoneVariant;
    TInt err = iRepository->Get( KTelSrvVariationFlags, telephoneVariant );
    return err == KErrNone ? telephoneVariant : 0;
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::FeatureSupported
// -----------------------------------------------------------------------------
// 
TBool CPhoneParserFeatures::FeatureSupported( TInt aFeature )
    {
    return Instance() ? Instance()->IsFeatureSupported( aFeature ) : EFalse;
    }

// -----------------------------------------------------------------------------
// CPhoneParserFeatures::IsFeatureSupported
// -----------------------------------------------------------------------------
// 
TBool CPhoneParserFeatures::IsFeatureSupported( TInt aFeature )
    {
    return FeatureManager::FeatureSupported( aFeature );
    }

//  End of File  
