/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       See class description below.
*
*/



#ifndef __EPOS_CSUPLSETTINGSENGINE_H__
#define __EPOS_CSUPLSETTINGSENGINE_H__

//  INCLUDES
#include <e32base.h>
#include <etel.h>
#include <etelmm.h>

#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"
// FORWARD DECLARATIONS
class CRepository;
class CSuplEtelRequest;
class CServerParams;
class CTriggerParams;
class CServerExtensionParams;
class CSuplSettings;
class CSettingsDatabaseHandler;

// CONSTANTS

// CLASS DECLARATION

/**
*  This class is used for accessing central repository and to get
*  IMSI from ETEL. 
*
*  This is part of @lib epos_suplsettings.lib
*  @since 3.1
*/
class CSuplSettingsEngine : public CBase
    {


   public:
        enum TEtelReqState
	    {
        EUndefined,                
	    EImsi,
		ELoc
		};            
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        static CSuplSettingsEngine* NewL();

        /**
        * Destructor.
        */
        virtual ~CSuplSettingsEngine();

    private:

        /**
        * C++ default constructor.
        */
        CSuplSettingsEngine();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

    public: // New functions

        /**
        * This method connects to ETEL, instantiates ETEL request
        * active object.  This is an asynchronous call.  
        * It retrieves IMSI value from IMSI.
        */
        void Initialize(TRequestStatus& aStatus);        

        /**
        * This method connects to ETEL.
        */
        TInt ConnectToEtelL();

        /**
        * This method cancels initialization request. 
        */
        void CancelInitialize();

       /* *
        * This method sets HSLP address, IAP Name and changed by field
        * in SUPL settings. 
        
        TInt Set( 
         IN         const TDesC&  aHslpAddress, 
         IN         const TDesC&  aIapName,
         IN         const TInt aChangedBy
        );

        *
        * This method retrieves HSLP address, IAP Name, IMSI vlaue and 
        * changed by field from SUPL settings. 
        
        TInt Get(
         OUT        TDes&    aHslpAddress, 
         OUT        TDes&  aIapName, 
         OUT        TDes&  aImsi,
         OUT        TInt&     aChangedBy
        );
*/
        /**
        * This method generates HSLP address from the IMSI retrieved from
        * ETEL during initialization. 
        */
        TInt GetHslpAddressFromImsi(
        /* OUT */       TDes& aHslpAddr
        );

        /**
        * This method creates a concurrent read/write transaction.
        * The method leaves a clean-up item to clean-up stack to roll back
        * the changes in case of leave.
        * Remember to pop the item from the stack before making commit.
        */
        TInt LockEngineL();

        /**
        * This method commits a concurrent read/write transaction and pops
        * the roll-back clean-up item from the stack
        *
        * The method handles possible roll back in case of failure.
        * 
        */
        TInt UnLockEngine(TBool aWithCommit);

    /**
     * This method retrieves currently active IMSI.  Currently active IMSI
     * is retrieved during initialization time.
     *
     * @param aImsi will hold, on successful completion, currently active IMSI
     * @return one of the following error codes returned
     *  - KErrNone if method completed successfully
     *  - KErrNotReady if this method is called before @ref Initialize
     */

    TInt GetImsi(TDes& aImsi);

    /**
     * This method compares IMSI value currently stored in SUPL settings and
     * currently active IMSI. If both are same, this method returns false else
     * returns true.   
     * 
     * @param aChanged will hold, on successful completion, one of the following value
     *   - ETrue if currently active IMSI and IMSI value stored in SUPL settings
     *   are different.
     *   - EFalse if currently active IMSI and IMSI value stored in SUPL settings
     *   are same.
     * @return one of the following
     *  - KErrNone if method completed successfully
     *  - KErrNotReady if this method is called before @ref Initialize
     */
    TInt IsImsiChanged(TBool& aChanged);

       /**
        * This method is used to retrieve SUPL usage from settings storage.  
        *
        * @param [OUT] aUsage  will hold, upon successful completion, value indicating 
	    * current SUPL usage 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL usage retrieved successfully.  
        *         - KErrNotFound if the SUPL State is not found in SUPL settings
        *         - KErrUnknown if retrieving SUPL usage from settings storage failed
        */

    TInt GetSuplUsage(TInt& aUsage);

       /**
        * This method is used to change SUPL usage.  Using this method, SUPL usage can be 
        * set to always ask or automatic or automatic in home network or disabled.  
        * By default, SUPL usage will be set to always ask. 
        *
        * Note that this method should be invoked only after settings are locked 
        * using @ref LockSettingsL() method. After changing SUPL usage, settings
        * should be unlocked using @ref UnlockSettings() method.
        *
        * @param [IN] aUsage Usage to be set for SUPL usage 
        * @return one of the following error codes: 
        *         - KErrNone if SUPL usage is changed successfully.  
        *         - KErrArgument if aUsage field is not in range
        *         - KErrNotReady if this method is invoked before @ref LockSettingsL()
        *         - KErrUnknown if changing SUPL usage in settings storage has failed
        */
    TInt SetSuplUsage(const TInt aUsage);
    /*
     * This method is used to check if the initialisation has been done  using @ref Initialize()
     * 
     * @return ETrue if init has been done
     *         EFalse otherwise
     */
    TBool IsInitDone();
    /*
     * This method is used to check if the initialisation has been successful 
     * 
     * @return ETrue if init has been succesful
     *         EFalse otherwise
     */
    TBool IsInitPassed();

    /**
     * This method is used to add new server into settings.  It is client's responsibility
     * to set all the parameters in aParamValues except SLP identification which will
     * generated automatically.
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aParamValues Server parameter values which which the server entry needs to be added 
     * @param [OUT] aSlpId SLP identification for the new server added 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     */ 
    TInt AddNewServer( 
            const CServerParams* aParamValues, 
            TInt64& aSlpId 
    );


    /**
     * This method is used to change server address parameter in server settings 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aServerAddres SUPL Server address 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if address passed is empty or if the SLP ID passed is not valid 
     */

    TInt SetServerAddress( 
            const TInt64 aSlpId, 
            const TDesC& aServerAddress
    );

    /**
     * This method is used to retrieve server address parameter from server settings.  Client
     * has to allocate memory for server address parameter.
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID of server for which the server address needs to be retrieved 
     * @param [OUT] aServerAddres SUPL Server address 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */

    TInt GetServerAddress( 
            TInt64 aSlpId, 
            TDes& aServerAddress
    ) const;

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
    TInt SetVersion( 
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
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetVersion( 
            const TInt64 aSlpId, 
            TInt& aMajor,
            TInt& aMinor 
    ) const;

    /**
     * This method is used to change IAP Name in server parameters. IAP name refers to 
     * the access point which is used to access the HSLP over the internet. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters. 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if IAP Name is empty or if the SLP ID passed is not valid 
     */
    TInt SetIapName( 
            const TInt64 aSlpId, 
            const TDesC& aIapName
    );

    /**
     * This method is used to retrieve IAP Name in server parameters. IAP name refers to 
     * the access point which is used to access the HSLP over the internet. It is client's
     * responsibility to allocate memory for IAP name before invoking this method.
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aIapName The Internet Access Point Name refers to the access point which is 
     * used to access the HSLP over the internet.  This can have maximum size of 100 characters. 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetIapName( 
            const TInt64 aSlpId, 
            TDes& aIapName
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
    TInt SetManufacturerName( 
            const TInt64 aSlpId, 
            const TDesC& aManufacturer
    );

    /**
     * This method is used to retrieve manufacturer name in server parameters. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aManufacturer Manufacture name.  This can have maximum size of 100 characters. 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetManufacturerName( 
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
    TInt SetLastUseTime( 
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
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetLastUseTime( 
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
    TInt SetLastTriedTime( 
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
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetLastTriedTime( 
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
    TInt SetNetInfoLastUse( 
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
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetNetInfoLastUse( 
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
    TInt SetNetInfoLastSucess( 
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
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetNetInfoLastSucess( 
            const TInt64 aSlpId, 
            CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,
            TInt& aMcc,
            TInt& aMnc,
            TInt& aCid, 
            TInt& aLac
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
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt SetEmergencySupportFlag( 
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
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetEmergencySupportFlag( 
            const TInt64 aSlpId, 
            TBool& aEmergencySupportFlag 
    ) const;

    /**
     * This method is used to retrieve parameter which indicates whether server
     * supports TLS or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aTlsFlag Flag to indicate whether TLS is enabled or not. 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetTlsFlag(const TInt64 aSlpId,TBool& aTlsFlag )const;

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
    TInt SetTlsSupportFlag( 
            const TInt64 aSlpId, 
            const TBool aTlsSupportFlag 
    );

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
    TInt SetPskTlsSupportFlag( 
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
     * @param [OUT] aPskTlsFlag Flag to indicate whether PSK-TLS needs to be enabled or not. 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetPskTlsFlag(
            const TInt64 aSlpId, 
            TBool& aPskTlsFlag 
    ) const;

    /**
     * This method is used to change parameter which indicates whether server
     * can be used for SUPL session or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aEnable Flag to indicate whether server can be enabled or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt SetServerEnabledFlag(
            const TInt64 aSlpId, 
            const TBool aEnable 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * can be used for SUPL session or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aEnable Flag to indicate whether server can be enabled or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetServerEnabledFlag(
            const TInt64 aSlpId, 
            TBool& aEnable 
    ) const;

    /**
     * This method is used to change parameter which indicates whether server
     * details can be removed if SIM is changed. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aSimChangeFlag Flag to indicate whether server can be removed or not if there is a SIM change 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt SetSimChangeRemoveFlag(
            const TInt64 aSlpId, 
            const TBool aSimChangeFlag 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * details can be removed if SIM is changed. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aSimChangeFlag Flag to indicate whether server can be removed or not if there is a SIM change 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetSimChangeRemoveFlag(
            const TInt64 aSlpId, 
            TBool& aSimChangeFlag 
    ) const;

    /**
     * This method is used to change parameter which indicates whether server
     * can be used out side home network or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aHomeNwFlag Flag to indicate whether server can be used outside home network or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt SetUsageInHomwNwFlag(
            const TInt64 aSlpId, 
            const TBool aHomeNwFlag 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * can be used out side home network or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aHomeNwFlag Flag to indicate whether server can be used outside home network or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */
    TInt GetUsageInHomwNwFlag(
            const TInt64 aSlpId, 
            TBool& aHomeNwFlag 
    ) const;

    /**
     * This method is used to change parameter which indicates whether server
     * details are editable or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
     * @param [IN] aEditFlag Flag to indicate whether server details are editable or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL() 
     *         - KErrArgument if the SLP ID passed is not valid 
     */ 
    TInt SetEditableFlag( 
            const TInt64 aSlpId, 
            const TBool aEditFlag 
    );

    /**
     * This method is used to retrieve parameter which indicates whether server
     * details are editable or not. 
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
     * @param [OUT] aEditFlag Flag to indicate whether server details are editable or not 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrArgument if the SLP ID passed is not valid 
     */ 
    TInt GetEditableFlag( 
            const TInt64 aSlpId, 
            TBool& aEditFlag 
    ) const;

    /**
     * This method is used to change all parameters of server in SUPL settings.  The server identity
     * in @ref CServerParams which is passed as parameter gives information on for which server
     * entry the values needs to be changed.  If the passed server identity does not exists in
     * settings, error is returned.  
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aParamValues New value for server parameters. 
     * @ref CServerParams class
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if the value of any of the pamater is not in range
     *         - KErrNotFound if the SLP Identity mentioned in aParamValues does not exist in settings 
     */

    TInt SetAllParameter( 
            const CServerParams* aParamValues
    );

    /**
     * This method is used to change all parameters of default server in SUPL settings.  In this
     * method server identity of @ref CServerParams passed is not considered and values
     * of default server is updated except server identity.  If none of the server is set
     * as default, error is returned.
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aParamValues Parameter for which the value needs to be changed  
     * @ref CServerParams class
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrArgument if the value of any of the pamater is not in range
     *         - KErrNotFound if none of the server is set as default
     */

    TInt SetDefaultServer( 
            const CServerParams* aParamValues
    );

    /**
     * This method is used to set one server as default in SUPL settings storage.  
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId SLP ID for which the parameters needs to be changed 
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are changed successfully.  
     *         - KErrNotReady if this method is invoked before invoked before @ref LockSettingsL()
     *         - KErrNotFound if the specified server identity does not exists in the settings
     */

    TInt SetAsDefault( 
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
    TInt GetDefaultSlpId(
            TInt64& aSlpId 
    ) const;

    /**
     * This method is deletes server entry from SUPL settings storage.  
     *
     * @since S60 9.1TB 
     *
     * @param [IN] aSlpId Server identity for which the entery needs to be deleted from the SUPL settings storage 
     *
     * @return one of the following error codes: 
     *         - KErrNone if entry is deleted sucessfully 
     *         - KErrNotFound if the specified server identity does not exists in settings storage 
     *
     */
    TInt RemoveServer(TInt64 aSlpId);

    /**
     * This method is changes the priority of the server to the specified priority.  
     *
     * @since S60 9.1TB 
     *
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
     * This method is used to retrieve all properties of default server from SUPL settings storage.  
     *
     * @since S60 9.1TB 
     *
     * @param [OUT] aParamValues Retrieves values from SUPL Settings storage 
     * @ref CServerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if SUPL settings are retrieved successfully.  
     *         - KErrNotFound if the settings does not have any server entry 
     *
     */
    TInt GetDefaultServer(
            CServerParams* aParamValues
    ) const;
    #if 0
           /**
            * This method is used to retrieve properties of all servers from SUPL settings storage.  
            *
            * @since S60 9.1TB 
            *
            * @param [OUT] aParamValues Array of retrieved server entries 
            * @ref CServerParams class
            *
            * @return one of the following error codes: 
            *         - KErrNone if SUPL settings are retrieved successfully.  
            *         - KErrNotFound if the settings does not have any server entry 
            *
            */
             TInt GetAllSlp(
                   RPointerArray<CServerParams> aParamValues,
                   TRequestStatus& aStatus
            ) const;
    #endif
     /**
      * This method enables or disables the fall back in SUPL settings storage.  
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aFallBack Indicates whether the fallback needs to be enabled or disabled 
      *
      * @return one of the following error codes: 
      *         - KErrNone if the fallback value changed sucessfully 
      *         - KErrGeneral if the fallback is enabled when only one server entry is present
      */
     TInt SetFallBack(TBool aFallBack);

     /**
      * This method retrieves the fallback value from SUPL settings storage 
      *
      * @since S60 9.1TB 
      *
      * @param [OUT] aFallBack Retrived fallback value 
      *
      * @return one of the following error codes: 
      *         - KErrNone if the fallback value retrieved sucessfully 
      */
     TInt GetFallBack(TBool& aFallBack) const;

     /**
      * This method gets the fall back timer from SUPL settings storage.  
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aFallBack Indicates whether the fallback needs to be enabled or disabled 
      *
      * @return one of the following error codes: 
      *         - KErrNone if the fallback value changed sucessfully 
      *         - KErrGeneral if the fallback is enabled when only one server entry is present
      */
     TInt GetFallBackTimer(TInt& aTimerValue) const;

     /**
      * This method sets the fall back timer value in SUPL settings storage.  
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aFallBack Indicates whether the fallback needs to be enabled or disabled 
      *
      * @return one of the following error codes: 
      *         - KErrNone if the fallback value changed sucessfully 
      *         - KErrGeneral if the fallback is enabled when only one server entry is present
      */
     TInt SetFallBackTimer(TBool aTimerValue);

     /**
      * This method is used to retrieve all properties of server from SUPL settings storage
      * based on SLP identification.  
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aSlpId SLP ID for which the parameters needs to be changed 
      * @param [OUT] aParamValues Parameter for which the value needs to be changed  
      * @ref CServerParams class
      *
      * @return one of the following error codes: 
      *         - KErrNone if SUPL settings are retrieved successfully.  
      *         - KErrArgument if any of the argument passed is not valid 
      *         - KErrNotFound if the specified server does not exists in the settings
      *
      */
     TInt GetSlpInfoFromId(
             const TInt64 aSlpId, 
             CServerParams* aParamValues
     ) const;

     /**
      * This method is used to retrieve all properties of server from SUPL settings storage
      * based on SLP address.  
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aServerAddress SLP address for which the parameters needs to be changed 
      * @param [OUT] aParamValues Parameter for which the value needs to be changed  
      * @ref CServerParams class
      *
      * @return one of the following error codes: 
      *         - KErrNone if SUPL settings are retrieved successfully.  
      *         - KErrArgument if any of the argument passed is not valid 
      *         - KErrNotFound if the specified server does not exists in the settings
      *
      */
     TInt GetSlpInfoAddress(
             const TDesC& aServerAddress, 
             CServerParams* aParamValues
     ) const;

     /**
      * This method is used to retrieve number of server entries present in SUPL settings. 
      *
      * @since S60 9.1TB 
      *
      * @param [OUT] aCount Number of SLP entries 
      *
      * @return one of the following error codes: 
      *         - KErrNone if number of SLP entries are calculated 
      *         - KErrNotFound if the settings does not have any server entry 
      *
      */
     TInt SlpCount(
             TInt& aCount 
     );

     /**
      * This method is used check whether server entry exists or not based on SLP
      * identification 
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aSlpId SLP identification 
      *
      * @return one of the following codes: 
      *         - ETrue if server entry exists in settings 
      *         - EFalse if server entry does not exists in settings 
      *
      */
     TBool IsSlpExists(
             const TInt64 aSlpId 
     );

     /**
      * This method is used check whether server entry exists or not based on SLP
      * address 
      *
      * @since S60 9.1TB 
      *
      * @param [IN] aServerAddress Server address 
      *
      * @return one of the following codes: 
      *         - ETrue if server entry exists in settings 
      *         - EFalse if server entry does not exists in settings 
      *
      */
     TBool IsSlpExists(
             const TDesC& aServerAddress 
     );


     /**
      * This synchronous method is used to retrieve properties of all servers from SUPL settings storage.  
      * This method retrieves all properties for each server.
      *
      * @since S60 9.1TB 
      *
      * @param [OUT] aParamValues Array of retrieved server entries 
      * @ref CServerParams class
      *
      * @return one of the following error codes: 
      *         - KErrNone if SUPL settings are retrieved successfully.  
      *         - KErrNotFound if the settings does not have any server entry 
      *
      */
     TInt GetAllSlp(
             RPointerArray<CServerParams>& aParamValues
     );

     /**
      * This method is used to retrieve properties of all servers from SUPL settings storage.  
      * This method retrieves all properties for each server.
      *
      * @since S60 9.1TB 
      *
      * @param [OUT] aParamValues Array of retrieved server entries 
      * @ref CServerParams class
      *
      * @return one of the following error codes: 
      *         - KErrNone if SUPL settings are retrieved successfully.  
      *         - KErrNotFound if the settings does not have any server entry 
      *
      */
     TInt GetAllSlp(
             RPointerArray<CServerParams>& aParamValues,
             TRequestStatus& aStatus
     );
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
     TInt RemoveOnSimChange();
     
    /**
     * This method is used to retrieve properties of all active triggers. 
     *
     * @since S60 5.2
     *
     * @param [OUT] aParamValues Array of retrieved trigger parameters 
     * @ref CTriggerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if active trigger parameters are retrieved successfully.  
     *         - KErrNotFound if there are no active triggers 
     *           otherwise system wide error codes. 
     *
     */
    TInt GetTriggerParams( RPointerArray<CTriggerParams>& aParamValues ) const;

    /**
     * This method is used to retrieve properties of a given active trigger. 
     *
     * @since S60 5.2
     *
     * @param [IN] aSessionId Session identifier to get paramater of particular session
     * @param [OUT] aParamValues Parameters for a given session 
     * @ref CTriggerParams class
     *
     * @return one of the following error codes: 
     *         - KErrNone if active trigger parameters are retrieved successfully.  
     *         - KErrNotFound if there is no active session with given id 
     *           otherwise system wide error codes. 
     *
     */
    
    TInt GetTriggerParams( TInt64 aSessionId,CTriggerParams*& aParamValues ) const;
    
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
     *      - KErrNotFound if there is no active session with given id 
     *           otherwise system wide error codes. 
     */
	TInt SetNotificationStatus( TInt64 aSessionId,
                                TBool aTriggerNotificationStatus
                              );
                
	/**
	 * Sets number of outstanding triggers for a particular session
	 *
	 * @since S60 5.2
	 * @param [IN] aSessionId Session identifier to get/change paramater of particular session 
	 * @param [IN] aOutstandingTriggers  Indicates number of outstanding triggers for a given session
	 *
	 * @return following error codes
	 *      - KErrNone if successful
	 *      - KErrNotFound if there is no active session with given id
	 *           otherwise system wide error codes. 
	 */
	
	TInt SetOutstandingTriggers( TInt64 aSessionId,TUint64 aOutstandingTriggers );
	
	/*
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
	TInt CancelTriggerSession( TInt64 aSessionId );
	
	
	/**
    * This method is used to insert Active session record into the SUPL session DB
    * @since S605.2
    *
    * @param [IN] aSessionParamValues The CServerParams pointer containing trigger session params 
    *                              to be added to the DB
    *      - KErrNone if successful
    *        otherwise system wide error codes.  
    */
    TInt InsertSessionRecord(const CTriggerParams* aSessionParamValues);
    
    /**
     * This method is used to delete a session record corresponding to the given session ID
     * @since S60 5.2 
     * @param [OUT] aSessionId  The session ID of the session that is to be deleted
     *      - KErrNone if successful
     *      - KErrNotFound if session is not found
     *           otherwise system wide error codes. 
     */
    TInt DeleteSessionRecord(TInt64 aSessionId); 
    
    /**
     * This method is used to delete all session records from session table
     * @since S60 5.2 
     *      - KErrNone if successful
     *           otherwise system wide error codes. 
     */
    
    TInt DeleteAllSessionRecords();
    
    /**
     * This method is used to retrieve extended parameters of given server
     * @since S60 5.2 
     */
    
    TInt GetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams ) const;
    
    /**
     * This method is used to set extended parameters of a given server.
     * @since S60 5.2 
     */
    
    TInt SetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams );
    
  
    /*
	 * Changes status of SUPL triggering service. 
	 *
	 * @since S60 5.2 
	 * @param [IN] aSuplTriggerStatus, Indicates that SUPL  triggering service needs to be ON ( ESuplTriggerOn)  or OFF (ESuplTriggerOff)
	 *
	 * @return following error codes
	 *      - KErrNone if successful
	 *           otherwise system wide error codes. 
	 */
	 TInt SetSuplTriggeredServiceStatus( const CSuplSettings::TSuplTriggerStatus& aSuplTriggerStatus );
        
    /*
	 * Gets status of SUPL triggering service.SUPL Triggering service can be either ESuplTriggerOn or ESuplTriggerOff
	 *
	 * @since S60 5.2 
	 * @param [OUT] aSuplTriggerStatus, Gets status of SUPL triggering session either ON or OFF
	 *
	 * @return following error codes
	 *      - KErrNone if successful
	 *           otherwise system wide error codes. 
	 */
	 TInt GetSuplTriggeredServiceStatus( CSuplSettings::TSuplTriggerStatus& aSuplTriggerStatus );
        
    /**
     * This method is used to read the default configured IAP name from the product config
     * central repository.
     * @since S60 5.2
     */ 
    TInt GetDefaultIAPName(TDes& aIapName);
    
    private: // New functions        
        void HandleNetInfoRequestComplete(TInt aErr, RMobilePhone::TMobilePhoneSubscriberId aImsi);
        void HandleMncInfoRequestComplete(TInt aErr, TInt aMcc, TInt aMnc);
        TInt GetMncLen(TInt aMcc);

    private:
        // By default, prohibit copy constructor
        CSuplSettingsEngine( const CSuplSettingsEngine& );
        // Prohibit assigment operator
        CSuplSettingsEngine& operator= ( const CSuplSettingsEngine& );

    private:    // Data
        TEtelReqState   iState;
        CRepository*            iRepository;
        
        RMobilePhone::TMobilePhoneSubscriberId	iImsi;
        RTelServer      iTelServer;            
        RMobilePhone    iMobPhone;
        RMobilePhone::TMobilePhoneNetworkInfoV1 iNetworkInfo;
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg iNetworkInfoPkg;
        RMobilePhone::TMobilePhoneLocationAreaV1 iLocationInfo;
        TRequestStatus*  iStatus;
        TBool           iInitDone;
        TBool           iInitPassed;
        TFileName       iTsyName;
        TInt iMcc;
        TInt iMnc;
        //pointer to the DB Handler Class that accesses the Settings Db
        CSettingsDatabaseHandler* iSettingsDBHandler;
        };

#endif      // __EPOS_CSUPLSETTINGSENGINE_H__

// End of File
