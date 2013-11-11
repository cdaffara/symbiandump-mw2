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
#include <Sendnorm.rsg>
#include <AknListQueryDialog.h>
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
#include <MessagingDomainCRKeys.h>

// Features
#include <FeatMgr.h>    
#include <bldvariant.hrh>
#include <msgmimetypes.h>
#include <CSendingServiceInfo.h>
#include <CSendingService.h>
#include <CMessageData.h>
#include <SendUiConsts.h>
#include <senduisingleton.h>

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
    CSendUiImpl* self = new( ELeave ) CSendUiImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSendUiImpl::~CSendUiImpl()
    {
    iResourceLoader.Close();
    TInt count = iSendingServices.Count();
    while ( count )
        {
        delete iSendingServices[--count];
        }
    iSendingServices.Close();
    delete iSendUiServicesOrderArray;
    delete iServicesToDim;
    delete iPropertyObserver;
    delete iSingleton;
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
    TInt i(0);
    TBool menuItemAdded = EFalse;
    TBuf<KTitleBufLength> menuItemName;
    
     // Read the resources
    aMenuType == CSendUi::ESendMenu ? 
        i = R_SENDUI_MENUITEM_SEND : 
        i = R_SENDUI_MENUITEM_WRITE; // CSendUi::EWriteMenu
        
    iCoeEnv->ReadResourceL( menuItemName, i );
  
    for ( i = 0; i < iSendingServices.Count(); i++ )
    {
    TUid serviceUid = iSendingServices[i]->ServiceId();
    if ( QueryCapabilities( i, aRequiredCapabilities ) )
        {
        CEikMenuPaneItem::SData data;
        data.iCascadeId = NULL;
        data.iText = menuItemName;
        data.iCommandId = aCommandId;

        if ( aIndex == aMenuPane.NumberOfItemsInPane() )
            {
            aMenuPane.AddMenuItemL( data );
            }
        else
            {
            aMenuPane.InsertMenuItemL( data, aIndex );
            }
            
        menuItemAdded = ETrue;
        break;
        }
    }

    // Hide menu item if sending is already in progress.
    if ( menuItemAdded )
        {
        aMenuPane.SetItemDimmed( aCommandId, iIsSending);
        }        
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
    TUid serviceId = ShowTypedQueryL(
        aMenuType,
        aMessageData,
        aRequiredCapabilities,
        aServicesToDim,
        aTitleText );

    if ( serviceId != KNullUid )
        {
        // Process user selection.
        CreateAndSendMessageL(
            serviceId,
            aMessageData,
            aLaunchEditorEmbedded);
        }
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
    // Implemented for CR # 401-1806 >>    
    // This section to enable/disable features (supported by feature manager) at run time, is to be revised,
    // once an observer class is available from feature manager side to intimate about feature state
		
    CRepository* repository = CRepository::NewLC( KCRUidSendUi );
    TInt oldFeatures(0);   
    TInt newFeatures(0); 
    TBool updateNeeded( EFalse );   
    
    if ( repository->Get( KKeySendUiFeatureManagerServices, oldFeatures ) != KErrNone )
    	{
    	updateNeeded = ETrue;
    	}
    FeatureManager::InitializeLibL();
    FeatureStatus( KFeatureIdMmsPostcard, newFeatures, KSendUiPostcard );
    FeatureStatus( KFeatureIdAudioMessaging, newFeatures, KSendUiAudioMessage );
    FeatureStatus( KFeatureIdSenduiMmsUpload, newFeatures, KSendUiMmsUpload );
    FeatureManager::UnInitializeLib();
    if ( newFeatures != oldFeatures )
        {
        repository->Set( KKeySendUiFeatureManagerServices, newFeatures );
        updateNeeded = ETrue;
        }

    CleanupStack::PopAndDestroy( repository );
    
    RProperty propertyUpdateOperation;
    TRequestStatus status;
    if ( updateNeeded )
        {
        RProperty::Set( 
            KPSUidSendUiProperty, 
            KSendUiUpdateRequest, 
            KSendUiUpdateRequested );
        User::LeaveIfError( propertyUpdateOperation.Attach( KPSUidSendUiProperty, KSendUiUpdateRequest ));
        propertyUpdateOperation.Subscribe( status );	
        User::WaitForRequest(status);
        TInt value = KErrNone;
        if(propertyUpdateOperation.Get( value ) !=KErrNotFound && value == KSendUiUpdated )
            {
            TRAPD( err, GetServicesL() );
            if ( err )
                {
#ifdef _DEBUG
                TBuf<256> buf;
                buf.Format(_L("SendUi err"));
                User::InfoPrint(buf);
#endif
                TUid serviceId( KNullUid );
                return serviceId;
                }
            }
        propertyUpdateOperation.Close();
        }
    // Implemented for CR # 401-1806 <<    
    // List items for list query.
    CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat( KArrayGranularity );
    CleanupStack::PushL( listItems);

    // This array is used to map user selection to correct service.
    CArrayFix<TUid>* listItemUids = 
        new ( ELeave ) CArrayFixFlat<TUid>( KArrayGranularity );
    CleanupStack::PushL( listItemUids );

    iServicesToDim->Reset();

    TInt i = 0;
    if ( aServicesToDim )
        {
        for ( i = 0; i < aServicesToDim->Count(); i++  )
            {
            iServicesToDim->AppendL( (*aServicesToDim)[i] );
            }
        }
  
    // Get and filter ECom based services.
    FilterServicesL(
        *listItems,
        *listItemUids,
        aRequiredCapabilities,
        aMessageData,
        aMenuType );

    TUid serviceId( KNullUid );
    if ( listItems->Count() > 0 )
        {
        // Show list query.
        SortListL( *listItems, *listItemUids );

        serviceId = ShowListQueryL(
            listItems,
            *listItemUids,
            aTitleText,
            aMenuType );
        }

    CleanupStack::PopAndDestroy( 2, listItems );  // listItemUids, listItems

    return serviceId;
    }

// -----------------------------------------------------------------------------
// SendMessageL
// -----------------------------------------------------------------------------
TInt CSendUiImpl::SendMessageL(    
    TUid                    aServiceUid,
    const CMessageData*     aMessageData,
    TBool                   aLaunchEditorEmbedded)
    { 
    if ( iIsSending || aServiceUid == KNullUid )
        {
        return KErrNone;
        }
        
        CAknInputBlock* absorber = CAknInputBlock::NewLC( ); 
        // Ensures proper sending state if leave happens.
        iIsSending = ETrue;
        TInt success( KErrNone );
        CleanupStack::PushL( TCleanupItem( ResetIsSending, (TAny*)&iIsSending ) );

        // Create the requested ECom service.
        TInt i( ResolveServiceIndex( aServiceUid ));
        if ( i != KErrNotFound )
            {
            TRAP( success, 
                CSendingService* sendingService = CSendingService::NewL( 
                    iSendingServices[i]->ServiceProviderId(), *iCoeEnv, *iSingleton );
                CleanupStack::PushL( sendingService );  
                
                sendingService->CreateAndSendMessageL( aServiceUid, aMessageData, aLaunchEditorEmbedded );
                CleanupStack::PopAndDestroy( sendingService );
                    );
                
            }
        else
            {
            iIsSending = EFalse;
            }
    CleanupStack::PopAndDestroy( 2, absorber); // TCleanupItem, absorber
    return success;
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
    TInt index( ResolveServiceIndex( aServiceUid ));
    TBool capabilitiesOk = ETrue;
    if ( index == KErrNotFound )
    	{
    	return;
    	}
    if ( iSendingServices[index]->ServiceFeatures() & CSendingServiceInfo::EServiceCanSendDirectly )
    	{
	    // Check sending rights
	    RProcess process;
	    TSecurityInfo securityInfo( process );
	    CSendUiCapabilityCheck* capabilityCheck = CSendUiCapabilityCheck::NewLC();

		capabilitiesOk = capabilityCheck->CheckCapabilitiesL( 
		    securityInfo, 
		    iSingleton->MsvSessionL(), 
		    KSenduiMtmMmsUid, 
		    iSendingServices[index]->ServiceName() );
		    
		CleanupStack::PopAndDestroy( capabilityCheck );
	    }
    
    if ( capabilitiesOk )
    	{
        TUid providerUid = iSendingServices[index]->ServiceProviderId();	
	    RSendUiServiceClient sendUiClient;
	    CleanupClosePushL( sendUiClient );
	    sendUiClient.ConnectChainedAppL( KSendUiLauncherAppUid );
	    // LaunchSendUiL is syncronous operation
	    sendUiClient.LaunchSendUiL( 
	        aServiceUid,
	        providerUid,
	        aLaunchEditorEmbedded, 
	        aMessageData ); 
	    CleanupStack::PopAndDestroy( &sendUiClient);
    	}
	    
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
    LOGTEXT(_L("CSendUiImpl::CreateAndSendMessageL >>"));
    TInt err(0); // initializing this to 0 helps debugging SendMessageViaServerL
    err = SendMessageL( aServiceUid, aMessageData, aLaunchEditorEmbedded );
    if ( !iServerAppLaunch && ( err == KErrPermissionDenied || err == KErrAccessDenied ) )
        {
        SendMessageViaServerL( aServiceUid, aMessageData, aLaunchEditorEmbedded );
        }
    else
       	{
        User::LeaveIfError( err );
       	}
    LOGTEXT(_L("CSendUiImpl::CreateAndSendMessageL <<"));
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
    TInt i( ResolveServiceIndex( aServiceUid ));
    
    if ( i != KErrNotFound )
        {
        aServiceCapabilities = 
            iSendingServices[i]->ServiceCapabilities();
        i = KErrNone;        
        }
    return i;
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
    for ( TInt i(0); i < iSendingServices.Count(); i++ )
        {
        if ( iSendingServices[i]->ServiceProviderId() == aServiceProvider 
            || aServiceProvider == KNullUid )
            {
            CSendingServiceInfo* serviceInfo = iSendingServices[i]->CopyLC();
            aServiceList.AppendL( serviceInfo );
            CleanupStack::Pop( serviceInfo ); // serviceInfo
            }
        }
    }
// -----------------------------------------------------------------------------
// TechnologyType
// -----------------------------------------------------------------------------
//
TUid CSendUiImpl::TechnologyType( TUid aServiceUid )
    {
    TInt i = ResolveServiceIndex( aServiceUid );
    if ( i != KErrNotFound )
        {
        return iSendingServices[i]->TechnologyTypeId(); 
        }
    else
        {        
        return TUid::Null();
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CSendUiImpl::ResolveServiceIndex( TUid aServiceUid )
    {
    TInt index( iSendingServices.Count() );
    
    while( --index >= 0)
        {
        if ( iSendingServices[index]->ServiceId() == aServiceUid )
            {
            return index;           
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// QueryCapabilities
// -----------------------------------------------------------------------------
//
TBool CSendUiImpl::QueryCapabilities( 
    TInt index, 
    TSendingCapabilities aRequiredCapabilities )
    {
    if ( index == KErrNotFound )
    	{
    	return EFalse;
    	}

     if ( aRequiredCapabilities.iFlags == TSendingCapabilities::EAllServices )
        { 
        // No capabilities required from service
        return ETrue;
        }
    TSendingCapabilities serviceCap = iSendingServices[ index ]->ServiceCapabilities();
    
    if ( !(serviceCap.iMessageSize == 0 // no size limit set for the service
    || aRequiredCapabilities.iMessageSize == 0 ) // no size limit asked from the client
    && aRequiredCapabilities.iMessageSize > serviceCap.iMessageSize )
        {
        return EFalse;
        }
    
    // Check if support for attachments are required.
    if ( aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsAttachments
        && serviceCap.iFlags & TSendingCapabilities::ESupportsAttachments )
        {
        return ETrue;
        }

    // Check if support for body text is required.
    if ( aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsBodyText )
        {
        if ( serviceCap.iFlags & TSendingCapabilities::ESupportsBodyText )
            {
            if ( serviceCap.iBodySize > 0 && aRequiredCapabilities.iBodySize > serviceCap.iBodySize )
                {
                return EFalse; // size limit defined and it is enough
                }
            // no size limit in service for bodytext or required size below max size.
            return ETrue;
            }
        }
        
    
    // Check if support for BIO sending is required.
        if ( aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsBioSending 
             && serviceCap.iFlags & TSendingCapabilities::ESupportsBioSending )
            {
            return ETrue;
            }

    // Check if support for editor is required.
        if ( aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsEditor
             && serviceCap.iFlags & TSendingCapabilities::ESupportsEditor )
            {
            return ETrue;
            }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSendUiImpl::GetServicesL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CSendUiImpl::GetServicesL( )
{
    TInt value(0);
    TInt serviceCount;
    TInt cleanupItems(0);
    
    CRepository* rep = CRepository::NewLC( KCRUidSendUi );
    cleanupItems++;
    TInt err = rep->Get( KKeySendUiServiceCount, serviceCount );
    
    if ( err )
        {
        LOGTEXT(_L("SendUiImpl->GetServicesL: Cenrep init file missing"));
        CleanupStack::PopAndDestroy( rep );
        return;
        }
    else
        {
        if ( serviceCount == KSendUiNotInitialized )
            {
            // Services has not been initialized, launch service resolver in the bacground.
            CleanupStack::PopAndDestroy( rep);
            // iSendingServices are updated, when update is done.
            RProperty::Set( 
                KPSUidSendUiProperty, 
                KSendUiUpdateRequest, 
                KSendUiUpdateRequested); 

            return;            
            }
        }
    
    while ( iSendingServices.Count() )
        {
        delete iSendingServices[0];
        iSendingServices.Remove(0);
        }
        
    HBufC* text = HBufC::NewLC( KSendUiServiceMaxNameLength );
    cleanupItems++;
    TPtr ptr = text->Des();
    TInt key;
    for( TInt i(0); i < serviceCount; i++ )
        {
        key = KSendUiServiceIndex * i;
        CSendingServiceInfo* info = CSendingServiceInfo::NewLC();
    
        err = rep->Get( key | KSendUiServiceId, value );
        if (!err )
            {
            info->SetServiceId( TUid::Uid( value  ));
            }

        err = rep->Get( key | KSendUiServiceProviderId, value );
        if (!err )
            {
            info->SetServiceProviderId( TUid::Uid( value  ));
            }

       err = rep->Get( key | KSendUiServiceName, ptr );
        if (!err)
            {
            __ASSERT_DEBUG( ptr.Length() <= KSendUiServiceMaxNameLength, User::Invariant() );
            info->SetServiceNameL( ptr );
            }
        
        err = rep->Get( key | KSendUiServiceMenuName, ptr );
        if (!err)
            {
            __ASSERT_DEBUG( ptr.Length() <= KSendUiServiceMaxNameLength, User::Invariant() );
            info->SetServiceMenuNameL( ptr );
            }
        
        err = rep->Get( key | KSendUiServiceAddress, ptr );
        if (!err)
            {
            __ASSERT_DEBUG( ptr.Length() <= KSendUiServiceMaxNameLength, User::Invariant() );
            info->SetServiceAddressL( ptr );
            }
        
        TSendingCapabilities cap;
            
        err = rep->Get( key | KSendUiServiceFlags, value );
        if (!err)
            {
            cap.iFlags = value;
            }
        
        err = rep->Get( key | KSendUiServiceBodySize, value );
        if (!err)
            {
            cap.iBodySize = value;
            }
        
        err = rep->Get( key | KSendUiServiceMessageSize, value );
        if (!err)
            {
            cap.iMessageSize = value;
            }
    
    
        err = rep->Get( key | KSendUiServiceTechnologyType, value );
        if (!err )
            {
            info->SetTechnologyTypeId( TUid::Uid( value  ));
            }
        
        err = rep->Get( key | KSendUiServiceFeatures, value );
        if (!err)
            {
            info->SetServiceFeatures( value );
            }
    
        if (!err)
            {
            info->SetServiceCapabilities( cap );            
            iSendingServices.Append( info );
            CleanupStack::Pop( info);
            }

        }
    
    CleanupStack::PopAndDestroy( cleanupItems, rep); // text, rep
    
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
    // Show list query.
    TInt choice = KErrNotFound;
    CAknListQueryDialog* dlg = new (ELeave) CAknListQueryDialog( &choice );
    TInt ret = 0;
    TRAPD( err, 
    dlg->PrepareLC( R_SENDUI_LIST_QUERY );
    dlg->SetItemTextArray( aListItems );
    dlg->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // Set title
    CAknPopupHeadingPane* heading = dlg->Heading();

    if ( aTitleText != KNullDesC )
        {
        heading->SetTextL( aTitleText );
        }
    else 
        {
        HBufC* title;
        TInt titleId(0);
        if (aMenuType == CSendUi::ESendMenu )
            {
            titleId = R_SENDUI_LIST_QUERY_TITLE ;
            }
        else if ( aMenuType == CSendUi::EWriteMenu )
            {
            titleId = R_SENDUI_LIST_QUERY_TITLE_WRITE;
            }
        
        if ( titleId )
            {
            title = iCoeEnv->AllocReadResourceLC( titleId );
            heading->SetTextL( *title );
            CleanupStack::PopAndDestroy( title );
            }
        }
    
    ret = dlg->RunLD();
    ); //end of TRAPD
    
    if( err == KLeaveExit )
        {
        delete dlg;
        dlg = NULL;
        }
    
    if ( !ret )
        {
        return KNullUid;
        }
    return aListItemUids.At( choice );
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
    // Other ecoms are filtered by their base provider, but versitile MTMs under 
    // GenericMtmPlugin have filtering by their serviceUid    
    if ( aServiceProviderId != KGenericMtmPlugin )
    {
        aServiceUid = aServiceProviderId;
    }
    // Content based filtering is done only for some selected services.
    // Cannot filter with no data, certain services and audio message (if no audiomessage support)
    if ( !aMessageData ||
         !( aServiceUid == KDrmLinkSender || 
            aServiceUid == KSenduiMtmPostcardUid ||
            aServiceUid == KMmsDirectUpload ||
            aServiceUid == KMmsIndirectUpload ||
            aServiceUid == KSenduiMtmAudioMessageUid ||
            aServiceUid == KSenduiMtmSmsUid ) )                            
        {
        return ETrue;
        }


    const CDesCArray& attachmentPaths = aMessageData->AttachmentArray();
    const RArray<RFile>& attachments = aMessageData->AttachmentHandleArray();

    TInt attachmentCount = attachmentPaths.Count() + attachments.Count();

    if ( attachmentCount == 0 )
        { // No content to send
        
        if ( aServiceUid == KDrmLinkSender )
            {
            return EFalse;
            }
        TInt bodyTextLength = aMessageData->BodyText()->DocumentLength();
        // Dim MMS Upload services, if there is no bodytext
        if ( ( aServiceUid == KMmsDirectUpload 
            || aServiceUid == KMmsIndirectUpload ) 
            && bodyTextLength == 0 )
            {
            return EFalse; // bodytext will be converted into attachment
            }
        // Dim Audio message, if there is bodytext    
        if ( aServiceUid == KSenduiMtmAudioMessageUid 
            && bodyTextLength > 0 )
            {
            return EFalse;
            }
        return ETrue;
        }
    else if ( attachmentCount == 1  )
        {
        TBool result = ETrue;
        
        switch ( aServiceUid.iUid )
            {
            case KSenduiMtmSmsUidValue: 
                {
                if ( aMimeType->Des8().CompareF( KMsgMimeVCard ) != 0 &&
                     aMimeType->Des8().CompareF( KMsgMimeVCal ) != 0 )
                    {
                    result = EFalse;
                    }
                break;
                }
            case KSenduiMtmPostcardUidValue:
                {
                if( aMimeType->Des8().CompareF( KMsgMimeImageJpeg ) != 0 &&
                    aMimeType->Des8().CompareF( KMsgMimeImageGif ) != 0 &&
                    aMimeType->Des8().CompareF( KMsgMimeImagePng ) != 0 &&
                    aMimeType->Des8().CompareF( KMsgMimeTextPlain ) != 0 )
                    {
                    // Supported types: jpeg, png, gif, plain text
                    result = EFalse;
                    }
                }
                break;   
            case KSenduiMtmAudioMessageUidValue:
                {
                if( aMimeType->Des8().CompareF( KMsgMimeAudioAmr ) != 0 ||
                    aMessageData->BodyText()->DocumentLength() > 0 )
                    {
                    // Does not allow other than amr attachment and empty bodytext
                    result = EFalse;
                    }
                }    
                break;    
            case KDrmLinkSenderValue:
                {
                // Check OMA DRM level

                TInt drmMethod = 0;
                CDRMHelper::TDRMHelperOMALevel omaDRMLevel;
                CDRMHelper* drmHelper = NULL;
                TRAPD( error, drmHelper = CDRMHelper::NewL( iFs ) );
                if ( error != KErrNone )
                    {
                    return error;
                    }
                error = drmHelper->SupportedDRMMethods2( drmMethod, omaDRMLevel );
                if ( !(omaDRMLevel & CDRMHelper::EOMA_2_0 ))
                     {
                     result = EFalse;
                     break;
                     }
                delete drmHelper;


                if ( attachmentPaths.Count() )
                    {
                    RFile handle = SendUiUtils::OpenFileHandleL( attachmentPaths.MdcaPoint(0), iFs );
                    CleanupClosePushL( handle );
                    result = SendUiUtils::HasContentUrlL( handle );
                    CleanupStack::PopAndDestroy( &handle );
                    }
                else
                    {
                    result = SendUiUtils::HasContentUrlL( const_cast<RFile&>( attachments[0] ) );
                    }
                }
                break;
            default:
                // Should never end up here
                break;
            }
     
        return result;        
        }
    else if ( attachmentCount > 1  )
        {
        if ( aServiceUid == KDrmLinkSender ||
             aServiceUid == KSenduiMtmPostcardUid ||
             aServiceUid == KSenduiMtmAudioMessageUid ||
             aServiceUid == KSenduiMtmSmsUid )
            {
            // Currently supports sending of one file only
            // or no files (SMS)
            return EFalse;
            }
        return ETrue;
        }
   
   // Should never end up here
   return ETrue;
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
    TInt i = 0;
    TInt cc = 0;
    TBool dimService = EFalse;
	TDataType* mimeType = NULL;
	TInt cleanupItems(0);
    // Filter some services away if there is no content to be send.
    if ( !aMessageData )
        {
        iServicesToDim->AppendL( KDrmLinkSender );
        iServicesToDim->AppendL( KMmsDirectUpload );
        iServicesToDim->AppendL( KMmsIndirectUpload );
        iServicesToDim->AppendL( KShareOnline );
        }
    else if ( aMessageData->AttachmentArray().Count() == 1
            || aMessageData->AttachmentHandleArray().Count() == 1 )
    	{
    	// CanSendContent can recycle these in mime type resolving
    	// for all services that have attachments
		mimeType = new (ELeave) TDataType();
		CleanupStack::PushL( mimeType );
		cleanupItems++;
				
	    if ( aMessageData->AttachmentArray().Count() )
            {
            RFile handle = SendUiUtils::OpenFileHandleL(
                aMessageData->AttachmentArray().MdcaPoint( 0 ), iFs );
            CleanupClosePushL( handle );
            SendUiUtils::ResolveMimeTypeL( handle, *mimeType );
            CleanupStack::PopAndDestroy( &handle );
            }
        else
            {
            SendUiUtils::ResolveMimeTypeL( aMessageData->AttachmentHandleArray()[0], *mimeType );
            }
    	}
    TBool addMailService = EFalse;
    TUid integratedEmailClientMtmUid;
		
    if ( iEmailClientIntegration )
    	{
    	CRepository* repository = NULL;
    	TInt mtmPluginID = 0;
    	TRAPD( ret, repository = CRepository::NewL( KCRUidSelectableDefaultEmailSettings ) );
    	
    	if(ret == KErrNone)
    	    {
    	    //Get Email application mtm plugin ID
    	    repository->Get( KIntegratedEmailAppMtmPluginId, mtmPluginID ); 
    	    }

    	delete repository;
    	integratedEmailClientMtmUid = TUid::Uid(mtmPluginID);
    	for( i = 0 ; i < iSendingServices.Count(); i++ )
    		{
    		TUid serviceUid = iSendingServices[i]->ServiceId();
    		// Check if it is found in sendingServices and it's valid, then hide
    		if ( integratedEmailClientMtmUid == iSendingServices[i]->ServiceId() && 
    				CheckServiceCapabilities( serviceUid, aRequiredCapabilities ) &&
    				CanSendContentL( serviceUid, 
    							     iSendingServices[i]->ServiceProviderId(), 
    							     aMessageData, mimeType ) )
    			{
    			addMailService = ETrue;
    			}
    		}
    	
    	}

    // Filter available ECom services and add to list item array.
    // Capability based filtering: CheckServiceCapabilities
    // Content based filtering: CanSendContentL
    // "dimmable array" filtering: done here
    TInt serviceFeatures;
    for( i = 0 ; i < iSendingServices.Count(); i++ )
        {
        dimService = EFalse;
        TUid serviceUid = iSendingServices[i]->ServiceId();
        TUid serviceProviderUid = iSendingServices[i]->ServiceProviderId();
        
        if ( iSendingServices[i]->ServiceMenuName() == KNullDesC )
            {
            dimService = ETrue;
            }
        
        TUid serviceTechnologyType = iSendingServices[i]->TechnologyTypeId();
        TUid dimServiceUid;
        for ( cc = 0; cc < iServicesToDim->Count(); cc++ )
            {
			dimServiceUid = (*iServicesToDim)[cc];
            if ( dimServiceUid == serviceUid 
            || dimServiceUid == serviceProviderUid
            || dimServiceUid == serviceTechnologyType 
            )
                {
                dimService = ETrue;
                break;
                }
            }
        if ( iEmailClientIntegration )
            {
            //Hide Email application (not the platform email) from "send" menu
            if( integratedEmailClientMtmUid == iSendingServices[i]->ServiceId() )
                {
                dimService = ETrue;
                }  
            }
		serviceFeatures = iSendingServices[i]->ServiceFeatures();
        if ( iEmailClientIntegration )
        	{
        	//If there is a mailbox created by integrated mail client then 
        	//put SMTP MTM ("Email") in the "send" menu, by changing the 
        	//serviceFeature    
        	if ( addMailService && 
        		 ( iSendingServices[i]->ServiceId() == KUidMsgTypeSMTP ) )
        		{
        		serviceFeatures = 0;
        		addMailService = EFalse; //MailService must be added only once
        		}
        	}       
        if ( !dimService &&
             !( serviceFeatures & CSendingServiceInfo::EServiceInValid ) &&
             !( serviceFeatures & CSendingServiceInfo::EServiceHidden )
             &&  CheckServiceCapabilities( serviceUid, aRequiredCapabilities ) &&
             CanSendContentL( serviceUid, serviceProviderUid, aMessageData, mimeType ) )
            {
            if ( aMenuType == CSendUi::ESendMenu )
                {
                aListItems.AppendL(iSendingServices[i]->ServiceMenuName() );
                }
            else if ( aMenuType == CSendUi::EWriteMenu )
                {
                aListItems.AppendL(iSendingServices[i]->ServiceName() );
                }

            aListItemUids.AppendL( serviceUid );
            }
        }
    CleanupStack::PopAndDestroy( cleanupItems );
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
    TInt insertIndex(0);
    TUid positionUid;   
    TInt j;
    // set the services into correct order. The order is predefined in the resources.
    for ( TInt i = 0; i < iSendUiServicesOrderArray->Count() ; i++ )
        {
        positionUid = TUid::Uid( (*iSendUiServicesOrderArray)[i] );
        for (j = insertIndex; j < aListItems.Count(); j++ )
            {
            TUid tmp = aListItemUids[j];
            if ( positionUid == aListItemUids[j] ) 
                {
                if ( j >= insertIndex )
                    {
                    HBufC* serviceName = aListItems[j].AllocL();
                    CleanupStack::PushL( serviceName );

                    aListItemUids.Delete( j );
                    aListItems.Delete( j );

                    aListItemUids.InsertL( insertIndex, positionUid );
                    aListItems.InsertL( insertIndex, *serviceName );
                    CleanupStack::PopAndDestroy( serviceName );
                    insertIndex++;      
                    }
                else
                    {
                    // service already on the right place. Next.
                    insertIndex++;  
                    }
                    
                }
            }
        }
    }


// ---------------------------------------------------------
// ResetIsSending
// Resets sending state flag.
// ---------------------------------------------------------
//    
void CSendUiImpl::ResetIsSending( TAny* aAny )
    {
    *( (TBool*)aAny ) = EFalse;
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
    TInt i = 0;
    TSendingCapabilities serviceCapabilities;

    // No capabilities defined, uses content-based filtering
    // or no capabilities required from service
    if ( aServiceUid == KDrmLinkSender 
         || aRequiredCapabilities.iFlags == TSendingCapabilities::EAllServices )
        { 
        return ETrue;
        }

    i = ResolveServiceIndex( aServiceUid );

    if ( i != KErrNotFound )
        {
        return QueryCapabilities( i, aRequiredCapabilities );  
        }
    else
        {
        return EFalse;          
        }
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
    if ( FeatureManager::FeatureSupported( aFeature ) )
        {
        // If Postcard only is supported,     aFeatureFlag is 0x00000001 ==> aNewFeatures == 0x00000001
        // If Audio message is also supported aFeatureFlag is 0x00000002 ==> aNewFeatures == 0x00000003
        // If MMSUpload is also supported,    aFeatureFlag is 0x00000004 ==> aNewFeatures == 0x00000007
        aNewFeatures |= aFeatureFlag;
        }
    }
// ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendUiImpl::ConstructL()
    {
    iSingleton = CSendUiSingleton::NewL();
    //User::LeaveIfError( iFs.Connect() );
    iFs.ShareProtected();

    TParse* fp = new(ELeave) TParse(); 
    fp->Set( KSendUiResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    iResourceFileName = fp->FullName();
    delete fp;
    fp = NULL;
    
    iResourceLoader.OpenL( iResourceFileName );

    // Check for server launch
    RProcess process;
    TUid hostId = process.SecureId();
    if ( hostId == KSendUiLauncherAppUid ) 
        {
        iServerAppLaunch = ETrue;
        }
   else
        {
        iServerAppLaunch = EFalse;
        iSendUiServicesOrderArray = 
            new ( ELeave ) CArrayFixFlat<TInt32>( KSendUiServiceOrderArrayGranularity );

        TResourceReader reader;
        iCoeEnv->CreateResourceReaderLC( reader, R_SENDUISERVICEORDER_ARRAY );
        const TInt count = reader.ReadInt16();
        for ( TInt loop = 0; loop < count; loop++ )
            {
            iSendUiServicesOrderArray->AppendL( reader.ReadInt32() );
            }
        CleanupStack::PopAndDestroy( ); // reader 
        }

    iPropertyObserver = CPropertyObserver::NewL( this );
    TRAPD( err, GetServicesL() );
    
    if ( err )
        {
#ifdef _DEBUG
        TBuf<256> buf;
        buf.Format(_L("SendUi err"));
        User::InfoPrint(buf);
        LOGTEXT2(_L("Error %d while loading CenRep"), err );
#endif
        return;
        }
    iServicesToDim = new (ELeave) CArrayFixFlat<TUid>( KArrayGranularity );
    FeatureManager::InitializeLibL();
    
    if ( FeatureManager::FeatureSupported( KFeatureIdEmailMceIntegration ) )
    	{

    	iEmailClientIntegration = ETrue;
    	}
    else
    	{

    	iEmailClientIntegration = EFalse;
    	}   
    FeatureManager::UnInitializeLib();
    }

// end of file

