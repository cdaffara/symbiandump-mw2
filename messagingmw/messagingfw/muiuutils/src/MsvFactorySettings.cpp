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
* Description: MsvFactorySettings implementation
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <e32test.h>
#include <msvapi.h>
#include <msvuids.h>
#include <mtmuibas.h>
#include <mtuireg.h>
#include <mtudcbas.h>
#include <bautils.h>
#include <mtmuidef.hrh>
#include "MsvFactorySettings.h"



// MODULE DATA STRUCTURES
class CDummyObserver : public CBase, public MMsvSessionObserver
    {
    public:
        CDummyObserver() {};
        void HandleSessionEventL( TMsvSessionEvent /*aEvent*/, 
                                  TAny* /*aArg1*/, 
                                  TAny* /*aArg2*/, 
                                  TAny* /*aArg3*/ ) {};
    };

// LOCAL FUNCTION PROTOTYPES
/**
* Performs factory settings to single item
*/
void DoRestoreFactorySettingsSingleMtmL( CClientMtmRegistry& aBaseReg, 
                                         CMtmUiRegistry& aUiReg, 
                                         TUid anId, 
                                         TMsvFactorySettingsLevel aLevel );


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// MsvFactorySettings::ResetL
// ---------------------------------------------------------
//
EXPORT_C void MsvFactorySettings::ResetL( TMsvFactorySettingsLevel aLevel )
    {
    // connect to the server
    CDummyObserver* ob = new( ELeave )CDummyObserver;
    CleanupStack::PushL(ob);

    CMsvSession* session = CMsvSession::OpenSyncL( *ob );
    CleanupStack::PushL( session );
    
    //create ui registry
    CClientMtmRegistry* basemtmregistry = CClientMtmRegistry::NewL( *session );
    CleanupStack::PushL( basemtmregistry );
    CMtmUiRegistry* uiRegistry = CMtmUiRegistry::NewL( *session );
    CleanupStack::PushL( uiRegistry );

    //check through mtms
    TInt error = 0;
    TInt count=uiRegistry->NumRegisteredMtmDlls();
    for ( TInt cc=0; cc < count; ++cc )
        {
        // return value is ignored, so as not to prevent other MTMs being initialised
        TRAPD(ret, DoRestoreFactorySettingsSingleMtmL( *basemtmregistry, 
                                                       *uiRegistry, 
                                                       basemtmregistry->MtmTypeUid( cc ), 
                                                       aLevel ) );
        if ( ret && ret != KErrNotSupported )
            {
            error = ret;
            }
        }
    
    User::LeaveIfError( error );
    CleanupStack::PopAndDestroy( 4,ob ); // uiRegistry,basemtmregistry,session,ob
    }


// ---------------------------------------------------------
// DoRestoreFactorySettingsSingleMtmL
// ---------------------------------------------------------
//
void DoRestoreFactorySettingsSingleMtmL( CClientMtmRegistry& aBaseReg, 
                                         CMtmUiRegistry& aUiReg, 
                                         TUid anId, 
                                         TMsvFactorySettingsLevel aLevel )
    {
    //create base mtm
    CBaseMtm* baseMtm = aBaseReg.NewMtmL( anId );
    CleanupStack::PushL( baseMtm );
    
    CBaseMtmUi* mtmUi = aUiReg.NewMtmUiL( *baseMtm );
    CleanupStack::PushL( mtmUi );

    TInt response;
    TUid isInstalled = { KUidMsvMtmQueryFactorySettings };
    TInt err = mtmUi->QueryCapability( isInstalled, response );

    if ( err == KErrNone )
        {
        // Need to install this type
        CMsvEntrySelection* tempobject = new ( ELeave ) CMsvEntrySelection;
        CleanupStack::PushL( tempobject );
        TPckg<TMsvFactorySettingsLevel > levelPack( aLevel );
        TInt installNow = { KMtmUiFunctionRestoreFactorySettings };    
        mtmUi->InvokeSyncFunctionL( installNow, *tempobject, levelPack );
        CleanupStack::PopAndDestroy( tempobject );
        }
    else if ( err != KErrNotSupported )
        {
        User::Leave( err );
        }
    else
        {
        //Does nothing
        }

    CleanupStack::PopAndDestroy( 2, baseMtm ); // mtmUi,baseMtm
    }

//  End of File  

