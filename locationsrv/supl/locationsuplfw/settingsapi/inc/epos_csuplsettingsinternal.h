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
* Description:   SUPL Settings class
*
*/



#ifndef __EPOS_CSUPLSETTINGSINTERNAL_H__
#define __EPOS_CSUPLSETTINGSINTERNAL_H__

//  INCLUDES
#include <e32base.h>
#include <epos_csuplsettings.h>
// FORWARD DECLARATIONS
class CTriggerParams;
class CServerExtensionParams;

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
*  When changing SUPL settings, client has to explicitly lock the SUPL settings and 
*  unlock the SUPL settings immediately after the changes are made.  Locking is required to
*  avoid multiple clients changing SUPL settings at the same time.  Only after 
*  unlock of SUPL settings, changes are committed to SUPL settings storage.
*
*  This is part of @lib epos_suplsettings.lib
*  @since S60 9.1TB 
*/

class CSuplSettingsInternal : public CSuplSettings
    {
    public:  
        /**
         * Enumeration to indicate which type of network used. 
	     * This is one of the parameters for @ref SetNetInfoLastUse() and 
         * @ref SetNetInfoLastSucess() methods. 
         * Client has to select one of the values from this enumeration when changing 
         * network info in server settings.  
         *
         * @since S60 9.1TB 
         */
        enum TSuplSettingsNetworkType
            {
            /** Indicates that network is GSM network */ 
            EGsmNetwork = 0,
            /** Indicates that network is WCDMA network */
            EWcdmaNetwork
        };
        /**
         * Enumeration to indicate which parameter of server needs to be changed. 
         * This is one of the parameters for @ref Set() method. Client has to select one of the values from
         * this enumeration when changing SUPL settings.  For example, if UI application is changing
         * SUPL settings, it should select EUserInterface. Client can get information about
         * particular parameter from SUPL settings using @ref Get() method with this enumeration.
         */
        enum TServerParam
            {
            /** Indicates SUPL server address parameter in server parameters */ 
            EServerAddress = 0,
            /** Indicates protocol version parameter in server parameters */
            EProtocolVersion,           
            /** Indicates emergency support parameter in server parameters */
            EEmergencySupport, 
            /** Indicates TLS parameter in server parameters */
            ETlsEnabled,
            /** Indicates PSK-TLS parameter in server parameters */
            EPskTlsUsed,
            /** Indicates IAP parameter in server parameters */
            EIap,
            /** Indicates server enabled parameter in server parameters */
            EServerEnabled,
            /** Indicates manufacturer parameter in server parameters */
            EManufacturer,
            /** Indicates SIM change remove parameter in server parameters */
            ESimChangeRemove,
            /** Indicates usage in home network parameter in server parameters */
            EUsageInHomeNw,
            /** Indicates time stamp when used last time parameter in server parameters */
            ETimeLastUse,
            /** Indicates time stamp when used tried parameter in server parameters */
            ETimeLastTried,
            /** Indicates network information when used last time parameter in server parameters */
            ENetInfoLastUse,
            /** Indicates network information when last succeeded parameter in server parameters */
            ENetInfoLastSucess,
            /** Indicates editable parameter in server parameters */
            EEditable
            };
            
        /**
         * Enumeration to indicate which parameter of active session needs to be changed. 
         */
        enum TSessionParam
            {
            /** Indicates Trigger notification status parameter */ 
            ETriggerNotificationStatus = 0,
            /** Indicates number of outstanding triggers parameter */ 
            EOutstandingTriggers = 1
            };     
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @since S60 3.1
        * @returns A new instance of this class.
        */
        IMPORT_C static CSuplSettingsInternal* NewL();

        /**
        * Two-phased constructor.
        *
        * @since S60 9.1TB 
        * @returns A new instance of this class.
        */
        IMPORT_C static CSuplSettingsInternal* NewLC();

        /**
        * Destructor.
        *
        * @since S60 3.1
        *
        */
        IMPORT_C ~CSuplSettingsInternal();

    private:

        /**
        * C++ default constructor.
        *
        * @since S60 3.1
        *
        */
        CSuplSettingsInternal();

        /**
        * By default EPOC constructor is private.
        *
        * @since S60 3.1
        *
        */
        void ConstructL();

    public: // New functions

       /**
        * This method is used to change protocol version supported by SUPL server.  The protocol
        * version consists of two parts : Major version and Minor version.  
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aMajor Major version number.  It can takes values from 0 to 255. 
        * @param [IN] aMinor Minor version number.  It can takes values from 0 to 255. 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if version parameters are not in range or if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetVersion( 
               const TInt64 aSlpId, 
               const TInt aMajor,
               const TInt aMinor 
        );

       /**
        * This method is used to retrieve protocol version supported by SUPL server.  The protocol
        * version consists of two parts : Major version and Minor version.  
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aMajor Major version number.  It can takes values from 0 to 255. 
        * @param [OUT] aMinor Minor version number.  It can takes values from 0 to 255. 
        * @return one of the following error codes: 
        *         - KErrNone if version parameters are retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetVersion( 
               const TInt64 aSlpId, 
               TInt& aMajor,
               TInt& aMinor 
        ) const;


       /**
        * This method is used to change parameter which indicates whether server
        * supports emergency service or not. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aEmergencySupportFlag Flag to indicate whether emergency service is supported or not 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetEmergencySupportFlag( 
               const TInt64 aSlpId, 
               const TBool aEmergencySupportFlag 
        );

       /**
        * This method is used to retrieve parameter which indicates whether server
        * supports emergency service or not. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aEmergencySupportFlag Flag to indicate whether emergency service is supported or not 
        * @return one of the following error codes: 
        *         - KErrNone if emergency support flag is retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetEmergencySupportFlag( 
               const TInt64 aSlpId, 
               TBool& aEmergencySupportFlag 
        ) const;
        

       /**
        * This method is used to change manufacturer in server parameters. Manufacturer name 
        * refers to the service provider who is providing SUPL service. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aManufacturer Manufacture name.  This can have maximum size of 100 characters. 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if manufacturer name is empty or if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetManufacturerName( 
               const TInt64 aSlpId, 
               const TDesC& aManufacturer
        );

       /**
        * This method is used to retrieve manufacturer name in server parameters. Manufacturer name 
        * refers to the service provider who is providing SUPL service. It is client's responsibility
        * to allocate memory to aManufacturer parameter before invoking this method.
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aManufacturer Manufacture name.  This can have maximum size of 100 characters. 
        * @return one of the following error codes: 
        *         - KErrNone if manugacturer name retrieved successfully 
        *	      - KErrOverflow if the descriptor is too small to hold the manufacturer name 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetManufacturerName( 
               const TInt64 aSlpId, 
               TDes& aManufacturerName
        ) const;


       /**
        * This method is used to change timestamp when the HSLP was used last time. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aTime Time stamp 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetLastUseTime( 
               const TInt64 aSlpId, 
               const TTime aTime 
        );

       /**
        * This method is used to retrieve timestamp when the SLP was used last time. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aTime Time stamp 
        * @return one of the following error codes: 
        *         - KErrNone if timestamp retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetLastUseTime( 
               const TInt64 aSlpId, 
               TTime& aTime 
        ) const;

        
       /**
        * This method is used to change timestamp when the HSLP was used tried last time. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aTime Time stamp 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetLastTriedTime( 
               const TInt64 aSlpId, 
               const TTime aTime 
        );

       /**
        * This method is used to retrieve timestamp when the HSLP was used tried last time. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aTime Time stamp 
        * @return one of the following error codes: 
        *         - KErrNone if timestamp retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetLastTriedTime( 
               const TInt64 aSlpId, 
               TTime& aTime 
        ) const;

       /**
        * This method is used to change network information when the SUPL session was last done.
        * The SUPL session could be sucessful one or failed one. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aNetType The network type 
        * @param [IN] aMcc Mobile country code.  It can have value from 0 to 999 
        * @param [IN] aMnc Mobile network code.  It can have value from 0 to 999 
        * @param [IN] aCid Cell Identity.  It can have value from 0 to 65535 if aNetType is GSM or
        * it can have value from 0 to 268435455 if aNetType is WCDMA 
        * @param [IN] aLac Location Aread Code. This parameter is written to settings only if aNetType is GSM.
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid or any of the parameters passed is not in range 
        */
        IMPORT_C TInt SetNetInfoLastUse( 
               const TInt64 aSlpId, 
               const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType,
               const TInt aMcc,
               const TInt aMnc,
               const TInt aCid, 
               const TInt aLac=0
        );

       /**
        * This method is used to retrieve network information when the SUPL session was last done.
        * The SUPL session could be sucessful one or failed one. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aNetType The network type 
        * @param [OUT] aMcc Mobile country code.  It can have value from 0 to 999 
        * @param [OUT] aMnc Mobile network code.  It can have value from 0 to 999 
        * @param [OUT] aCid Cell Identity.  It can have value from 0 to 65535 if aNetType is GSM or
        * it can have value from 0 to 268435455 if aNetType is WCDMA 
        * @param [OUT] aLac Location Aread Code. This parameter is written to settings only if aNetType is GSM.
        * @return one of the following error codes: 
        *         - KErrNone if network information retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetNetInfoLastUse( 
               const TInt64 aSlpId, 
               CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,
               TInt& aMcc,
               TInt& aMnc,
               TInt& aCid, 
               TInt& aLac
        ) const;

       /**
        * This method is used to change network information when the SUPL session was last done sucessfully.
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aNetType The network type 
        * @param [IN] aMcc Mobile country code.  It can have value from 0 to 999 
        * @param [IN] aMnc Mobile network code.  It can have value from 0 to 999 
        * @param [IN] aCid Cell Identity.  It can have value from 0 to 65535 if aNetType is GSM or
        * it can have value from 0 to 268435455 if aNetType is WCDMA 
        * @param [IN] aLac Location Aread Code. This parameter is written to settings only if aNetType is GSM.
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid or any of the parameters passed is not in range 
        */
        IMPORT_C TInt SetNetInfoLastSucess( 
               const TInt64 aSlpId, 
               const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType,
               const TInt aMcc,
               const TInt aMnc,
               const TInt aCid, 
               const TInt aLac=0
        );

       /**
        * This method is used to retrieve network information when the SUPL session was last done sucessfully.
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aNetType The network type 
        * @param [OUT] aMcc Mobile country code.  It can have value from 0 to 999 
        * @param [OUT] aMnc Mobile network code.  It can have value from 0 to 999 
        * @param [OUT] aCid Cell Identity.  It can have value from 0 to 65535 if aNetType is GSM or
        * it can have value from 0 to 268435455 if aNetType is WCDMA 
        * @param [OUT] aLac Location Aread Code. This parameter is written to settings only if aNetType is GSM.
        * @return one of the following error codes: 
        *         - KErrNone if network information retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetNetInfoLastSucess( 
               const TInt64 aSlpId, 
               CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,
               TInt& aMcc,
               TInt& aMnc,
               TInt& aCid, 
               TInt& aLac
        ) const;

     

       /**
        * This method is used to change parameter which indicates whether server
        * supports TLS or not. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aTlsSupportFlag Flag to indicate whether TLS is needs to be enabled or not. 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetTlsSupportFlag( 
               const TInt64 aSlpId, 
               const TBool aTlsSupportFlag 
        );

       /**
        * This method is used to retrieve parameter which indicates whether server
        * supports TLS or not. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aTlsFlag Flag to indicate whether TLS is enabled or not 
        * @return one of the following error codes: 
        *         - KErrNone if TLS support flag is retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetTlsFlag(
               const TInt64 aSlpId, 
               TBool& aTlsFlag 
        ) const;
        

       /**
        * This method is used to change parameter which indicates whether server
        * supports PSK-TLS or not. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
        * @param [IN] aPskTlsSupportFlag Flag to indicate whether PSK-TLS is needs to be enabled or not. 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt SetPskTlsSupportFlag( 
               const TInt64 aSlpId, 
               const TBool aPskTlsSupportFlag 
        );

       /**
        * This method is used to retrieve parameter which indicates whether server
        * supports PSK-TLS or not. 
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
        * @param [OUT] aPskTlsFlag Flag to indicate whether PSK-TLS enabled or not. 
        * @return one of the following error codes: 
        *         - KErrNone if PSK-TLS support flag is retrieved successfully 
        *         - KErrArgument if the SLP ID passed is not valid 
        */
        IMPORT_C TInt GetPskTlsFlag(
               const TInt64 aSlpId, 
               TBool& aPskTlsFlag 
        ) const;
        
       /**
        * This method is used to set one server entry as default in SUPL settings.  
        *
        * @since S60 9.1TB 
        *
        * @param [IN] aSlpId SLP ID of server entry which needs to set as default server 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are changed successfully.  
        *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
        *         - KErrNotFound if the specified server identity does not exists in the settings
        */

        IMPORT_C TInt SetTopPriority( 
               const TInt64 aSlpId 
        );

       /**
        * This method is used to retrieve default SLP identification. 
        *
        * @since S60 9.1TB 
        *
        * @param [OUT] aSlpId SLP ID of default server 
        *
        * @return one of the following error codes: 
        *         - KErrNone if SUPL settings are retrieved successfully.  
        *         - KErrNotFound if settings does not have any server entries or if none of server is marked as default 
        *
        */
        IMPORT_C TInt GetDefaultSlpId(
               TInt64& aSlpId 
        ) const;

        /**
         * This method is used to delete all servers which have the Sim Change Remove Flag set 
         *
         * @since S60 9.1TB 
         *
         * 
         * @return one of the following error codes: 
         *         - KErrNone if entries were deleted.  
         *         - KErrGeneral if changing settings failed 
         *
         */
        IMPORT_C TInt RemoveOnSimChange();
        /**
         * This Method is used to Set the fallback timer value into the SUPL Settings
         * 
         * @since S60 9.1 TB
         * @return one of the following error codes: 
         *         - KErrNone if entries were deleted.  
         *         - KErrGeneral if retrieving settings failed
         */
        IMPORT_C TInt GetFallBackTimerValue(TInt& aTimerValue);
        
        /**
         * This Method is used to Get the fallback timer value from the SUPL Settings
         * 
         * @since S60 9.1 TB
         * @return one of the following error codes: 
         *         - KErrNone if entries were deleted.  
         *         - KErrGeneral if Setting timer value failed
         */
        IMPORT_C TInt SetFallBackTimerValue(const TInt aTimerValue);
        
       /**
     	* This method is used to insert Active session record into the SUPL session DB
     	* @since S60 5.2
     	*
     	* @param [IN] aSessionParamValues The CServerParams pointer containing trigger session params 
     	*                              to be added to the DB
     	* @return one of the following error codes: 
        *         - KErrNone if entries were added successfully.  
        *         - KErrArgument if entry to be added is NULL
        *           otherwise system wide error codes. 
     	*/
        IMPORT_C TInt InsertSessionRecord(const CTriggerParams* aSessionParamValues);
    
    	 /**
    	  * This method is used to set outstanding triggers attribute for a given session
    	  * @since S60 5.2
    	  *
    	  * @param [IN] aSessionId Session identifier to get/change paramater of particular session 
          * @param [IN] aOutstandingTriggers  Indicates number of outstanding triggers for a given session
    	  * @return one of the following error codes: 
    	  *      - KErrNone if successful
          *      - KErrNotFound if there is no active session with given id
    	  *           otherwise system wide error codes. 
    	  */
        IMPORT_C TInt SetOutstandingTriggers( TInt64 aSessionId,TUint64 aOutstandingTriggers );
    	
    	
    	/**
     	* This method is used to delete a session record corresponding to the given session ID
     	* @since S60 5.2
     	* @param [OUT] aSessionId  The session ID of the session that is to be deleted
     	* @return one of the following error codes: 
        *         - KErrNone if entries were deleted successfully.  
        *         - KErrNotFound if given session id doesnt exist.
        *           otherwise system wide error codes. 
     	*/
        IMPORT_C TInt DeleteSessionRecord(TInt64 aSessionId);
    	
    	/**
    	 * This method is used to delete all session records from session table
    	 * @since S60 5.2
    	 * @return one of the following error codes: 
    	 *         - KErrNone if entries were deleted successfully.  
    	 *           otherwise system wide error codes. 
    	 */
        IMPORT_C TInt DeleteAllSessionRecords();
        
        /**
    	 * This method is used to retrieve extended parameters of given server
    	 * @since S60 5.2
    	 * @param [IN] aSlpId SLP identifier. 
    	 * @param [OUT] aServerExtnParams  Extended parameters of default server.Ownership remains with client.
    	 * @return one of the following error codes: 
    	 *         - KErrNone if parameters were retrieved successfully.
    	 *         - KErrNotFound if server doesnt exist.    
    	 *         - KErrArgument if parameter is NULL.
    	 *           otherwise system wide error codes. 
    	 */
        IMPORT_C TInt GetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams ) const;
        
        /**
    	 * This method is used to set extended parameters of a given server.
    	 * @since S60 5.2
    	 * @param [IN] aSlpId SLP identifier. 
         * @param [IN] aServerExtnParams  Server extended parameters.Ownership remains with client. 
    	 * @return one of the following error codes: 
    	 *         - KErrNone if entries were deleted successfully.  
    	 *         - KErrNotFound if SLP with given id doesnt exist. 
    	 *         - KErrArgument if aServerExtnParams is NULL
    	 *           otherwise system wide error codes. 
    	 */
        IMPORT_C TInt SetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams ); 

    private: // New functions
        
    private:
        // By default, prohibit copy constructor
        CSuplSettingsInternal( const CSuplSettingsInternal& );
        // Prohibit assigment operator
        CSuplSettingsInternal& operator= ( const CSuplSettingsInternal& );

    private:    // Data
    };

#endif      // __EPOS_CSUPLSETTINGSINTERNAL_H__

// End of File
