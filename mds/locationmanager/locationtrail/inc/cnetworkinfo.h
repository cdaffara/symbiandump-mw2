/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/


#ifndef C_CNETWORKINFO_H
#define C_CNETWORKINFO_H

#include <etel3rdparty.h>
#include <etel.h>
#include <etelmm.h>

/**
*  An observer interface, which is used for getting current network cell id.
*
*  @since S60 3.1
*/
class MNetworkInfoObserver
    {
public:    
    /**
     * This method is used for setting the network cell id to the 
     * location trail.
     */
    virtual void NetworkInfo( const CTelephony::TNetworkInfoV1 &aNetworkInfo, const TInt aError ) = 0;

    /**
     * Get mobile phone object for network info object
     * @return mobile phone object
     */
    virtual RMobilePhone& GetMobilePhone4NwInfo() = 0;
    };

/**
 *  
 *  @since S60 3.1
 */
class CNetworkInfo : public CActive
    {
public:  
    /**
     * 2-phased constructor.
     * @since S60 3.1
     */
    IMPORT_C static CNetworkInfo* NewL( MNetworkInfoObserver* aTrail );

    /**
     * C++ destructor.
     * @since S60 3.1
     */    
    IMPORT_C virtual ~CNetworkInfo();

    /**
     * Listen to n/w info change
     * @since S60 9.2
     */    
    IMPORT_C void StartNwInfoChangeNotifier();

   /**
     * Stop n/w info change listener
     * @since S60 9.2
     */    
   IMPORT_C void StopNwInfoChangeNotifier();
   
   /**
     * Map etel to ctelephony
     * @since S60 9.2
     */    
   IMPORT_C static void MapNetworkInfo(CTelephony::TNetworkInfoV1* aISVNetworkInfo, 
       RMobilePhone::TMobilePhoneNetworkInfoV2* aMMNetworkInfo, 
       RMobilePhone::TMobilePhoneLocationAreaV1* aMMArea);
    
    /*
    * Get registrer network country code
    *
    * @return current register n/w info
    */
    RMobilePhone::TMobilePhoneNetworkInfoV2& GetCurrentRegisterNw();
    
protected:
    /**
     * Run error implementation in case of RunL leaving.
     * @since S60 3.1
     */
    TInt RunError( TInt aError );
    
private:
    /**
     * C++ constructor.
     */  
    CNetworkInfo( MNetworkInfoObserver* aTrail );
    
    /**
     * 2nd phase constructor.
     */
    void ConstructL();
    
private:
    /**
    * From CActive.
    */        
    void DoCancel();
    
    /**
    * From CActive.
    */        
    void RunL(); 

private:
   typedef enum
    {
    ENetworkInfoNoOptState = 0x00,
    ENetworkInfoOptGetState,
    ENetworkInfoOptNotifyState
    }TNetworkInfoOptState;
   
    /**
     * An observer interface to set current cell id to the location trail.
     * Not own.
     */
    MNetworkInfoObserver* iTrail;
    
    /**
     * phone object to retrieve modem parameters
     */ 
    RMobilePhone& iMobilePhone;

    /**
     * Maintain the operation state.
     */ 
    TNetworkInfoOptState iState;

    /*
    * Retained this object just to give backward compatibility
    */
    CTelephony::TNetworkInfoV1 iNetworkInfoV1;

	RMobilePhone::TMobilePhoneNetworkInfoV2 iMMNetworkInfo;
	RMobilePhone::TMobilePhoneNetworkInfoV2Pckg iMMNetworkInfoPckg;
	RMobilePhone::TMobilePhoneLocationAreaV1 iMMArea;
    };

#endif // C_CNETWORKINFO_H

// End of file.
