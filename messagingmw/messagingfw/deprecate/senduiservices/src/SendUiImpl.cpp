/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#include <coemain.h>
#include <sendnorm.rsg>
#include <aknlistquerydialog.h>
#include <eikmenup.h>
#include <data_caging_path_literals.hrh>
#include <DRMHelper.h>
#include <centralrepository.h>
#include <e32cmn.h> // Uid
#include <e32property.h>
#include <barsread.h>
#include <akninputblock.h>
#include <centralrepository.h>
#include <miutset.h>
//#include <MessagingDomainCRKeys.h>

// Features
#include <featmgr.h>    
#include <bldvariant.hrh>
//#include <msgmimetypes.h>
//#include <CSendingServiceInfo.h>
//#include <CSendingService.h>
//#include <CMessageData.h>
//#include <SendUiConsts.h>
//#include <senduisingleton.h>

#include "SendUiImpl.h"
#include "senduiserviceclient.h"
#include "SendUiCapabilityCheck.h"
#include "SendUiUtils.h"
#include "senduiserviceslog.h"              // Logging
#include "SendUiPrivateCRKeys.h"
#include "senduiproperty.h"
#include "propertyobserver.h"
#include "senduilauncher.h"
#include "SendUiInternalConsts.h"


const TInt KArrayGranularity = 2;
const TInt KSendUiServiceOrderArrayGranularity = 6;
const TInt KTitleBufLength = 64;


_LIT( KSendUiResourceFileName,"sendnorm.rsc" );

// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// CSendUiImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendUiImpl* CSendUiImpl::NewL()
    {
    
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSendUiImpl::~CSendUiImpl()
    {
    
    }

// -----------------------------------------------------------------------------
// AddTypedMenuItemL
//
// No menu item is added, if services are not found. This can happen if asyncronous
// sending service update is not yet finished.
// -----------------------------------------------------------------------------
//
void CSendUiImpl::AddTypedMenuItemL(
    CSendUi::TSendUiMenuType    aMenuType,
    CEikMenuPane&               aMenuPane,
    TInt                        aIndex,
    TInt                        aCommandId,
    TSendingCapabilities        aRequiredCapabilities )
    {
   
    }

// -----------------------------------------------------------------------------
// ShowTypedQueryAndSendL
// First shows list query and then creates the message.
// Editor is launched for editing the message or the message is sent
// directly without opening the editor. Functionality depends on the
// type of selected sending service.
// -----------------------------------------------------------------------------
//
void CSendUiImpl::ShowTypedQueryAndSendL(
    CSendUi::TSendUiMenuType    aMenuType,
    const CMessageData*         aMessageData,
    TSendingCapabilities        aRequiredCapabilities,
    CArrayFix<TUid>*            aServicesToDim,
    TBool                       aLaunchEditorEmbedded,
    const TDesC&                aTitleText )
    {
   
    }

// -----------------------------------------------------------------------------
// ShowTypedQueryL
// Shows list query and returns the user selection.
// -----------------------------------------------------------------------------
//
TUid CSendUiImpl::ShowTypedQueryL(
    CSendUi::TSendUiMenuType    aMenuType,
    const CMessageData*         aMessageData,
    TSendingCapabilities        aRequiredCapabilities,
    CArrayFix<TUid>*            aServicesToDim,
    const TDesC&                aTitleText )
    {
    }

// -----------------------------------------------------------------------------
// SendMessageL
// -----------------------------------------------------------------------------
TInt CSendUiImpl::SendMessageL(    
    TUid                    aServiceUid,
    const CMessageData*     aMessageData,
    TBool                   aLaunchEditorEmbedded)
    { 
    }

// -----------------------------------------------------------------------------
// SendMessageViaServer
// -----------------------------------------------------------------------------
//
void CSendUiImpl::SendMessageViaServerL( 
    TUid                    aServiceUid,
    const CMessageData*     aMessageData,
    TBool                   aLaunchEditorEmbedded )
    {
   
    }

// -----------------------------------------------------------------------------
// CreateAndSendMessageL
// Creates the message and launches the editor for editing the message
// or sends the message directly without opening the editor. 
// Functionality depends on the type of selected sending service.
// -----------------------------------------------------------------------------
//
void CSendUiImpl::CreateAndSendMessageL(
    TUid                    aServiceUid,
    const CMessageData*     aMessageData,
    TBool                   aLaunchEditorEmbedded )
    {
   
   }

// -----------------------------------------------------------------------------
// ServiceCapabilitiesL
// Returns sending capabilities of the sending service.
// -----------------------------------------------------------------------------
//
TInt CSendUiImpl::ServiceCapabilities(
    TUid                    aServiceUid,
    TSendingCapabilities&   aServiceCapabilities )
    {
     }

// -----------------------------------------------------------------------------
// AvailableServicesL
// Populates given list with the information about services
// provided by specified ECom service provider.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CSendUiImpl::AvailableServicesL(
    RPointerArray<CSendingServiceInfo>& aServiceList,
    TUid aServiceProvider)
    {
    
    }
// -----------------------------------------------------------------------------
// TechnologyType
// -----------------------------------------------------------------------------
//
TUid CSendUiImpl::TechnologyType( TUid aServiceUid )
    {
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CSendUiImpl::ResolveServiceIndex( TUid aServiceUid )
    {
   
    }

// -----------------------------------------------------------------------------
// QueryCapabilities
// -----------------------------------------------------------------------------
//
TBool CSendUiImpl::QueryCapabilities( 
    TInt index, 
    TSendingCapabilities aRequiredCapabilities )
    {
   
    }

// -----------------------------------------------------------------------------
// CSendUiImpl::GetServicesL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CSendUiImpl::GetServicesL( )
{
    
}

// ---------------------------------------------------------
// ShowListQueryL(
// Displays list query.
// (other items were commented in a header).
// ---------------------------------------------------------
// 
TUid CSendUiImpl::ShowListQueryL(
    MDesCArray*                 aListItems,
    const CArrayFix<TUid>&      aListItemUids,
    const TDesC&                aTitleText,
    CSendUi::TSendUiMenuType    aMenuType )
    {
   
    }


// -----------------------------------------------------------------------------
// CanSendContentL
// -----------------------------------------------------------------------------
//
TBool CSendUiImpl::CanSendContentL(
    TUid                    aServiceUid,
    TUid                    aServiceProviderId,
    const CMessageData*     aMessageData,
    TDataType*              aMimeType )
    {
   
   }

// -----------------------------------------------------------------------------
// CSendUiImpl::FilterServicesL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSendUiImpl::FilterServicesL(
    CDesCArrayFlat&         aListItems,
    CArrayFix<TUid>&        aListItemUids,
    TSendingCapabilities    aRequiredCapabilities,
    const CMessageData*     aMessageData,
    CSendUi::TSendUiMenuType    aMenuType  )
    {
    
    }

// -----------------------------------------------------------------------------
// SortListL
// Sets ECom services to correct positions in "Send"-list query.
// -----------------------------------------------------------------------------
//
void CSendUiImpl::SortListL(
    CDesCArrayFlat&     aListItems,
    CArrayFix<TUid>&    aListItemUids )
    {
    
    }


// ---------------------------------------------------------
// ResetIsSending
// Resets sending state flag.
// ---------------------------------------------------------
//    
void CSendUiImpl::ResetIsSending( TAny* aAny )
    {
    }

// -----------------------------------------------------------------------------
// CheckServiceCapabilities
// Compares ECom service sending capabilities to required capabilities.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSendUiImpl::CheckServiceCapabilities(
    TUid                    aServiceUid,
    TSendingCapabilities    aRequiredCapabilities )
    {
    
    }

// -----------------------------------------------------------------------------
// CSendUiImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUiImpl::CSendUiImpl()
    :  iCoeEnv( CCoeEnv::Static() ),
        iFs( iCoeEnv->FsSession() ),
        iResourceLoader( *iCoeEnv )
        
    {
    }

// -----------------------------------------------------------------------------
// CSendUiImpl::FeatureStatus
// -----------------------------------------------------------------------------
//
void CSendUiImpl::FeatureStatus( TInt aFeature, TInt &aNewFeatures, TInt aFeatureFlag )
    {
    
    }
// ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendUiImpl::ConstructL()
    {
    
    }

// end of file

