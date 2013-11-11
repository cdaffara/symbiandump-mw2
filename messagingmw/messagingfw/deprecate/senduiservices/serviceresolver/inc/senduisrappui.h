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
* Description:   Resolves sending service information and stores it to 
*                Central Repository.    
*
*/





#ifndef C_CSENDUISRAPPUI_H
#define C_CSENDUISRAPPUI_H


#include <aknappui.h>
#include <msvapi.h>

// Forward declarations
class CSendUiPropertyObserver;
class CSendingServiceInfo;
class CSendingService;
class TSendingCapabilities;
class CSendUiSrAppView;
class CRepository;
class CSendUiSingleton;

const TInt KEightBits = 8;

/**
 *  An instance of class CSendUiSrAppUi is the UserInterface part of the AVKON
 *  application framework for the SendUiServiceResolver application
 *
 *  @since S60 v3.2
 */
class CSendUiSrAppUi : public CAknAppUi
    {

public:

    /**  Publish & Subscribe API keys */
    enum TSendUiPropertyKeys { ESendUiListStatus };

    virtual ~CSendUiSrAppUi();

    CSendUiSrAppUi();
    
// from base class CAknAppUi

    /**
     * From CAknAppUi.
     *
     * @since S60 v3.2
     * @param aCommand Command code
     */
    void HandleCommandL( TInt aCommand );
    
private:

    void ConstructL();

    /**
     * Resolves and stores sending service information
     *
     * @since S60 v3.2
     */
    void HandleUpdateSendListL();
    
    /**
     * Removes all SendUi's sending service information from the central repository
     *
     * @since S60 v3.2
     * @param aRep Reference to an open CRepository object
     */
    void ClearPluginListL( CRepository* aRep );
 
    /**
     * Inserts given sending service information to Central Repository.
     *
     * @since S60 v3.2
     * @param aRep          Reference to an open CRepository object
     * @param aServiceId    Identification code of this service
     * @param aServiceProviderId Identification code of the service provider
     * @param aTechnologyType   Identification code of the service's technology type
     * @param aCapabilities     Sending capabilities of this service
     * @param aServiceMenuName  Name of the service shown in menu (Send...)
     * @param aServiceName      Name of the service (Write...)
     */ 
    TInt AddSendingService( 
        CRepository* aRep,
        TUint aIndex, 
        TUid aServiceId,
        TUid aServiceProviderId,
        TUid aTechnologyType,
        TInt aFeatures,
        TSendingCapabilities aCapabilities,
        TPtrC aServiceMenuName,
        TPtrC aServiceName,
        TPtrC aServiceAddress );

    /**
    * Populates given list with the sending services.
    * The pointers in the list point to the actual objects
    * (which remain owned by the Service Providers).
    *
    * @since Series 60 3.2
    * @param aServiceList List of available services
    * @param aServiceProviders list of service providers.
    * @param aCoeEnv
    * @return aServiceList and aServiceProviders
    */
    void PopulateServicesListL(
        RPointerArray<CSendingServiceInfo>& aServiceList,
        RPointerArray<CSendingService>&      aServiceProviders,
        CCoeEnv* aCoeEnv );

    static TInt UpdateTask( TAny* aThis );
    
    /**
     * Loads a service provides and fetches the provided services. 
     * This functions is trapped in PopulateServicesListL to insulate serious
     * service loading problems to the service in question.
     *
     * @since Series 60 3.2
     * @param aServiceProviderUid 
     * @param aCoeEnv Pointer to CCoeEnv passed to the service provider
     * @param aSingleton Pointer to a singleton class passed to the service provider
     * @param aServiceList List of services available at the loaded service provider.
     */
     
    CSendingService* LoadServiceL( 
        TUid aServiceUid, 
        CCoeEnv* aCoeEnv, 
        CSendUiSingleton* aSingleton,
        RPointerArray<CSendingServiceInfo>& aServiceList );
    
private:

      /**
       * Not own.
       */
       CCoeEnv* iCoeEnv;
       
      /**
       * Timer for updater. HandleUpdateSendListL can not be called directly from ConstructL,
       * so HandleUpdateSendListL call is requested after construction with timer.
       *
       * Own.
       */
       CIdle* iIdle;
   };

#endif // C_CSENDUIAPPUI_H
