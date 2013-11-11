/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  LaunchBrowser command handler
*
*/


#ifndef CLAUNCHBROWSERHANDLER_H
#define CLAUNCHBROWSERHANDLER_H

#include <etelsat.h>
#include <etelpckt.h>
#include <w32std.h>             // WsSession
#include <apgtask.h>            // TApaTask
#include <DocumentHandler.h>    // KWmlcHandler

#include "CSatCommandHandler.h"
#include "SatSTypes.h"          // TSatQuery

class MSatUtils;
class RCmManager;
class RCmConnectionMethod;

/**
 *  Command handler for LaunchBrowser command.
 *
 *  @lib LaunchBrowserCmd.lib
 *  @since S60 v3.0
 */
class CLaunchBrowserHandler : public CSatCommandHandler
    {
    
public:

    static CLaunchBrowserHandler* NewL( MSatUtils* aUtils );

    virtual ~CLaunchBrowserHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client.
     *
     */
    void ClientResponse();

// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler.
     * Event notification.
     *
     * @param aEvent An event that is occured.
     */
    void Event( TInt aEvent );

protected:

// from base class CActive

    /**
     * From CActive.
     * Cancels the usat request.
     *
     */
    void DoCancel();
    
// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler.
     * Requests the command notification.
     *
     * @param aStatus Request status
     */
    void IssueUSATRequest( TRequestStatus& aStatus );

    /**
     * From CSatCommandHandler.
     * Precheck before executing the command.
     *
     * @return TBool indicating command is currently allowed.
     */
    TBool CommandAllowed();

    /**
     * From CSatCommandHandler.
     * Need for ui session.
     *
     * @return TBool indicating need of ui session.
     */
    TBool NeedUiSession();

    /**
     * From CSatCommandHandler.
     * Called when USAT API notifies that command.
     *
     */
    void HandleCommand();

    /**
     * From CSatCommandHandler.
     * Indicates the failure of launching ui client.
     *
     */
    void UiLaunchFailed();

private:

    CLaunchBrowserHandler();

    void ConstructL();

    /**
     * Checks if Browser application is launched.
     *
     * @return The result of the operation.
     */
    TBool BrowserExists();

    /**
     * Closes the Browser application if it is active.
     *
     * @return The result of the operation.
     */
    void CloseBrowser();

    /**
     * Launches The Browser with URL.
     *
     * @param aParam, Reference to the parameter for the Browser
     * application.
     * @param aAccessPointUid, Reference to the accesspoint uid.
     */
    void LaunchWithUrlL( 
        const TDesC& aParam,
        const TUid& aAccessPointUid );

    /**
     * Handle the Launch Browser command.
     *
     */
    void LaunchBrowserL();

    /**
     * Starts timer. Function returns as the given time has elapsed or
     * timer is cancelled.
     *
     * @param aDelay Delay from the function call to complete function in
     *               microseconds
     * @param aInterval Interval to complete wait after aDelay, zero by
     *                  default
     */
    void After(
        const TTimeIntervalMicroSeconds32& aDelay,
        const TTimeIntervalMicroSeconds32& aInterval = 0 );

    /**
     * Called after given delay in After function.
     * Causes the After function to complete
     *
     * @param aPtr Pointer to itself since this is static function
     * @return whether to call function again after an interval
     */
    static TInt DelayCallBack( TAny* aPtr );

    /**
     * Create a new APN with given parameters using Access Point engine.
     *
     * @param aApName APN name.
     * @param aProxyServerAddr Proxy/Gateway server address.
     * @param aProxyPortNumber Proxy server port number.
     * @param aIPvType Gateway address IP type.
     *
     * @return the ID for the new APN
     */
    TUint32 CreateAccessPointL( const TDesC16& aApName,
        const TDesC16& aProxyServerAddr,
        const TUint32 aProxyPortNumber,
        const RPacketContext::TProtocolType& aIPvType );

    /**
     * Select an Access Point by the given gateway address. Create one if
     * there is not a corresponding AP.
     * 
     * @return IAP id from GCF connection method.
     */
    TUint32 GetAccessPointByGatewayAddressL();
    
    /**
     * Get an Access Point by default.
     *
     * @return IAP id from default connection method.
     */ 
    TUint32 GetAccessPointByDefaultL();

    /**
     * Get default Url by access point id.
     *
     * @param aApId Input IAP id by given connection method.
     * @return Default Url by given connection method.
     */    
    HBufC* GetDefaultUrlByAccessPointLC( TUint32 aApId );

    /**
     * A gateway address is required to be a IP address in 
     * connection method, so it is necessary to check 
     * the validity of the gateway address to avioding leave in
     * APE.
     *
     * @param aAddress A gateway address.
     * @return EIPv4 if the gateway address is a valid IPv4 address.
     *         EIPv6 if the gateway address is a valid IPv6 address.
     *         otherwise return 0 to indicate a invalid IP address.
     */
    TInt ValidateGatewayAddress( const TDesC16& aAddress );

    /**
     * Check the validity of a IPv4 address.
     *
     * @param aAddress A IPv4 address.
     * @return ETrue if the address is a valid IPv4 address.
     */
    TBool ValidateIPv4Address( const TDesC16& aAddress );

    /**
     * Check the validity of a IPv6 address.
     *
     * @param aAddress A IPv6 address.
     * @return ETrue if the address is a valid IPv6 address.
     */
    TBool ValidateIPv6Address( const TDesC16& aAddress );

    /**
     * Convert a descriptor to a integer.
     *
     * @param aAddress A descriptor.
     * @return Integer value.
     */
    TInt ConvertDesToInt( const TDesC16& aValue );
         
    /**
     * Check the validity of a descriptor in hex.
     *
     * @param aAddress A descriptor.
     * @return ETrue if each character in the descriptor is a valid 
     *         hexadecimal.
     */
    TBool ValidateHexadecimal( const TDesC16& aHex );
        
    /**
     * Separate the port number from a address if it is existing. For example, 
     * 210.13.246.172:8080.
     *
     * @param aAddress A address.
     * @param aPureAddr The pure address without any additional info.
     * @param aPortNumber The port number.
     * @return None.
     */
    void SeparatePortNumberFromAddress( const TDesC16& aAddress, 
        TDes16& aPureAddr, TUint32& aPortNumber );
         
    /**
     * Fulfill connecion method with default params.
     *
     * @param aCmManager Connection method manager.
     * @param aCm Output reference to RCmConnectionMethod.
     * @return None
     */    
    void FulfillConnnectionMethodL( RCmManager& aCmManager,
                                    RCmConnectionMethod& aCm );
   
    /**
     * Check the validity of offered bearer type
     *
     * @param aCm reference to RCmConnectionMethod.
     * @param aIsValid Output KErrNone if connection method is valid.
     * @return None
     */     
    void ValidateBearerTypeL( RCmConnectionMethod& aCm, TInt& aIsValid );
    
    /**
     * Get default connection method.
     *
     * @param aCmManager Input Connection method manager.
     * @param aDefCm Output reference to RCmConnectionMethod.
     * @param aIsValid Output KErrNone if found
     * @return None
     */       
    void GetDefConnMethodL( RCmManager& aCmManager,
                            RCmConnectionMethod& aDefCm,
                            TInt& aIsValid );  
    
private: // data

    /**
     * LaunchBrowser command data.
     */
    RSat::TLaunchBrowserV2 iLaunchBrowserData;

    /**
     * LaunchBrowser command package.
     */
    RSat::TLaunchBrowserV2Pckg iLaunchBrowserPckg;

    /**
     * Response from client
     */
    RSat::TLaunchBrowserRspV2 iLaunchBrowserRsp;

    /**
     * Response package.
     */
    RSat::TLaunchBrowserRspV2Pckg iLaunchBrowserRspPckg;

    /**
     * Query command data
     */
    TSatQueryV1 iQueryData;

    /**
     * Query package
     */
    TSatQueryV1Pckg iQueryPckg;

    /**
     * Query response
     */
    TSatQueryRspV1 iQueryRsp;

    /**
     * Query response package
     */
    TSatQueryRspV1Pckg iQueryRspPckg;

    /**
     * Window server session
     */
    RWsSession iWsSession;

    /**
     * Application UID of the Browser.
     */
    TUid iUidWmlBrowser;

    /**
     * Indicates does this command need UI session ot not
     */
    TBool iNeedUiSession;

    /**
     * Blocker for After
     */
    CActiveSchedulerWait iAfterWait;

    /**
     * Timer used in After
     */
    CPeriodic* iAfterTimer;
                
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;
    };

#endif      // CLAUNCHBROWSERHANDLER_H

