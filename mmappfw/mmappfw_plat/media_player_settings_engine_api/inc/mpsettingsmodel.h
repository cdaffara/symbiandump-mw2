/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common interface for MMF specific settings models.
*
*/

// Version : %version: 3.1.2 %




#ifndef MMPSETTINGSMODEL_H
#define MMPSETTINGSMODEL_H

// INCLUDES
#include    <e32std.h>
#include    <e32base.h>
#include    <ecom/implementationinformation.h>

// DATA TYPES
// Config file version
enum TConfigVersion
    {
    EConfigDefault,
    EConfigUser
    };

// Bearer type
enum TDataBearer
    {
    EBearerGPRS,
    EBearerEGPRS,
    EBearerWCDMA,
    EBearerCDMA,
    EBearerCDMA2000,
    EBearerWLAN,
    EBearerHSDPA
    };

// CLASS DECLARATION

/**
*  CMPSettingsModel
*  Common API for MMF specific settings models.
*
*  @lib MPSettEngine.dll
*  @since 2.0
*/
class CMPSettingsModel : public CBase
    {
    public:

       /**
        * Constructor. Uses ECom to construct an instance of this class.
        * @param aUid Implementation uid of the object that is to be constructed.
        */
        IMPORT_C static CMPSettingsModel* NewL(TUid aUid);

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CMPSettingsModel();

    public: // New functions

        /**
        * Provides an implementation information array.
        * @since 2.1
        * @param aImplInfoArray An array which is populated with the implementation
        *   information of the available implementations.
        */
        IMPORT_C static void ListImplementationsL(RImplInfoPtrArray& aImplInfoArray);

        /**
        * Reads settings from MMF controller.
        * @since 2.0
        * @param aSettingsType Settings value type:
        *         EConfigDefault: Load default values
        *         EConfigUser:    Load user values
        */
        virtual void LoadSettingsL(TInt aConfigVersion) = 0;

        /**
        * Writes settings to MMF controller.
        * @since 2.0
        */
        virtual void StoreSettingsL() = 0;
        
        /**
        * Sets video contrast
        * @since 2.0
        * @param aContrast Contrast level
        * @return Error value.
        */
        virtual TInt SetVideoContrast(TInt aContrast) = 0;

        /**
        * Returns video contrast level.
        * @since 2.0
        * @param aContrast Contrast level:
        *         0:    Minimum contrast
        *         20:   20% of maximum
        *         40:   40% of maximum
        *         60:   60% of maximum
        *         80:   80% of maximum
        *         100:  Maximum contrast
        * @return Error value.
        */
        virtual TInt GetVideoContrast(TInt& aContrast) = 0;

        /**
        * Sets proxy mode..
        * @since 2.0
        * @param aMode Proxy mode.
        * @return Error value.
        */
        virtual TInt SetProxyMode(TInt aMode) = 0;
        
        /**
        * Returns proxy mode.
        * @since 2.0
        * @param aMode Proxy mode:
        *         0:    Disabled
        *         1:    Enabled
        * @return Error value.
        */
        virtual TInt GetProxyMode(TInt& aMode) = 0;

        /**
        * Sets Proxy host name.
        * @since 2.0
        * @param aHostName Proxy host name.
        * @return Error value.
        */
        virtual TInt SetProxyHostNameL(const TDesC& aHostName) = 0;
        
        /**
        * Returns proxy host name.
        * @since 2.0
        * @param aHostName Proxy host name.
        * @return Error value.
        */
        virtual TInt GetProxyHostName(TDes& aHostName) = 0;

        /**
        * Sets proxy port number.
        * @since 2.0
        * @param aPort Proxy port number.
        * @return Error value.
        */
        virtual TInt SetProxyPort(TInt aPort) = 0;
        
        /**
        * Returns proxy port number..
        * @since 2.0
        * @return Integer: Proxy port number.
        * @return Error value.
        */
        virtual TInt GetProxyPort(TInt& aPort) = 0;
        
        /**
        * Sets default access point.
        * @since 2.0
        * @param aApId Access point ID.
        * @return Error value.
        */
        virtual TInt SetDefaultAp(TUint32 aApId) = 0;

        /**
        * Returns default access point ID.
        * @since 2.0
        * @param aApId Access point ID.
        * @return Error value.
        */
        virtual TInt GetDefaultAp(TUint32& aApId) = 0;

        /**
        * Sets bandwidth control mode.
        * @since 2.0
        * @param aMode bandwidth control mode.
        * @return Error value.
        */
        virtual TInt SetBandwidthControlMode(TInt aMode) = 0;
        
        /**
        * Returns bandwidth control mode.
        * @since 2.0
        * @param aMode bandwidth control mode:
        *           0: Use dynamic bw control if possible
        *           1: Use static bw control
        * @return Error value.
        */
        virtual TInt GetBandwidthControlMode(TInt& aMode) = 0;

        /**
        * Sets maximum bandwidth.
        * @since 2.1
        * @param aMaxBw Maximum bandwidth.
        * @param aBearer Bearer, which max bw is set.
        * @return Error value.
        */
        virtual TInt SetMaxBandwidth(TInt aMaxBw, TDataBearer aBearer) = 0;
        
        /**
        * Returns maximum bandwidth.
        * @since 2.1
        * @param aMaxBw Maximum bandwidth.
        * @param aBearer Bearer, which max bw is enquired.
        * @return Error value.
        */
        virtual TInt GetMaxBandwidth(TInt& aMaxBw, TDataBearer aBearer) = 0;

        /**
        * Sets connection timeout.
        * @since 2.0
        * @param aTimeout Timeout in minutes.
        * @return Error value.
        */
        virtual TInt SetConnectionTimeout(TInt aTimeout) = 0;
        
        /**
        * Returns connection timeout.
        * @since 2.0
        * @param aTimeout Timeout in minutes.
        * @return Error value.
        */
        virtual TInt GetConnectionTimeout(TInt& aTimeout) = 0;
        
        /**
        * Sets server timout.
        * @since 2.0
        * @param aTimeout Timeout in minutes.
        * @return Error value.
        */
        virtual TInt SetServerTimeout(TInt aTimeout) = 0;
        
        /**
        * Returns server timeout.
        * @since 2.0
        * @param aTimeout Timeout in minutes.
        * @return Error value.
        */
        virtual TInt GetServerTimeout(TInt& aTimeout) = 0;

        /**
        * Sets minimum UDP port number.
        * @since 2.0
        * @param aPort minimum port number.
        * @return Error value.
        */
        virtual TInt SetMinUDPPort(TInt aPort) = 0;

        /**
        * Returns minimum UDP port number.
        * @since 2.0
        * @param aPort minimum UDP port number in minutes.
        * @return Error value.
        */
        virtual TInt GetMinUDPPort(TInt& aPort) = 0;

        /**
        * Sets maximum UDP port number.
        * @since 2.0
        * @param aPort maximum port number.
        * @return Error value.
        */
        virtual TInt SetMaxUDPPort(TInt aPort) = 0;

        /**
        * Returns maximum UDP port number.
        * @since 2.0
        * @param aPort maximum UDP port number in minutes.
        * @return Error value.
        */
        virtual TInt GetMaxUDPPort(TInt& aPort) = 0;

        /**
        * Provides MMF controller version information.
        * @since 2.0
        * @param aVersion Controller version number.
        * @return Error value.
        */
        virtual TInt GetControllerVersionInfo(TDes& aVersion) = 0;
            
        /**
        * Provides MMF controller's build date.
        * @since 2.0
        * @param aBldDate Controller build date.
        * @return Error value.
        */
        virtual TInt GetControllerBuildDate(TDes& aBldDate) = 0;

        /**
        * Provides additional information about MMF controller.
        * @since 2.0
        * @param aAdditionalInfo Additional controller information.
        * @return Error value.
        */
        virtual TInt GetControllerAdditionalInfo(TDes& aAdditionalInfo) = 0;

        /**
        * Sets demand bandwidth factor.
        * @since 2.1
        * @param aFactor Demand bandwidth factor.
        * @return Error value.
        */
        virtual TInt SetDemandBwFactor(TInt aFactor) = 0;

        /**
        * Returns demand factor bandwidth.
        * @since 2.1
        * @param aFactor Demand bandwidth factor.
        * @return Error value.
        */
        virtual TInt GetDemandBwFactor(TInt& aFactor) = 0;       

        /**
        * Sets sustainable bandwidth.
        * @since 2.1
        * @param aSustainBw Sustainable bandwidth.
        * @param aBearer Bearer, which sustain bw is set.
        * @return Error value.
        */
        virtual TInt SetSustainBandwidth(TInt aSustainBw, TDataBearer aBearer) = 0;
        
        /**
        * Returns sustainable bandwidth.
        * @since 2.1
        * @param aSustainBw Sustainable bandwidth.
        * @param aBearer Bearer, which sustain bw is enquired.
        * @return Error value.
        */
        virtual TInt GetSustainBandwidth(TInt& aSustainBw, TDataBearer aBearer) = 0;

        /**
        * Returns an array of the fixed sustainable bandwidth values.
        * @since 2.1
        * @param aBwArray Fixed sustain bandwidth values are copied to this array.
        * @param aBearer Bearer, which fixed sustain bw values is enquired.
        * @return Error value.
        */
        virtual TInt GetSustainBwPresetsL(RArray<TInt>& aBwArray, TDataBearer aBearer) = 0;

        /**
        * Returns an array of the fixed max bandwidth values.
        * @since 2.1
        * @param aBwArray Fixed max bandwidth values are copied to this array.
        * @param aBearer Bearer, which fixed max bw values is enquired.
        * @return Error value.
        */
        virtual TInt GetMaxBwPresetsL(RArray<TInt>& aBwArray, TDataBearer aBearer) = 0;

	/**
        * Returns video repeat mode.
        * @since 2.0
        * @return ETrue:    On
        *         EFalse:   Off
        */
        virtual TBool IsVideoRepeatOnL() = 0;

        /**
        * Sets video repeat mode.
        * @since 2.0
        * @param aRepeat Repeat mode.
        */
        virtual void SetVideoRepeatL(const TBool aRepeat) = 0;

		/**
        * Returns video view mode.
        * @since 3.2
        * @return ETrue:    On  (Normal-screen view)
        *         EFalse:   Off (Full-screen view)
        */
        virtual TBool IsDefaultViewOnL() = 0;

        /**
        * Sets video view mode.
        * @since 3.2
        * @param aView View mode.
        */
        virtual void SetDefaultViewL(const TBool aView) = 0;

		/**
        * Returns if Rocker Keys Feature is supported
        * @since 3.2
        * @return ETrue:    - Rocker Keys feature is supported
        *         EFalse:   - Rocker Keys feature is not supported
        */
        virtual TBool IsRockerKeysSupportedL() = 0;

        /**
        * Sets rocker keys mode.
        * @since 3.2
        * @param aRockerKeys rocker keys mode.
        */
        virtual void SetRockerKeysL(const TBool aRockerKeys) = 0;

		/**
        * Returns Rocker Keys mode
        * @since 3.2
        * @return ETrue:    Show (Rocker Keys are shown)
        *         EFalse:   Hide (Rocker Keys are hidden)
        */
		virtual TBool ShowRockerKeysL() = 0;

		/**
        * Sets Media Player auto disconnection time.
        * @since 2.0
        * @param aTime Auto disconnection time.
        */
        virtual void SetAutoDisconTimeL(const TInt aTime) = 0;

        /**
        * Returns Media Player auto disconnection time.
        * @since 2.0
        * @return 0:    Unlimited time
        *         1-99: Time in minutes
        */
        virtual TInt AutoDisconTimeL() = 0;


    protected:
        TUid iDtor_ID_Key;
    };

#endif      // MMPSETTINGSMODEL_H   
            
// End of File
