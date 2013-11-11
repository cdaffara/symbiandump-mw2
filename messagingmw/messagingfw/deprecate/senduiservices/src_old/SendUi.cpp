/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Can be used to show "Send" list query and to create and send
*                messages via available sending services. Created messages are
*                sent directly or message editor is opened for editing,
*                depending on the type of the selected service.
*
*/




// INCLUDE FILES
#include <sendui.h>
#include "SendUiImpl.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUi::CSendUi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUi::CSendUi()
    {
    }

// -----------------------------------------------------------------------------
// CSendUi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendUi::ConstructL()
    {
    iSendUiImpl = CSendUiImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CSendUi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUi* CSendUi::NewL()
    {
    CSendUi* self = CSendUi::NewLC();
    
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CSendUi::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
EXPORT_C CSendUi* CSendUi::NewLC()
    {
    CSendUi* self = new (ELeave) CSendUi;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }
   
// Destructor
EXPORT_C CSendUi::~CSendUi()
    {
    delete iSendUiImpl;
    }

// -----------------------------------------------------------------------------
// CSendUi::AddSendMenuItemL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUi::AddSendMenuItemL(
    CEikMenuPane&               aMenuPane,
    TInt                        aIndex,
    TInt                        aCommandId,
    TSendingCapabilities        aRequiredCapabilities )
    {
    iSendUiImpl->AddTypedMenuItemL(
        ESendMenu,
        aMenuPane,
        aIndex,
        aCommandId,
        aRequiredCapabilities);
    }

// -----------------------------------------------------------------------------
// CSendUi::ShowQueryAndSendL
// First shows "Send" pop-up list query and then creates the message.
// Editor is launched for editing the message or the message is sent
// directly without opening the editor. Functionality depends on the
// type of selected sending service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUi::ShowQueryAndSendL(
    const CMessageData*     aMessageData,
    TSendingCapabilities    aRequiredCapabilities,
    CArrayFix<TUid>*        aServicesToDim,
    TUid                    /*aBioTypeUid*/,
    TBool                   aLaunchEditorEmbedded,
    const TDesC&            aTitleText )
    {
    iSendUiImpl->ShowTypedQueryAndSendL(
        ESendMenu,
        aMessageData,
        aRequiredCapabilities,
        aServicesToDim,
        aLaunchEditorEmbedded,
        aTitleText );
    }

// -----------------------------------------------------------------------------
// CSendUi::ShowSendQueryL
// Shows "Send" pop-up list query and returns the user selection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CSendUi::ShowSendQueryL(
    const CMessageData*     aMessageData,
    TSendingCapabilities    aRequiredCapabilities,
    CArrayFix<TUid>*        aServicesToDim,
    const TDesC&            aTitleText )
    {
    return iSendUiImpl->ShowTypedQueryL(
        ESendMenu,
        aMessageData,
        aRequiredCapabilities,
        aServicesToDim,
        aTitleText );
    }

// -----------------------------------------------------------------------------
// CSendUi::CreateAndSendMessageL
// Creates the message and launches the editor for editing the message
// or sends the message directly without opening the editor. 
// Functionality depends on the type of selected sending service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUi::CreateAndSendMessageL(
    TUid                    aServiceUid,
    const CMessageData*     aMessageData,
    TUid                    /*aBioTypeUid*/,
    TBool                   aLaunchEditorEmbedded )
    {
    iSendUiImpl->CreateAndSendMessageL(
        aServiceUid,
        aMessageData,
        aLaunchEditorEmbedded );
    }

// -----------------------------------------------------------------------------
// CSendUi::ValidateServiceL
// Validates that service is available and has required capabilities.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendUi::ValidateServiceL(
    TUid                    aServiceUid,
    TSendingCapabilities    aRequiredCapabilities )
    {
    return iSendUiImpl->QueryCapabilities(
        iSendUiImpl->ResolveServiceIndex( aServiceUid ), 
        aRequiredCapabilities );
    }

// -----------------------------------------------------------------------------
// CSendUi::ServiceCapabilitiesL
// Returns sending capabilities of the sending service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSendUi::ServiceCapabilitiesL(
    TUid                    aServiceUid,
    TSendingCapabilities&   aServiceCapabilities )
    {
    return iSendUiImpl->ServiceCapabilities(
        aServiceUid, aServiceCapabilities );
    }

// -----------------------------------------------------------------------------
// CSendUi::CanSendBioMessage
// Can be used to check if sending of bio message is supported.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendUi::CanSendBioMessage( TUid /*aBioMessageUid*/ ) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSendUi::AvailableServicesL
// Populates given list with the information about services
// provided by specified ECom service provider.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUi::AvailableServicesL(
    RPointerArray<CSendingServiceInfo>& aServiceList,
    TUid aServiceProvider)
    {
    iSendUiImpl->AvailableServicesL( aServiceList, aServiceProvider );
    }

// -----------------------------------------------------------------------------
// CSendUi::AddTypedMenuItemL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUi::AddTypedMenuItemL(
    TSendUiMenuType             aMenuType,
    CEikMenuPane&               aMenuPane,
    TInt                        aIndex,
    TInt                        aCommandId,
    TSendingCapabilities        aRequiredCapabilities )
    {
    iSendUiImpl->AddTypedMenuItemL(
        aMenuType,
        aMenuPane,
        aIndex,
        aCommandId,
        aRequiredCapabilities);
    }

// -----------------------------------------------------------------------------
// CSendUi::ShowTypedQueryAndSendL
// First shows list query and then creates the message.
// Editor is launched for editing the message or the message is sent
// directly without opening the editor. Functionality depends on the
// type of selected sending service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUi::ShowTypedQueryAndSendL(
    TSendUiMenuType         aMenuType,
    const CMessageData*     aMessageData,
    TSendingCapabilities    aRequiredCapabilities,
    CArrayFix<TUid>*        aServicesToDim,
    TUid                    /*aBioTypeUid*/,
    TBool                   aLaunchEditorEmbedded,
    const TDesC&            aTitleText )
    {
    iSendUiImpl->ShowTypedQueryAndSendL(
        aMenuType,
        aMessageData,
        aRequiredCapabilities,
        aServicesToDim,
        aLaunchEditorEmbedded,
        aTitleText );
    }

// -----------------------------------------------------------------------------
// CSendUi::ShowTypedQueryL
// Shows list query and returns the user selection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CSendUi::ShowTypedQueryL(
    TSendUiMenuType         aMenuType,
    const CMessageData*     aMessageData,
    TSendingCapabilities    aRequiredCapabilities,
    CArrayFix<TUid>*        aServicesToDim,
    const TDesC&            aTitleText )
    {
    return iSendUiImpl->ShowTypedQueryL(
        aMenuType,
        aMessageData,
        aRequiredCapabilities,
        aServicesToDim,
        aTitleText );
    }

// -----------------------------------------------------------------------------
// CSendUi::TechnologyType
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CSendUi::TechnologyType( TUid aServiceUid ) const
    {
    return iSendUiImpl->TechnologyType( aServiceUid );
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File

