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
* Description:  The Refresh handler to inform refresses.
*
*/



// INCLUDE FILES
#include    "psetrefreshhandlerimpl.h" 
#include    <etelmm.h>
#include    <msssettingsrefreshhandler.h>
#include    <csssettingsrefreshcontainer.h>
#include    <psetcsp.h> 
#include    "phonesettingslogger.h" 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::CPSetRefreshHandlerImpl
// 
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPSetRefreshHandlerImpl::CPSetRefreshHandlerImpl()
    {
    }


// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSetRefreshHandlerImpl::ConstructL()
    {
    __PHSLOGSTRING("[PHS]--> CPSetRefreshHandlerImpl::ConstructL");
    iCsp = CPsetCustomerServiceProfile::NewL();

    iSSSetRefContainer = CSSSettingsRefreshContainer::NewL();
    iSSSetRefHandler = &iSSSetRefContainer->RefreshHandler();
    __PHSLOGSTRING("[PHS] <--CPSetRefreshHandlerImpl::ConstructL");
    }


// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPSetRefreshHandlerImpl* CPSetRefreshHandlerImpl::NewL()
    {
    CPSetRefreshHandlerImpl* self = new( ELeave ) CPSetRefreshHandlerImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::~CPSetRefreshHandlerImpl
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPSetRefreshHandlerImpl::~CPSetRefreshHandlerImpl()
    {
    iObserver = NULL;

    iSSSetRefHandler = NULL;
    delete iSSSetRefContainer;
    iSSSetRefContainer = NULL;

    delete iCsp;
    iCsp = NULL;
    }


// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::NotifyFileChangeL
// 
// 
// -----------------------------------------------------------------------------
//
void CPSetRefreshHandlerImpl::NotifyFileChangeL(
    MSSSettingsRefreshObserver& aObserver,
    TSatElementaryFiles aObservedFiles,
    TSatRefreshType aObservedRefreshType )
    {
    __PHSLOGSTRING("[PHS]--> CPSetRefreshHandlerImpl::NotifyFileChangeL");
    iObserver = &aObserver;

    iSSSetRefHandler->NotifyFileChangeL(
        *this,
        aObservedFiles,
        aObservedRefreshType );
    __PHSLOGSTRING("[PHS] <--CPSetRefreshHandlerImpl::NotifyFileChangeL");
    }


// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::CancelNotify
// 
// 
// -----------------------------------------------------------------------------
//
void CPSetRefreshHandlerImpl::CancelNotify()
    {
    iObserver = NULL;

    iSSSetRefHandler->CancelNotify();
    }


// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::ChangedCspSettings
// 
// 
// -----------------------------------------------------------------------------
//
TInt CPSetRefreshHandlerImpl::ChangedCspSettings( 
    TPSetChangedCspSetting& aChangedCspSettings,
    TPSetChangedCspSetting& aNewValues )
    {
    __PHSLOGSTRING("[PHS]--> CPSetRefreshHandlerImpl::ChangedCspSettings");
    RMobilePhone::TMobilePhoneCspFileV1 cspSettings;
    RMobilePhone::TMobilePhoneCspFileV1Pckg cspSettingsPckg ( cspSettings );
    RMobilePhone::TMobilePhoneCspFileV1 cspValues;
    RMobilePhone::TMobilePhoneCspFileV1Pckg cspValuePckg ( cspValues );

    TInt err = iSSSetRefHandler->ChangedCspTable( 
        cspSettingsPckg,
        cspValuePckg );

    if ( err == KErrNone )
        {
        // First reset the parameters to contain nothing.
        TInt changedSettings = EPSetNoChangedSetting;
        TInt newValues = EPSetNoChangedSetting;


        // Call waiting support.
        if ( iCsp->CheckIfCWSupported( ( RMobilePhone::TCspCallCompletion ) 
            cspSettings.iCallCompletionServices ) )
            {
            // Call waiting settings have changed.
            changedSettings += EPSetCW;
            // Find the new value.
            if ( iCsp-> CheckIfCWSupported( ( RMobilePhone::TCspCallCompletion ) 
                cspValues.iCallCompletionServices ) )
                {
                newValues += EPSetCW;
                }
            }

        // Call barring support.
        if ( iCsp->CheckIfCBSupported( ( RMobilePhone::TCspCallRestriction ) 
            cspSettings.iCallRestrictionServices ) )
            {
            // Call barring settings have changed.
            changedSettings += EPSetCB;

            // Find the new value.
            if ( iCsp->CheckIfCBSupported( ( RMobilePhone::TCspCallRestriction ) 
                cspValues.iCallRestrictionServices ) )
                {
                newValues += EPSetCB;
                }
            }

        // Call forwarding support.
        if ( iCsp->CheckIfCFSupported( ( RMobilePhone::TCspCallOffering ) 
            cspSettings.iCallOfferingServices )  )
            {
            // Call forwarding settings have changed.
            changedSettings += EPSetCF;

            // Find the new value.
            if ( iCsp->CheckIfCFSupported( ( RMobilePhone::TCspCallOffering ) 
                cspValues.iCallOfferingServices )  )
                {
                newValues += EPSetCF;
                }
            }

        // ALS support.
        if ( iCsp->CheckIfAlsSupported( ( RMobilePhone::TCspCPHSTeleservices ) 
            cspSettings.iCphsTeleservices )  )
            {
            // ALS settings have changed.
            changedSettings += EPSetALS;

            // Find the new value.
            if ( iCsp->CheckIfAlsSupported( ( RMobilePhone::TCspCPHSTeleservices ) 
                cspValues.iCphsTeleservices )  )
                {
                newValues += EPSetALS;
                }
            }

        // Network selection support.
        if (  iCsp->CheckIfNetworkSelectionSupported( ( 
            RMobilePhone::TCspValueAdded ) cspSettings.iValueAddedServices ) )
            {
            // Network selection support settings have changed.
            changedSettings += EPSetNetSelSup;

            // Find the new value.
            if (  iCsp->CheckIfNetworkSelectionSupported( ( 
                RMobilePhone::TCspValueAdded ) cspValues.iValueAddedServices ) )
                {
                newValues += EPSetNetSelSup;
                }
            }

        // Set the changed values to user parameter.
        aChangedCspSettings = ( TPSetChangedCspSetting ) changedSettings;
        aNewValues = ( TPSetChangedCspSetting ) newValues;
        }

    __PHSLOGSTRING1("[PHS]   CPSetRefreshHandlerImpl::ChangedCspSettings: err: %d", err);
    __PHSLOGSTRING("[PHS] <--CPSetRefreshHandlerImpl::ChangedCspSettings");
    return err;
    }

// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::AllowRefresh
// 
// 
// -----------------------------------------------------------------------------
//
TBool CPSetRefreshHandlerImpl::AllowRefresh(
    const TSatRefreshType aType,
    const TSatElementaryFiles aFiles )
    {
    TBool allow = ETrue;

    if ( iObserver )
        {
        allow = iObserver->AllowRefresh(
            aType,
            aFiles );
        }

    return allow;
    }

// -----------------------------------------------------------------------------
// CPSetRefreshHandlerImpl::Refresh
// 
// 
// -----------------------------------------------------------------------------
//
void CPSetRefreshHandlerImpl::Refresh(
    const TSatRefreshType aType,
    const TSatElementaryFiles aFiles )
    {
    if ( iObserver )
        {
        iObserver->Refresh(
            aType,
            aFiles );
        }
    }


//  End of File  
