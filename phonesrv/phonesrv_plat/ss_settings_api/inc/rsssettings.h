/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Class for getting information about the Clir, Cug, Als and Als blocking
*     status in the phone. 
*
*/


#ifndef RSSSETTINGS_H
#define RSSSETTINGS_H

//  INCLUDES
#include <etelmm.h>
#include <rcustomerserviceprofilecache.h> 

// DATA TYPES

/**
* All enumerations are fixed - DO NOT CHANGE!!
*/

/**
* It enumerates all SSSettings settings.
*
* ESSSettingsCug         - closed user group setting, 
*                     for some values see TSSSettingsCugValue.
* ESSSettingsClir        - default setting for calling line identification 
*                     restriction, see TSSSettingsClirValue.
* ESSSettingsAls         - selected line, see TSSSettingsAlsValue.
* ESSSettingsAlsBlocking - line selection blocking, see TSSSettingsAlsBlocking.
* ESSSettingsCugDefault - value for cug default.
*/
enum TSSSettingsSetting
    {
    ESSSettingsCug = 0,  
    ESSSettingsClir = 1, 
    ESSSettingsAls = 2,
    ESSSettingsAlsBlocking = 3,
    ESSSettingsDefaultCug = 4
    };

/**
* It enumerates all values for CLIR.
*
* ESSSettingsClirNetworkDefault - use network default
* ESSSettingsClirExplicitInvoke - by default, invoke CLIR
* ESSSettingsClirExplicitSuppress - by default, suppress CLIR
*/
enum TSSSettingsClirValue
    {
    ESSSettingsClirNetworkDefault,
    ESSSettingsClirExplicitInvoke,
    ESSSettingsClirExplicitSuppress
    };

/**
* It enumerates SOME values for CUG.
* 
* Alternative CUG default (and min value) can be set on/off from 
* shared data (shareddatakeys.h, key: KPhoneCUGIndexStartValue).
*
* CUG value is basically interger.
*
* ESSSettingsCugSuppress - special, suppress CUG
* ESSSettingsCugDefault - special, use network default CUG
* ESSSettingsCugMin - minimum value for proper CUG index.
* ESSSettingsCugMax - maximum value for proper CUG index.
* ESSSettingsAltCugDefault - special, alternative default CUG
* ESSSettingsAltCugMin - special, alternative minimum for CUG
*/
enum TSSSettingsCugValue
    {
    ESSSettingsCugSuppress = -1,
    ESSSettingsCugDefault = 0, //Note: default can be 0 or 65535 (see above)
    ESSSettingsCugMin = 1,     //Note: min value can be 0 or 1 (see above)
    ESSSettingsCugMax = 32767,
    ESSSettingsAltCugDefault = 65535,
    ESSSettingsAltCugMin = 0
    };

/**
* It enumerates all values for ALS.
*
* ESSSettingsAlsNotSupported - ALS not supported, always primary line.
* ESSSettingsAlsPrimary - ALS supported, primary line selected.
* ESSSettingsAlsAlternate - ALS supported, alternate line selected.
*/
enum TSSSettingsAlsValue
    {
    ESSSettingsAlsNotSupported = 0,
    ESSSettingsAlsPrimary = 1,
    ESSSettingsAlsAlternate = 2
    };

/**
* It enumerates all values for ALS blocking.
*
* ESSSettingsAlsBlockingNotSupported - ALS blocking not supported.
* ESSSettingsAlsBlockingOff - ALS blocking off.
* ESSSettingsAlsBlockingOn - ALS blocking on.
*/
enum TSSSettingsAlsBlockingValue
    {
    ESSSettingsAlsBlockingNotSupported,
    ESSSettingsAlsBlockingOff,
    ESSSettingsAlsBlockingOn
    };

// FORWARD DECLARATIONS
class MSSSettingsObserver;
class CSSSettingsNotifier;
class RMmCustomAPI;
class CRepository;


// CLASS DECLARATION

/**
*  It is class that can be used to fetch phone related settings.
*  @since 1.0
*  @lib sssettings.lib
*/
class RSSSettings
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        IMPORT_C RSSSettings();

    public: // New functions
        
        /**
        * Closes SsSettings.
        *
        * After the use of the class, the method must be called.
        */
        IMPORT_C void Close();

        /**
        * Opens SsSettings.
        *
        * Before using the class, one of Open methods must be called.
        *
        * @return error code. KErrNone, if success.
        */
        IMPORT_C TInt Open();

        /**
        * Opens SsSettings.
        *
        * Ownership of parameters is NOT transferred. If parameter
        * is NULL, then the instance creates its own sessions.
        *
        * Before using the class, one of Open methods must be called.
        *
        * @param aTelServer ETel server session.
        * @return error code. KErrNone, if success.
        */
        IMPORT_C TInt Open(
            RTelServer* aTelServer );


        // CHANGE NOTIFICATIONS

        /**
        * Registers an observer to receive all changes.
        *
        * @param aObserver observer.
        * @return error code. If other than KErrNone, CancelAll is called 
        *                     automatically.
        */
        IMPORT_C TInt RegisterAll( 
            MSSSettingsObserver& aObserver );

        /**
        * Registers an observer to receive changes of a setting.
        *
        * @param aSetting setting.
        * @param aObserver observer.
        * @return error code. KErrNone: observer is added
        *                     KErrAlreadyExists: observer is already added
        *                     KErrNotReady: Possible when BT SAP is on.
        *                     KErrNotSupported: Possible with ALS.
        *                     other: observer not registered
        */
        IMPORT_C TInt Register( 
            TSSSettingsSetting aSetting, 
            MSSSettingsObserver& aObserver );

        /**
        * Cancels all registrations of the observer.
        *
        * @param aObserver observer.
        */
        IMPORT_C void CancelAll( 
            MSSSettingsObserver& aObserver );

        /**
        * Cancel specific setting.
        *
        * @param aSetting setting.
        * @param aObserver observer.
        */
        IMPORT_C void Cancel( 
            TSSSettingsSetting aSetting,
            MSSSettingsObserver& aObserver );

        // CHANGE & FETCH VALUE

        /**
        * Fetch value of a setting.
        *
        * @param aSetting setting.
        * @param aValue result will be stored here (if success).
        * @return error code: KErrNone, all ok aValue updated.
        *                     KErrCorrupt, value in shared data is not OK.
        *                     KErrNotSupported, aSetting is not a "real" setting
        *                     KErrNotReady, get CSP from ETEL has failed.
        *                     KErrServerBusy, get CSP from ETEL has failed.
        */
        IMPORT_C TInt Get( 
            TSSSettingsSetting aSetting,
            TInt& aValue );

        /**
        * Changes value of a setting. Successfull function call generates notify
        * call to each observer in this object even if the value is the same. 
        * From other objects of this class the observers will get only change 
        * notifications.
        *
        * @param aSetting setting.
        * @param aValue new value for setting.
        * @return error code. KErrNone in case of non-error.
        *                     KErrArgument: parameter not valid
        *                     KErrNotSupported: In als, if ME doesn't support
        *                     other, from Shared data or from ETel
        */
        IMPORT_C TInt Set(
            TSSSettingsSetting aSetting,
            TInt aValue );

        // SIM CHANGE EVENT
        
        /**
        * It is called when SIM changes.
        *
        * @return error code.
        */
        IMPORT_C TInt HandleSIMChanged();

        /**
        * Get previous CUG value
        * @param aPreviousValue Old value is saved here. 0 if not set
        * @return Error code
        */
        IMPORT_C TInt PreviousCugValue( TInt& aPreviousValue );

        /**
        * Reset previous CUG value
        * @return Error code
        */
        IMPORT_C TInt ResetPreviousCugValue();

        /**
        * Handle refresh
        * @return Error code
        */
        IMPORT_C TInt HandleRefresh();

        /**
        * Check whether or not the given CUG index is valid.
        * @param aValue CUG index
        * @return Boolean, is it valid CUG index or not
        */
        IMPORT_C TBool IsValueValidCugIndex( const TInt& aValue ) const;

    private:
    
        /**
        * Checks if Alternate Line is to be supported.
        * @param aPpAlsValue parameter to be checked.
        * @param aSimAlsValue parameter to be checked.
        * @param aAlsCspSupport parameter to be checked.
        * @param aAlsCspError parameter to be checked.
        * @return error code. KErrNone in case of ALS supported.
        *                     KErrNotSupported in case if ME doesn't support
        */    
        TInt IsALSSupported( TInt aPpAlsValue, TInt aSimAlsValue, 
            TBool aAlsCspSupport, TInt aAlsCspError ) const;
            
        // Gets ALS setting value.
        TInt GetALSSettingValue( TInt& aValue, const TInt aSimState );

        // Prohibit copy constructor if not deriving from CBase.
        RSSSettings( const RSSSettings& );
        // Prohibit assigment operator if not deriving from CBase.
        RSSSettings& operator= ( const RSSSettings& );

        // Opens connection. Leaves on error cases
        void OpenL( RTelServer* aTelServer );

        // Closes all connections and deletes own data
        void DeleteSettingsData();

        // Finds if observer is already added
        TBool ExistsObserver( TSSSettingsSetting aSetting, 
                    MSSSettingsObserver& aObserver );

        // Informs observers about the value change
        void InformChange( TSSSettingsSetting aSetting , TInt aNewValue );

        // Finds next observer, NULL if there isn't any. Used in deleting data.
        MSSSettingsObserver* FindOneObserver() const;

        // Async call back for informing observer about own changes.
        static TInt InformCallBack( TAny* aPtr );

        // CUG value is divided into two parts - needs special handling...
        TInt GetCug( TInt& aValue ) const;
        TInt GetDefaultCugL( TInt& aValue ) const;
        TInt SetCug( const TInt aValue, const TInt aCugDefault ) const;

    private:    // Data

        //Private Array type for RSSSettings.
        typedef RPointerArray<MSSSettingsObserver> RSSSettingsObserverArray;

        /**
        * All data members are included in one structure.
        * This it can be changed accoring to needs, but the
        * BC stays the same.
        */
        struct TSSSettingsData
            {
            RTelServer*          iTelServer;         // ETel client
            TBool                iOwnTelServer;      // ETrue if own
            RMobilePhone         iMobilePhone;       // Mobile Phone
            RArray<RSSSettingsObserverArray> iObservers;// Array for obs. arrays
            CAsyncCallBack*      iCallBack;          // Async call back
            TInt                 iCallBackSetting;   // Setting that is changed
            TInt                 iCallBackValue;     // New value.
            CSSSettingsNotifier* iNotifier;          // Change notifier
            RMmCustomAPI*        iCustomPhone;       // Custom Phone
            TInt                 iCUGDefault;        // CUG default
            CRepository*         iRepository;        // Central repository.           
            };

        TSSSettingsData* iData;                         // The data

    protected:
        // Friend class uses all members in iData. This procedure saves
        // a lot of function from the API which are not meant for
        // external use.
        friend class CSSSettingsNotifier;
    };

// Panic enums
enum TSSSettingsPanic
    {
    SSSettingsPanicOpenNotDone = 6000,  // Some function is called before Open()
    SSSettingsPanicInvalidRequest,      // Given Setting is out of range.
    SSSettingsPanicConflictInAls        // Can't determine ALS state.
    };

GLREF_C void Panic( TSSSettingsPanic aPanic );

#endif      // RSSSETTINGS_H   
            
// End of File
