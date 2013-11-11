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
* Description:   SendUI API implementation
*
*/





#ifndef C_CSENDUIIMPL_H
#define C_CSENDUIIMPL_H

#include <f32file.h>
#include <e32base.h>
#include <e32property.h>
#include <badesca.h>
#include <ConeResLoader.h>
#include <apaserverapp.h>
#include <msvapi.h>

#include <TSendingCapabilities.h>
#include <sendui.h>
#include <SendUiConsts.h>

class CEikMenuPane;
class CSendNormalUi;
class CMessageData;
class CSendingServiceManager;
class CSendingServiceInfo;
class RSendUiServiceClient;
class CPropertyObserver;
class TDataType;
class CSendUiSingleton;

/**
 *  Implementation of SendUI API
 *
 *  Can be used to show "Send" list query and to create and send
 *  messages via available services. Sending services can be based
 *  on the MTM or ECom architecture. Created messages are sent
 *  directly or message editor is opened for editing, depending on
 *  the type of selected service.
 *  @lib sendui.lib
 *  @since S60 v3.0
 */
class CSendUiImpl : public CBase
    {
    friend class UT_CSendUiImpl;
public:

    static CSendUiImpl* NewL();

    virtual ~CSendUiImpl();

public:

    /**
    * Adds menu item of given type to menupane.
    *
    * @since Series 60 3.0
    * @param aMenuType Type of the menu, e.g. "ESendMenu" or "EWriteMenu"
    * @param aMenuPane Menupane where the menu item should be added.
    * @param aIndex The place of the menu item in menupane.
    * @param aCommandId Command id for the menu item.
    * @param aRequiredCapabilities Capabilities required by services to be
    *        shown in "Send" list query. If no capabilities are required
    *        (KCapabilitiesForAllServices), all available services are
    *        shown in "Send" list query.
    * @return None.
    */
    void AddTypedMenuItemL(
        CSendUi::TSendUiMenuType aMenuType,
        CEikMenuPane& aMenuPane,
        TInt aIndex,
        TInt aCommandId,
        TSendingCapabilities aRequiredCapabilities = KCapabilitiesForAllServices );
    
    /**
    * First displays list query and then creates the message.
    * Editor is launched for editing the message or the message is sent
    * directly without opening the editor. Functionality depends on the
    * type of selected service.
    *
    * List query can be manipulated in three ways. All three methods can be
    * used or just some of those.
    *
    * The first option is to set the required capabilities for services.
    * Eg. attachments supported. Sending services not matching to required
    * capabilities are filtered away. Capabilities are defined in
    * TSendingCapabilities.h.
    *
    * The second option is content based filtering. Filtering is based on
    * the files intended to send. Those files can be set as part of message
    * data. Filtering is done according the predefined rules.
    *
    * The third option is to set the array of service uids not wanted to
    * be shown in list query. Uid can be the technology type, service provider or
    * service id. These sending services are filtered away
    * even if those match to required capabilities.
    *
    * @since Series 60 3.0
    * @param aMenuType Type of the menu, e.g. "ESendMenu" or "EWriteMenu"
    * @param aMessageData Data for the message.
    * @param aRequiredCapabilities Capabilities required by sending services
    *        to be shown in "Send" pop-up list query. If no capabilities are
    *        required (KCapabilitiesForAllServices), all available services
    *        are shown in "Send" pop-up list query.
    * @param aServicesToDim Array of service uids (service uid, service 
    *        provider's uid or service's TechnologyType) _NOT_ wanted to "Send" 
    *        list query. 
    *        See predefined sending service uids in SendUiConsts.h .
    * @param aLaunchEditorEmbedded ETrue if the editor should be launched
    *        embedded. Otherwise the editor is launched stand-alone.
    *        Note: some services sends the messages without launching the
    *        editor at all.
    * @param aTitleText Title of the list query. If no title is defined,
    *        then the localised default title is used, e.g. "Send" or 
    *        "Write".
    * @return None.
    */
    void ShowTypedQueryAndSendL(
        CSendUi::TSendUiMenuType aMenuType, 
        const CMessageData* aMessageData,
        TSendingCapabilities aRequiredCapabilities = KCapabilitiesForAllServices,
        CArrayFix<TUid>* aServicesToDim = NULL,
        TBool aLaunchEditorEmbedded = ETrue,
        const TDesC& aTitleText = KNullDesC );
    
    /**
    * Displays list query and returns the user selection.
    *
    * List query can be manipulated in three ways. All three methods can be
    * used or just some of those.
    *
    * The first option is to set the required capabilities for services.
    * Eg. attachments supported. Sending services not matching to required
    * capabilities are filtered away. Capabilities are defined in
    * TSendingCapabilities.h.
    *
    * The second option is content based filtering. Filtering is based on
    * the files intended to send. Those files can be set as part of message
    * data. Filtering is done according the predefined rules.
    *
    * The third option is to set the array of service uids not wanted to
    * be shown in list query. These sending services are filtered away
    * even if those match to required capabilities.
    * 
    * @since Series 60 3.0
    * @param aMenuType Type of the menu, e.g. "ESendMenu" or "EWriteMenu"
    * @param aMessageData Data for the message. Attachments are used for
    *        content based filtering.
    * @param aRequiredCapabilities Capabilities required by sending services
    *        to be shown in list query. If no capabilities are required
    *        (KCapabilitiesForAllServices), all available services are 
    *        shown in list query.
    * @param aServicesToDim Array of service uids (service uid, service 
    *        provider's uid or service's TechnologyType) _NOT_ wanted to "Send" 
    *        list query. 
    *        See predefined sending service uids in SendUiConsts.h .
    * @param aTitleText Title of the list query. If no title is defined,
    *        then the localised default title is used, e.g. "Send" or 
    *        "Write"
    * @return Uid of the selected service (MTM or ECom). 
    *        KNullId is returned if user selects cancel.
    */
    TUid ShowTypedQueryL(
        CSendUi::TSendUiMenuType aMenuType, 
        const CMessageData* aMessageData = NULL,
        TSendingCapabilities aRequiredCapabilities = KCapabilitiesForAllServices,
        CArrayFix<TUid>* aServicesToDim = NULL,
        const TDesC& aTitleText = KNullDesC );
    
    /**
    * Creates the message and launches the editor for editing the message
    * or sends the message directly without opening the editor. 
    * Functionality depends on the type of selected service.
    *
    * @since Series 60 3.2
    * @param aServiceUid Uid of the sending service.
    * @param aMessageData Data for the message.
    * @return None.
    */
    TInt SendMessageL( 
        TUid                    aServiceUid,
        const CMessageData*     aMessageData,
        TBool                   aLaunchEditorEmbedded );

    /**
    * Launches server application, which will eventually call SendMessageL
    * with the same parameters.
    *
    * @since Series 60 3.2
    * @param aServiceUid Uid of the sending service.
    * @param aMessageData Data for the message.
    * @return None.
    */
    void SendMessageViaServerL( 
        TUid                    aServiceUid,
        const CMessageData*     aMessageData,
        TBool                   aLaunchEditorEmbedded );
    /**
    * Creates the message and launches the editor for editing the message
    * or sends the message directly without opening the editor. 
    * Functionality depends on the type of selected service.
    *
    * @since Series 60 3.0
    * @param aServiceUid Uid of the sending service.
    * @param aMessageData Data for the message.
    * @return None.
    */
    void CreateAndSendMessageL(
        TUid aServiceUid,
        const CMessageData* aMessageData,
        TBool aLaunchEditorEmbedded = ETrue );
    
    /**
    * Returns sending capabilities of the sending service.
    * @since Series 60 3.0
    * @param aServiceUid Uid of the sending service.
    * @param aServiceCapabilities TSendingCapabilities of the sending
    *        service as a return value.        
    * @return Return KErrNone if successful, or one of the system wide
    *         errors if unsuccessful.
    */
    TInt ServiceCapabilities(
        TUid aServiceUid,
        TSendingCapabilities& aServiceCapabilities );       
    
    /**
    * Populates given list with the information about services
    * provided by specified ECom service provider.
    * Each service provider can provide 1..n services.
    * Ownership of the pointed objects are transfered to caller.
    *
    * Can be used for ex. when creating own "Send" menu, which needs to
    * include ECom based services.
    *
    * NOTE: Can be used only for ECom based services, also for MTMs
    *
    * @since Series 60 2.8
    * @param aServiceList Service list to be populated.
    * @param aServiceProvider Id of the specified service provider.
    *        KNullId means all service providers.
    *        Service providers are defined in SendUiConsts.h
    * @return none
    */
    void AvailableServicesL(
        RPointerArray<CSendingServiceInfo>& aServiceList,
        TUid aServiceProvider = KNullUid);
    
    /**
    *   Returns the technology type of a service
    *
    *   @since Series 60 3.0
    *   @param aServiceId Service of interest
    *   @return Technology type of the provided service. NULL if Mtm
    *           service doesn't exist or service is ECOM based.
    */
    TUid TechnologyType( TUid aServiceUid );
    
    /**
    *   Returns the index of the service in service array
    *
    *   @since Series 60 3.2
    *   @param aServiceId Service of interest
    *   @return Index of a found service. Otherwise KErrNotFound.
    */
    TInt ResolveServiceIndex( TUid aUid );

    /**
     * Compares service capabilities with the required capabilities
     *
     * @since S60 v3.2
     * @param aServiceIndex Index of the service
     * @param aRequiredCapabilities Required capabilities
     * @return ETrue, if service has required or more capabilities. 
     *         EFalse, if service does not have the required capabilities 
     *         or service does not exist.
     */    
    TBool QueryCapabilities( 
        TInt aServiceIndex, 
        TSendingCapabilities aCap );
    
    /**
     * Copies sending service information from Central Repository to local data
     *
     * @since S60 v3.2
     */
     void GetServicesL( );
    
private:

    
    /**
    * 
    * @since Series 60 3.0
    * @param aListItems
    * @param aTitleText Title of the pop-up list query. If no title
    *        is defined, then the localised default title is used.
    * @return: User selection.
    */
    TUid ShowListQueryL(
        MDesCArray* aListItems,
        const CArrayFix<TUid>& aListItemUids,
        const TDesC& aTitleText,
        CSendUi::TSendUiMenuType aMenuType );
    
    /**
    * Filtering services by their content
    * 
    * @since Series 60 3.0
    * @param aServiceUid
    * @param aServiceProviderUid
    * @param aMessageData Message data, which is base of the content filtering.
    * @param aMimeType The mimetype of the first attachment.
    * @return: True, if given service can send specified content
    */
    TBool CanSendContentL( 
        TUid aServiceUid,
        TUid aServiceProviderUid,
        const CMessageData* aMessageData,
        TDataType* aMimeType );
    
    /**
    * 
    * @since Series 60 3.0
    * @param aListItemUids
    * @param aListItemUids
    * @param aRequiredCapabilities
    * @param aContentArray
    * @param aServicesToDim
    * @return: None.
    */
    void FilterServicesL(
        CDesCArrayFlat& aListItems,
        CArrayFix<TUid>& aListItemUids,
        TSendingCapabilities aRequiredCapabilities,
        const CMessageData* aMessageData,
        CSendUi::TSendUiMenuType aMenuType );
    
    /**
     * Sorts service lists
     *
     * @since S60 v3.0
     * @param aListItems Names of items in array
     * @param aListItemUids Uids of items in array
     * @return: Sorted array
     */
    void SortListL(
        CDesCArrayFlat& aListItems,
        CArrayFix<TUid>& aListItemUids );
    
    /**
    * Resets sending state flag.
    * @since Series 60 3.0
    * @return None.
    */        
    static void ResetIsSending( TAny* aAny );
    
    /**
    * Compares ECom service sending capabilities to required capabilities.
    * @since Series 60 3.0
    * @param aServiceUid Uid of the sending service.
    * @param aRequiredCapabilities Capabilities required for the sending
    *        service. 
    * @return: ETrue if service sending capabilities matches to
    *          required capabilities, otherwise EFalse.
    */
    TBool CheckServiceCapabilities(
        TUid aServiceUid,
        TSendingCapabilities aRequiredCapabilities );
        
    /**
    * Checks if the feature is supported by feature manager.
    * @since Series 60 3.2.3
    * @param aFeature Feature manager id of the sending service.
    * @param aNewFeatures To keep track of which feature is modified
    * @param aFeatureFlag Uid of the sending service used for central repository storage.
    * @return None
    */
    void FeatureStatus(
        TInt aFeature, 
        TInt &aNewFeatures, 
        TInt aFeatureFlag );
private:

    CSendUiImpl();
    
    void ConstructL();
    
private: // data

    /**
     * Pointer to Control environment
     * Not own.
     */
    CCoeEnv* iCoeEnv;

    /**
     * Pointer to File Server Session. Initialized from CCoeEnv->FsSession.
     * Not own.
     */
    RFs& iFs;

    /**
     * Pointer to CSendUiSingleton
     * Own.
     */
    CSendUiSingleton* iSingleton;

    /**
     * Indicates whether or not the sending process is in progress.
     */
    TBool iIsSending;

    /**
     * For UI resource loading
     * Own.
     */
    RConeResourceLoader iResourceLoader;

    /**
     * Services to be dimmed
     * Own.
     */
    CArrayFixFlat<TUid>* iServicesToDim;

    /**
     * Resource file name and path. 
     * Own.  
     */
    TFileName iResourceFileName;
    
    
    TBool iEmailClientIntegration;
    /**
     * Array of available sending service
     * Own. 
     */
    RPointerArray<CSendingServiceInfo> iSendingServices;
    
    /**
     * IPC client with SendUiLauncherApp
     * Own.  
     */
    RSendUiServiceClient* iSendUiClient;
    
    /**
     * The requested order of sending services in the menu.
     * Read from resources.
     * Own.  
     */
    CArrayFixFlat<TInt32>* iSendUiServicesOrderArray;

    TBool iServerAppLaunch; 
    
    CPropertyObserver* iPropertyObserver;

    };

#endif // C_CSENDUIIMPL_H
