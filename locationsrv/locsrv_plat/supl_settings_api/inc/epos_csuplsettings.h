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


#ifndef __EPOS_CSUPLSETTINGS_H__
#define __EPOS_CSUPLSETTINGS_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSuplSettingsEngine;
class MSuplSettingsObserver;
class MSuplSessionObserver;
class CSuplSettingsNotifier;
class CSuplSessionNotifier;
class CServerParams;
class CTriggerParams;

// CONSTANTS

// CLASS DECLARATION

/**
*  This class is used for accessing SUPL settings. The SUPL settings 
*  contains information such about server properties which includes following:
*
*  Server address in string format.  This is an HSLP address used for SUPL connection.
*  Protocol version in mmnn format where mm indicates minor version nn is major version
*  The Internet Access Point Name refers to the access point which is used to access the HSLP over the internet.  
*  This can have maximum size of 100 characters.
*  Manufacturer name of server indicates the server manufacturer.
*  Time stamp when last used. 
*  Time stamp when last tried. 
*  Network info when the server was last used 
*  Network info when the server was last used sucessfully
*  Information on whether server supports emergency support or not
*  Information on whether server supports TLS or not
*  Information on whether server supports PSK-TLS or not
*  Information on whether server enabled or not
*  Information on whether this server entry needs to be deleted when SIM changes 
*  Information on whether server can be used in the home network or not 
*  Information on whether this server entry can be edited or not 
*  SUPL Usage indicates whether user should be notified before using SUPL.
*  Fallback indicates whether fallback is allowed or not if SUPL session fails with default server.
*
*  From the abover properties, only following fieds can be changed by SUPL settings client:
*  - SLP address
*  - IAP / destination.
*  - SLP enabled / disabled.
*  - Remove SLP, when SIM change ( yes / no ).
*  - Usage in home network ( yes / no ).
*  - Editable ( yes / no).
*
*  This is part of @lib epos_suplsettings.lib
*  @since S60 5.1 
*/

class CSuplSettings : public CBase
    {
    public:  
                  
        /**
         * Enumeration to indicate SUPL usage.  This is used as parameter for 
         * @ref SetSuplUsage() method. Client has to select one of the values from
	     * this enumeration when changing SUPL usage.  For example, if client is 
         * changing SUPL usage to "always ask", it should select ESuplUsageAlwaysAsk. 
         * If SUPL usage is set to ESuplUsageDisabled,
         * SUPL cannot be used. 
         *
         * Client can get SUPL usage using @ref GetSuplUsage() method.
         *
         * @since S60 3.2
         */
        enum TSuplSettingsUsage
            {
            /** Indicates that user should be asked always before SUPL usage */ 
            ESuplUsageAlwaysAsk = 0, 
            /** Indicates that SUPL can be used without asking user */ 
            ESuplUsageAutomatic, 
            /** Indicates that SUPL can be used without asking user in the home network and 
            * user should be notified in the visiting network */ 
            ESuplUsageHomeAutomatic, 
            /** Indicates that SUPL usage is disabled */ 
            ESuplUsageDisabled 
            };                 

		
        /**
         * Enumeration to indicate status of SUPL triggered service.  This is used as parameter for 
         * @ref ChangeSuplTriggerServiceStatus() and @ref GetSuplTriggerServiceStatus() method. Client has to select one of the values from
	     * this enumeration when changing SUPL triggered service status.  
	     * For example, if client is changing SUPL triggered service status to ON,it should select ESuplTriggerON. 
         * If SUPL triggered service status is set to ESuplTriggerON,then any client requesting triggered service will be served by SUPL Framework.
         * If SUPL triggered service status is set to ESuplTriggerOFF,then client's request for triggered service will fail.
         * If SUPL triggers are in progress and client sets off triggered service,then all started triggering sessions will get completed with error.
         *
         * @since S60 5.2
         */
			enum TSuplTriggerStatus
            {
            /** Indicates that SUPL triggered service status is ON */ 
            ESuplTriggerOn = 0, 
            /** Indicates that SUPL triggered service status is OFF */ 
            ESuplTriggerOff
            };      
                      
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @since S60 3.1
        * @returns A new instance of this class.
        */
        IMPORT_C static CSuplSettings* NewL();

       /**
        * Two-phased constructor.
        *
        * @since S60 5.1 
        * @returns A new instance of this class.
        */
        IMPORT_C static CSuplSettings* NewLC();

       /**
        * Destructor.
        *
        * @since S60 3.1
        *
        */
        IMPORT_C ~CSuplSettings();

    protected:

       /**
        * C++ default constructor.
        *
        * @since S60 3.1
        *
        */
        CSuplSettings();

       /**
        * By default EPOC constructor is private.
        *
        * @since S60 3.1
        *
        */
        void ConstructL();

    public: // New functions

       /**
        * This method is used to start listening for SUPL setting changes. To
        * listen for changes, the client must implement the
        * @ref MSuplSettingsObserver interface.  Client must allocated memory 
        * for the observer.
        *
        * Listening can be stopped by invoking @ref RemoveObserver().
        *
        * @since S60 3.1
        *
        *
        * @param [IN] aObserver The observing object.
	    * @return one of the following values
	    * 	- KErrNone if setting observer is successful
	    * 	- KErrAlreadyExists if observer is already set 
	    *  
        */
        IMPORT_C TInt SetObserverL(
               MSuplSettingsObserver& aObserver
        );

        /**
         * This method is used to stop listening for SUPL setting changes.  After removing observer,
         * client has to free the memory allocated for the observer.  If this method is invoked without
         * setting observer using @ref SetObserverL() method, the request is silently ignored.
         *
         * @since S60 3.1
         *
         */
        IMPORT_C void RemoveObserver();

       /**
        * This method is used to start listening for SUPL session changes. To
        * listen for changes, the client must implement the
        * @ref MSuplSessionObserver interface.  Client must allocated memory 
        * for the observer.
        *
        * Listening can be stopped by invoking @ref RemoveSessionObserver().
        *
        * @since 
        *
        *
        * @param [IN] aObserver The observing object.
	    * @return one of the following values
	    * 	- KErrNone if setting observer is successful
	    * 	- KErrAlreadyExists if observer is already set 
	    *  
        */
        IMPORT_C TInt SetSessionObserverL(
               MSuplSessionObserver& aObserver
        );

        /**
         * This method is used to stop listening for SUPL session changes.  After removing observer,
         * client has to free the memory allocated for the observer.  If this method is invoked without
         * setting observer using @ref SetSessionObserverL() method, the request is silently ignored.
         *
         * @since 
         *
         */
        IMPORT_C void RemoveSessionObserver(); 
        
        
        /**
         * This method is invoked to initialize SUPL settings object.  This is 
         * an asynchronous call and client can cancel this request by invoking 
         * @ref CancelInitialize().  
         * During initialization, current IMSI value is obtained from
	     * the system. Client should invoke this method to re-read the IMSI if the client detects 
         * an IMSI change.
         *
         * Note that if @ref Initialize() method is invoked after started listening to 
         * SUPL settings changes by invoking @ref SetObserverL(), listening is not stopped.
         *
         * @since S60 3.1
         *
	     * @param [OUT] aStatus To return result code.  It will hold one of the following values
         * after completion:
	     * 	- KErrNone if intialization is successful
         * 	- KErrGeneral if initialization has failed (initialization can fail if SIM card is not present)
         * 	- KErrCancel if initialization is cancelled
         *
         */ 

        IMPORT_C void Initialize(TRequestStatus& aStatus);
    
        /**
         * This method is invoked to cancel @ref CSuplSettings::Initialize().
         *
         * @since S60 3.1
         *
         */
        
        IMPORT_C void CancelInitialize();

        

	/**
	 * This method can be used to generate HSLP address from the IMSI
     * obtained during initialization.  
     * HSLP address in the FQDN format can have maximum size of
     * 256 bytes.  Client must allocate enough memory to hold the HSLP address.
     * HSLP address can be generated from IMSI, for example,
     * if HSLP address read from SUPL settings is empty or not valid.  
     * The generated HSLP is in the FQDN format and looks like 
     * "h-slp.mncXXX.mccYYY.3gppnetwork.org" 
     * where XXX and YYY values for MNC and MCC extracted from the IMSI.
     *
     * This method should be invoked only after 
     * @ref Initialize() is completed with KErrNone.  
	 *
     * @since S60 3.1
     *
	 * @param [OUT] aHslpAddress will hold, on successful completion, HSLP address generated 
     * from IMSI obtained during initialization.  HSLP address can be of size 256 bytes.
	 * @return one of the following error codes
	 *	- KErrNone if method completed successfully
     *	- KErrOverflow if the descriptor is too small to hold the HSLP address 
	 *	- KErrNotReady if this method is invoked before @ref Initialize or @ref Initialize has not completed with @p KErrNone
     *	- KErrArgument if argument passed is of length zero
	 */

	IMPORT_C TInt GenerateHslpAddressFromImsi(TDes& aHslpAddress);
    
    /**
     * This method returns IMSI obtained during initialization of SUPL settings object.  
     * This method should be invoked after @ref Initialize() is completed with KErrNone.  
     *
     * @since S60 3.1
     *
     * @param [OUT] aImsi will hold, on successful completion, IMSI obtained during initialization.  
     * IMSI can have maximum size of 15 characters.
     * If SUPL Settings library is unable to retrieve the IMSI value during initialization, 
     * aImsi will be a descriptor of length zero.
     * @return one of the following error codes returned
     *  - KErrNone if method completed successfully
     *  - KErrNotReady if this method is invoked before @ref Initialize or @ref Initialize has not 
     *  completed with @p KErrNone
     *  - KErrArgument if argument passed is of length zero 
     *  - KErrOverflow if the descriptor is too small to hold the IMSI 
     *
     */

    IMPORT_C TInt GetImsi(TDes& aImsi);

    /**
     * This method compares IMSI value currently stored in SUPL settings storage with
     * IMSI value obtained during initialization. If IMSI value obtained during 
     * initialization and IMSI value stored in SUPL settings storage are same, this method returns false 
     * otherwise true is returned.   
     * This method should be invoked after @ref Initialize() is completed with KErrNone.  
     * 
     * @since S60 3.1
     *
     * @param [OUT] aChanged will hold, on successful completion, one of the following value
     *   - ETrue if IMSI obtained during initialization and IMSI value stored in SUPL settings
     *   are different.
     *   - EFalse if IMSI obtained during initialization and IMSI value stored in SUPL settings
     *   are same.
     * @return one of the following
     *  - KErrNone if method completed successfully
     *  - KErrNotReady if this method is invoked before @ref Initialize or @ref Initialize has not completed with @p KErrNone 
     */
    
    IMPORT_C TInt IsImsiChanged(TBool& aChanged);
    
    /**
     * This method is deprecated.  
     *
     * @since S60 10.1
     *
     * @return one of the following error codes: 
     *         - KErrNotSupported  
     */

    IMPORT_C TInt GetSuplUsage(TSuplSettingsUsage& aUsage) const;

     /**
     * This method is deprecated.  
     *
     * @since S60 10.1
     *
     * @return one of the following error codes: 
     *         - KErrNotSupported  
     */
    IMPORT_C TInt SetSuplUsage(TSuplSettingsUsage aUsage);
    /**
     * This method is used to add new server into settings.  It is client's responsibility
     * to set all the parameters in aParamValues except SLP identification.  After adding
     * server entry, the server identity generated will be retuned with aSlpId.  The returned
     * server identity can be used with other methods change any of server parameters. If server 
     * entry with the same server address already presnt, then this method returns error.
     *
     * @since S60 5.1 
     *
     * @param [IN] aParamValues Server parameter values which which the server entry needs to be added 
     * @param [OUT] aSlpId SLP identification for the new server added 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrAlreadyExists if the server address mentioned in aParamValues already present in settings 
     *         - KErrGeneral if changing SUPL settings has failed 
     */ 
    IMPORT_C TInt AddNewServer( 
            const CServerParams *aParamValues, 
            TInt64& aSlpId 
    );


    /**
     * This method is used to change server address parameter in server settings 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aServerAddres SUPL Server address 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if address passed is empty or if the SLP ID passed is not valid 
     *         - KErrGeneral if changing SUPL settings has failed 
     */

    IMPORT_C TInt SetServerAddress( 
            const TInt64 aSlpId, 
            const TDesC& aServerAddress
    );

    /**
     * This method is used to retrieve server address parameter from server settings.  Client
     * has to allocate memory for server address parameter.
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID of server for which the server address needs to be retrieved 
     * @param [OUT] aServerAddres SUPL Server address 
     * @return one of the following error codes: 
     *         - KErrNone if server address is retrieved successfully 
     *         - KErrArgument if the SLP ID passed is not valid 
     */

    IMPORT_C TInt GetServerAddress( 
            TInt64 aSlpId, 
            TDes& aServerAddress
    ) const;

    /**
     * This method is used to change IAP Name in server parameters. IAP name refers to 
     * the access point which is used to access the HSLP over the internet. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters. 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if IAP Name is empty or if the SLP ID passed is not valid 
     *         - KErrGeneral if changing SUPL settings has failed 
     */
    IMPORT_C TInt SetIapName( 
            const TInt64 aSlpId, 
            const TDesC& aIapName
    );

    /**
     * This method is used to retrieve IAP Name in server parameters. IAP name refers to 
     * the access point which is used to access the HSLP over the internet. It is client's
     * responsibility to allocate memory for IAP name before invoking this method.
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters. 
     * @return one of the following error codes: 
     *         - KErrNone if IAP name is retrieved successfully 
     *	      - KErrOverflow if the descriptor is too small to hold the IAP name 
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    IMPORT_C TInt GetIapName( 
            const TInt64 aSlpId, 
            TDes& aIapName
    ) const;


    /**
     * This method is used to change parameter which indicates whether server
     * can be used for SUPL session or not. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aEnable Flag to indicate whether server can be enabled or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     *         - KErrGeneral if changing SUPL settings has failed 
     */
    IMPORT_C TInt SetServerEnabledFlag(
            const TInt64 aSlpId, 
            const TBool aEnable 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * can be used for SUPL session or not. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aEnable Flag to indicate whether server can be enabled or not 
     * @return one of the following error codes: 
     *         - KErrNone if server available flag is retrieved successfully 
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    IMPORT_C TInt GetServerEnabledFlag(
            const TInt64 aSlpId, 
            TBool& aEnable 
    ) const;

    /**
     * This method is used to change parameter which indicates whether server
     * details can be removed if SIM is changed. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aSimChangeFlag Flag to indicate whether server can be removed or not if there is a SIM change 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     *         - KErrGeneral if changing SUPL settings has failed 
     */
    IMPORT_C TInt SetSimChangeRemoveFlag(
            const TInt64 aSlpId, 
            const TBool aSimChangeFlag 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * details can be removed if SIM is changed. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aSimChangeFlag Flag to indicate whether server can be removed or not if there is a SIM change 
     * @return one of the following error codes: 
     *         - KErrNone if SIM change removal flag is retrieved successfully 
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    IMPORT_C TInt GetSimChangeRemoveFlag(
            const TInt64 aSlpId, 
            TBool& aSimChangeFlag 
    ) const;

    /**
     * This method is used to change parameter which indicates whether server
     * can be used out side home network or not. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aHomeNwFlag Flag to indicate whether server can be used outside home network or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     *         - KErrGeneral if changing SUPL settings has failed 
     */
    IMPORT_C TInt SetUsageInHomwNwFlag(
            const TInt64 aSlpId, 
            const TBool aHomeNwFlag 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * can be used out side home network or not. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aHomeNwFlag Flag to indicate whether server can be used outside home network or not 
     * @return one of the following error codes: 
     *         - KErrNone if home network usage flag is retrieved successfully 
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    IMPORT_C TInt GetUsageInHomwNwFlag(
            const TInt64 aSlpId, 
            TBool& aHomeNwFlag 
    ) const;


    /**
     * This method is used to change parameter which indicates whether server
     * details are editable or not. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [IN] aEditFlag Flag to indicate whether server details are editable or not 
     * @return one of the following error codes: 
     *         - KErrNone if editable flag is retrieved successfully 
     *         - KErrArgument if the SLP ID passed is not valid 
     *         - KErrGeneral if SUPL settings cannot be changed
     */ 
    IMPORT_C TInt SetEditableFlag( 
            const TInt64 aSlpId, 
            const TBool aEditFlag 
    ) const;

    /**
     * This method is used to retrieve parameter which indicates whether server
     * details are editable or not. 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aEditFlag Flag to indicate whether server details are editable or not 
     * @return one of the following error codes: 
     *         - KErrNone if editable flag is retrieved successfully 
     *         - KErrArgument if the SLP ID passed is not valid 
     */ 
    IMPORT_C TInt GetEditableFlag( 
            const TInt64 aSlpId, 
            TBool& aEditFlag 
    ) const;

    /**
     * This method is used to change all parameters of server in SUPL settings.  The server identity
     * in @ref CServerParams which is passed as parameter gives information on for which server
     * entry the values needs to be changed.  If the passed server identity does not exists in
     * settings, error is returned.  
     *
     * @since S60 5.1 
     *
     * @param [IN] aParamValues New value for server parameters. 
     * @ref CServerParams class
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the value of any of the pamater is not in range
     *         - KErrNotFound if the SLP Identity mentioned in aParamValues does not exist in settings 
     *         - KErrGeneral if changing SUPL settings has failed 
     */

    IMPORT_C TInt SetAllParameter( 
            const CServerParams *aParamValues
    );

    /**
     * This method is used to change all parameters of default server in SUPL settings.  In this
     * method server identity of @ref CServerParams passed is not considered and values
     * of default server is updated except server identity.  If none of the server is set
     * as default, the new server entry will be added and will be marked as default.  Client can
     * use @ref GetDefaultSlpId() method to get SLP identity of default server entry.
     *
     * @since S60 5.1 
     *
     * @param [IN] aParamValues Parameter for which the value needs to be changed  
     * @ref CServerParams class
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the value of any of the pamater is not in range
     *         - KErrGeneral if changing SUPL settings has failed 
     */

    IMPORT_C TInt SetDefaultServer( 
            const CServerParams *aParamValues
    );


    /**
     * This method is deletes server entry from SUPL settings storage.  
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId Server identity for which the entery needs to be deleted from the SUPL settings storage 
     *
     * @return one of the following error codes: 
     *         - KErrNone if entry is deleted sucessfully 
     *         - KErrNotFound if the specified server identity does not exists in settings storage 
     *         - KErrGeneral if changing SUPL settings has failed 
     *
     */
    IMPORT_C TInt RemoveServer(const TInt64 aSlpId);

       /**
        * This method changes the priority of the server to the specified priority.  Here the priority 
        * refers to the order of server entries in the SUPL settings storage.  If the priority of server
        * is changed from position N to M and N > M, then the priority of all the server entries from Mth
        * position will get. If the priority of server is changed from position N to M and
        * N < M, then the priority of all servers from N+1 whill get changed. Priority is not valid if the
        * mentioned priority specified beyond the number of number of servers already present in the settings.
        * For example, if the specified priority is N and number of server entries in the settings in M,
        * then N is invalid if N > M.
        *
        * The parameter aDirection specifies whether the server to be moved up or down.  If this has value
        * ETrue server will be moved up.  Else server will be moved down.
        *
        * @since S60 5.1 
        *
        * @param [IN] aSlpId Server identity for which the priority needs to be changed 
        * @param [IN] aPriority Number of positions to move 
        * @param [IN] aDirection Whether to increase or decrease priority. 
        *
        * @return one of the following error codes: 
        *         - KErrNone if the priority is changed sucessfully 
        *         - KErrNotFound if the specified server identity does not exists in settings storage 
        *         - KErrArgument if the specified priority is not valid (eg. negative)
        *
        */
        IMPORT_C TInt ChangePriority(
                        TInt64 aSlpId, 
                        TInt aPriority,
                        TBool aDirection
                        );

    /**
     * This method is used to retrieve all properties of server from SUPL settings storage
     * based on SLP identification.  
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP ID for which the parameters needs to be changed 
     * @param [OUT] aParamValues Parameter for which the value needs to be changed  
     * @ref CServerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrNotFound if the specified server does not exists in the settings
     *
     */
    IMPORT_C TInt GetSlpInfoFromId(
            const TInt64 aSlpId, 
            CServerParams *aParamValues
    ) const;


    /**
     * This method is used to retrieve all properties of server from SUPL settings storage
     * based on SLP address.  
     *
     * @since S60 5.1 
     *
     * @param [IN] aServerAddress SLP address for which the parameters needs to be changed 
     * @param [OUT] aParamValues Parameter for which the value needs to be changed  
     * @ref CServerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrNotFound if the specified server does not exists in the settings
     *
     */
    IMPORT_C TInt GetSlpInfoAddress(
            const TDesC& aServerAddress, 
            CServerParams *aParamValues
    ) const;

    /**
     * This method is used to retrieve all properties of default server from SUPL settings storage.  
     *
     * @since S60 5.1 
     *
     * @param [OUT] aParamValues Retrieves values from SUPL Settings storage 
     * @ref CServerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrNotFound if the settings does not have any server entry 
     *
     */
    IMPORT_C TInt GetDefaultServer(
            CServerParams *aParamValues
    ) const;

    /**
     * This method is used to retrieve number of server entries present in SUPL settings. 
     *
     * @since S60 5.1 
     *
     * @param [OUT] aCount Number of SLP entries 
     *
     * @return one of the following error codes: 
     *         - KErrNone if number of SLP entries are calculated 
     *         - KErrNotFound if the settings does not have any server entry 
     *
     */
    IMPORT_C TInt SlpCount(
            TInt& aCount 
    );

    /**
     * This method is used check whether server entry exists or not based on SLP
     * identification 
     *
     * @since S60 5.1 
     *
     * @param [IN] aSlpId SLP identification 
     *
     * @return one of the following codes: 
     *         - ETrue if server entry exists in settings 
     *         - EFalse if server entry does not exists in settings 
     *
     */
    IMPORT_C TBool IsSlpExists(
            const TInt64 aSlpId 
    );

    /**
     * This method is used check whether server entry exists or not based on SLP
     * address 
     *
     * @since S60 5.1 
     *
     * @param [IN] aServerAddress Server address 
     *
     * @return one of the following codes: 
     *         - ETrue if server entry exists in settings 
     *         - EFalse if server entry does not exists in settings 
     *
     */
    IMPORT_C TBool IsSlpExists(
            const TDesC& aServerAddress 
    );

    /**
     * This synchronous method is used to retrieve properties of all servers from SUPL settings storage.  
     * This method retrieves all properties for each server.
     *
     * @since S60 5.1 
     *
     * @param [OUT] aParamValues Array of retrieved server entries 
     * @ref CServerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrNotFound if the settings does not have any server entry 
     *
     */
    IMPORT_C TInt GetAllSlp(
            RPointerArray<CServerParams>& aParamValues
    ) const;

    /**
     * This method is used to retrieve properties of all servers from SUPL settings storage.  
     * This method retrieves all properties for each server.
     *
     * @since S60 5.1 
     *
     * @param [OUT] aParamValues Array of retrieved server entries 
     * @ref CServerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrNotFound if the settings does not have any server entry 
     *
     */
    IMPORT_C TInt GetAllSlp(
            RPointerArray<CServerParams>& aParamValues,
            TRequestStatus& aStatus
    ) const;


    /**
     * This method enables or disables the fall back in SUPL settings.  If the fallback enabled,
     * if the SUPL session fails with default server or with the sever mentioned then fallback will
     * happen to next server in the list or to the server address generated from IMSI depending
     * on the failure type.  If the SUPL session has failed due to connection problem with server,
     * then fallback will happen to the address generated from IMSI.  In all other failures,
     * fallback will happen to the next server in the list.
     *
     * @since S60 5.1 
     *
     * @param [IN] aFallBack Indicates whether the fallback needs to be enabled or disabled 
     *
     * @return one of the following error codes: 
     *         - KErrNone if the fallback value changed sucessfully 
     *         - KErrGeneral if the fallback is enabled when only one server entry is present
     *         - KErrGeneral if changing SUPL settings has failed 
     */
    IMPORT_C TInt SetFallBack(TBool aFallBack);

    /**
     * This method retrieves the fallback value from SUPL settings storage.
     *
     * @since S60 5.1 
     *
     * @param [OUT] aFallBack Retrived fallback value 
     *
     * @return one of the following error codes: 
     *         - KErrNone if the fallback value retrieved sucessfully 
     */
    IMPORT_C TInt GetFallBack(TBool& aFallBack) const;

    /**
     * This method is used to retrieve properties of all active triggers. 
     *
     * @since S60 5.2 
     *
     * @param [OUT] aParamValues Array of retrieved trigger parameters.Ownership remains with client. 
     * @ref CTriggerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if active trigger parameters are retrieved successfully.  
     *         - KErrNotFound if there are no active triggers 
     *           otherwise system wide error codes. 
     *
     */
    IMPORT_C TInt GetTriggerParams(
            RPointerArray<CTriggerParams>& aParamValues
    ) const;
    /**
     * This method is used to retrieve properties of a given session trigger. 
     *
     * @since S60 5.2 
     *
     * @param [IN] aSessionId Session identifier to get paramater of particular session.
     * @param [OUT] aParamValues Trigger Parameter.Ownership remains with client. 
     * @ref CTriggerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if active trigger parameters are retrieved successfully.  
     *         - KErrNotFound if there is no active session with given id 
     *         - KErrArgument if aParamValues is NULL.
     *           otherwise system wide error codes. 
     */
    
    IMPORT_C TInt GetTriggerParams( TInt64 aSessionId,
                                     CTriggerParams*& aParamValues ) const;
          

    /**
     * Sets Notification status of particular session
     *
     * @since S60 5.2 
     * @param [IN] aSessionId Session identifier to get/change paramater of particular session 
     * @param [IN] aTriggerNotificationStaus  Indicates whether notification needs to be shown or
     * not after every trigger fired
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if session is not found
     *           otherwise system wide error codes. 
     */
	IMPORT_C TInt SetNotificationStatus(
	            TInt64 aSessionId,
                TBool aTriggerNotificationStatus
                );
	/**
	 * Cancels given ongoing triggering session
	 *
	 * @since S60 5.2 
	 * @param [IN] aSessionId Session identifier 
	 *
	 * @return following error codes
	 *      - KErrNone if successful
	 *      - KErrNotFound if session is not found
	 *           otherwise system wide error codes. 
	 */
	IMPORT_C TInt CancelTriggerSession( TInt64 aSessionId );
	
	/**
	 * This method can be used to obtain the default IAP configured.
	 *
	 * @since S60 5.2
	 *
	 * @param [OUT] aIapName a will hold, on successful completion, the default IAP name that was configured 
	 * This can have maximum size of 100 characters.
	 * @return one of the following error codes
	 *  - KErrNone if method completed successfully
	 *  - KErrOverflow if the descriptor is too small to hold the HSLP address 
	 *  - KErrArgument if argument passed is of length zero
	 */

	IMPORT_C TInt GetDefaultIAPName(TDes& aIapName);
    
	/**
	 * Changes status of SUPL triggered service. 
	 *
	 * @since S60 10.1 
	 * @param [IN] aSuplTriggerStatus, Indicates the new SUPL triggered service status value
	 * @capability WriteDeviceData Its required for cenrep write operation.
	 * @return following error codes
	 *      - KErrNone if successful
	 *      otherwise system wide error codes. 
	 */
	IMPORT_C TInt SetSuplTriggeredServiceStatus( const TSuplTriggerStatus& aSuplTriggerStatus );
        
	/**
	 * Returns the status of SUPL triggered service.
	 *
	 * @since S60 10.1 
	 * @param [OUT] aSuplTriggerStatus, Gets status of SUPL triggering session either ON or OFF
	 * @capability ReadDeviceData Its required for cenrep read operation.
	 * @return following error codes
	 *      - KErrNone if successful
	 *      otherwise system wide error codes. 
	 */
	IMPORT_C TInt GetSuplTriggeredServiceStatus( TSuplTriggerStatus& aSuplTriggerStatus );
    
    private: // New functions
        
    private:
        // By default, prohibit copy constructor
        CSuplSettings( const CSuplSettings& );
        // Prohibit assigment operator
        CSuplSettings& operator= ( const CSuplSettings& );

    protected:    // Data
//    private:    // Data
        /** 
         * Pointer to notifiers, needed to listen to SUPL settings change
         * events
         */
        CSuplSettingsNotifier*        iSettingsNotifier;
        /*
         * Pointer to notifiers, needed to listen to SUPL Settings Db Change
         */
        CSuplSettingsNotifier*        iSettingsDBChangeNotifier;
        /**  Pointer to CSuplSettingsEngine to all settings functions */
        CSuplSettingsEngine*         iSettingsEngine;
        
        CSuplSessionNotifier*        iSessionNotifier;
        /** To store and retun status for Initialize method */
        TRequestStatus  iStatus;
        /** To store whether settings are locked or not */
        TBool iLocked;
    };

#endif      // __EPOS_CSUPLSETTINGS_H__

// End of File
