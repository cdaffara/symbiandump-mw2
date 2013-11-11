/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SUPL Settings class
*
*/


#ifndef __EPOS_CSUPLSETTINGPARAMS_H__
#define __EPOS_CSUPLSETTINGPARAMS_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CONSTANTS

// CLASS DECLARATION

/**
 * This class is used to specify/retrieve the desired server properties.  The required memory for
 * the parameters will be allocated during construction of this class and this class owns
 * the memory allocated.  And same will be released when this class is destroyed.  The 
 * server property includes 
 *  - Supported protocol version.
 *  - Emergency support.
 *  - TLS enabled / disabled.
 *  - PSK used / not used ( GBA or any other key generation mechanism ).
 *  - IAP / destination.
 *  - SLP address.
 *  - SLP enabled / disabled.
 *  - Manufacturer ( Optional ).
 *  - Remove SLP, when SIM change ( yes / no ).
 *  - Usage in home network ( yes / no ).
 *  - Timestamp ( when last used ).
 *  - Timestamp ( when tried last ).
 *  - Network & Operator code ( when tried last ).
 *  - Network & Operator code ( when last used successfully).
 *  - Editable ( yes / no).
 *	- Product Configuration - Indicates that server is configured by product team or configured by user. 
 *    
 *
 *  From the abover properties, only following fieds can be changed by SUPL settings client:
 *  - SLP address
 *  - IAP / destination.
 *  - SLP enabled / disabled.
 *  - Remove SLP, when SIM change ( yes / no ).
 *  - Usage in home network ( yes / no ).
 *  - Editable ( yes / no).
 *	- Product Configuration
 * 
 */
class CServerParams : public CBase 
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @since S60 5.1
        * @returns A new instance of this class.
        */
        IMPORT_C static CServerParams* NewL();

        /**
        * Two-phased constructor.
        *
        * @since S60 5.1 
        * @returns A new instance of this class.
        */
        IMPORT_C static CServerParams* NewLC();

        /**
        * Destructor.
        *
        * @since S60 5.1
        *
        */
        IMPORT_C ~CServerParams();

    private:

        /**
        * C++ default constructor.
        *
        * @since S60 5.1
        *
        */
	    CServerParams();

        /**
        * By default EPOC constructor is private.
        *
        * @since S60 5.1
        *
        */
        void ConstructL();
public:

    /**
     * Sets server parameters.  
     * 
     * @param [IN] aServerAddress Server address in string format 
     * @param [IN] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.
     * @param [IN] aServerEnabled Inidcates whether server enabled or not
     * @param [IN] aSimChangeRemove Inidcates whether this server entry needs to be deleted when SIM changes 
     * @param [IN] aUsageInHomeNw Inidcates whether server can be used in the home network or not 
     * @param [IN] aEditable Inidcates whether this server entry can be edited or not 
     * @param [IN] aSlpId,Id of server for which client has to modify properties of SLP.
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range or values specified are not reset value
     */
	IMPORT_C TInt Set(
                const TDesC& aServerAddress,
                const TDesC& aIapName,
                const TBool aServerEnabled = ETrue,
                const TBool aSimChangeRemove = EFalse,
                const TBool aUsageInHomeNw = ETrue,
                const TBool aEditable = ETrue,
                const TInt64 aSlpId = 0
                );


    /**
     * Returns server parameters. 
     * @param [IN] aSlpId,Id of server for which client has to retrive properties of SLP.
     * @param [OUT] aServerAddress Server address in string format 
     * @param [OUT] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.
     * @param [OUT] aServerEnabled Inidcates whether server enabled or not
     * @param [OUT] aSimChangeRemove Inidcates whether this server entry needs to be deleted when SIM changes 
     * @param [OUT] aUsageInHomeNw Inidcates whether server can be used in the home network or not 
     * @param [OUT] aEditable Inidcates whether this server entry can be edited or not 
     * 
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if all of the parameters has reset value 
     */
	IMPORT_C TInt Get(
                TInt64& aSlpId,                    
                TPtr aServerAddress,
                TPtr aIapName,
                TBool& aServerEnabled,
                TBool& aSimChangeRemove,
                TBool& aUsageInHomeNw,
                TBool& aEditable
                ) const;
                

    /**
     * Sets server parameters.  
     * @param [IN] aConfigurationType, Set ETrue to indicates that server is configured by product,
     * 																 		 EFalse to indicates that server is configured by user
     *
     * @return following error codes 
     *      - KErrNone if successful
     *			- Otherwise system wide error code
     */
	IMPORT_C void SetServerConfigurationType(TBool aProductConfiguration);


    /**
     * Gets server parameters.  
     * @param [OUT] aConfigurationType, ETrue  indicates that server is configured by product,
     * 																  EFalse indicates that server is configured by user
     *
     * @return following error codes 
     *      - KErrNone if successful
     *			- KErrNotFound if server configuration property is not set.
     */
	IMPORT_C TInt GetServerConfigurationType(TBool& aProductConfiguration) const;



private:
    TInt64 iSlpId;
    HBufC* iServerAddress;
    HBufC* iIap;
    TBool iServerEnabled;
    TBool iSimChangeRemove;
    TBool iUsageInHomeNw;
    TBool iEditable;
    TBool iProductConfigured;
    
    //To indicate if the values were set using the Set method 
    TBool iIsValueSet;
    
	};

/**
 * This class is used to retrieve the active trigger session information.  The required memory for
 * the parameters will be allocated during construction of this class and this class owns
 * the memory allocated.  And same will be released when this class is destroyed.  The 
 * trigger session information includes 
 *  - Notification Present (Yes/No)
 *  - TriggerNotificationStaus (On/Off)
 *  - TriggerType (Periodic/Area Event)
 *  - TriggerParams for Periodic triggering
 *  --- OutstandingTriggers (Interger)
 *  --- Interval (Integer)
 *
 *  From the abover properties, only following fieds can be changed by SUPL settings client:
 *  - TriggerNotificationStaus (On/Off)
 * 
 */
class CTriggerParams : public CBase 
	{
    public:

        /**
         * Enumeration to indicate trigger type.  
         *
         * @since S60 5.2
         */
        enum TTriggerType
            {
            /** Unknown  */
            EUnknown = -1,
            /** Periodic trigger */ 
            EPeriodic = 0, 
            /** Area Event trigger */ 
            EAreaEvent 
            };
        
        /**
         * Enumeration to indicate request type.  
         *
         * @since S60 5.2
         */
        enum TRequestType
            {
            /** Not specified  */
            EUnSpecified = -1,
            /** Network Initiated */ 
            ENetworkInitiated = 0, 
            /** Terminal Initiated */ 
            ETerminalInitiated 
            };
                    

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @since S60 5.2
        * @returns A newinstance of this class.
        */
        IMPORT_C static CTriggerParams* NewL();

        /**
        * Two-phased constructor.
        *
        * @since S60 5.2
        * @returns A new instance of this class.
        */
        IMPORT_C static CTriggerParams* NewLC();

        /**
        * Destructor.
        *
        * @since S60 5.2
        *
        */
        IMPORT_C ~CTriggerParams();

    private:

        /**
        * C++ default constructor.
        *
        * @since S60 5.2
        *
        */
	    CTriggerParams();

        /**
        * By default EPOC constructor is private.
        *
        * @since S60 5.2
        *
        */
        void ConstructL();
public:

    /**
     * Sets trigger parameters. 
     *
     * @since S60 5.2
     * @param [IN] aSessionId Session identifier to get/change paramater of particular session 
     * @param [IN] aSessionName Session name
     * @param [IN] aNotificationPresent Parameter to indicate whether notification is present or not
     * @param [IN] aTriggerNotificationStaus  Indicates whether notification needs to be shown or
     * not after every trigger fired
     * @param [IN] aTriggerType Indicates whether the trigger is periodic trigger or area event trigger 
     * @param [IN] aRequestType Indicates whether the request is terminal initiated or network initiated
     * @param [IN] aOutstandingTrigger Indicates number of outstanding triggers.  This is valid only 
     * if the trigger type is periodic triggers. 
     * @param [IN] aInterval Inidcates interval between the triggers.  This is valid only 
     * if the trigger type is periodic triggers.
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range or values specified are not reset value
     */
	IMPORT_C TInt Set( TInt64 aSessionId,
                       const TDesC& aSessionName,
                       TBool aNotificationPresent,
                       TBool aTriggerNotificationStatus,
                       TTriggerType aTriggerType,
                       TRequestType aRequestType,
                       TUint64  aOutstandingTrigger,
                       TUint64  aInterval
                      );                  
    
    
    /**
     * Returns trigger parameters. 
     *
     * @since S60 5.2
     * @param [OUT] aSessionId Session identifier to get/change paramater of particular session 
     * @param [OUT] aSessionName Session Name
     * @param [OUT] aNotificationPresent Parameter to indicate whether notification is present or not
     * @param [OUT] aTriggerNotificationStaus  Indicates whether notification needs to be shown or
     * not after every trigger fired
     * @param [OUT] aTriggerType Indicates whether the trigger is periodic trigger or area event trigger 
     * @param [OUT] aRequestType Indicates whether the request is terminal initiated or network initiated
     * @param [OUT] aOutstandingTrigger Indicates number of outstanding triggers.  This is valid only 
     * if the trigger type is periodic triggers. 
     * @param [OUT] aInterval Inidcates interval between the triggers.  This is valid only 
     * if the trigger type is periodic triggers.
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if all of the parameters has reset value
     *      - KErrOverflow if the input descriptor is too small to hold the value
     *      - KErrArgument if client has not allocated buffer for session name parameter i.e. if its NULL. 
     */
	
   IMPORT_C TInt Get( TInt64& aSessionId,
                      TPtr aSessionName,
                      TBool& aNotificationPresent,
                      TBool& aTriggerNotificationStatus,
                      TTriggerType& aTriggerType,
                      TRequestType& aRequestType,
                      TUint64&  aOutstandingTrigger,
                      TUint64&  aInterval
                    ) const;     
                    
/**
     * Sets trigger parameters. 
     *
     * @since S60 5.2
     * @param [IN]  aEndTime Inidcates end time for triggering session.  This is valid only 
     * 						 if the trigger type is periodic triggers.
     * @return following error codes
     *      - KErrNone if successful
     *		- KErrNotFound if Session id specified is not running as periodic triggering session.
     */
     
	IMPORT_C void SetTriggerEndTime(const TTime aEndTime);                  
	
	/**
     * Gets trigger parameters. 
     *
     * @since S60 5.2
     * @param [OUT] aEndTime Inidcates end time for triggering session.  This is valid only 
     * 						 if the trigger type is periodic triggers.
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrArgument if Session id is not in the specified range
     *			- KErrNotFound if Session id specified is not running as periodic triggering session.
     */
     
	IMPORT_C TInt GetTriggerEndTime(TTime& aEndTime)const ;                  

    

private:
    TInt64 iSessionId;
    HBufC* iSessionName;
    TBool iNotificationPresent;
    TBool iTriggerNotificationStatus;
    TTriggerType iTriggerType;
    TRequestType iRequestType;
    TUint64 iOutstandingTrigger;
    TUint64 iInterval;
    TTime   iEndTime;
    //To indicate if the values were set using the Set method 
    TBool iIsValueSet;
    };
#endif /* __EPOS_CSUPLSETTINGPARAMS_H__ */
