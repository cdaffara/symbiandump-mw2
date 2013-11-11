/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MT_SSSettings
*
*/


#ifndef __MT_SSSETTINGS_H__
#define __MT_SSSETTINGS_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <etelmm.h>
#include <MSSSettingsObserver.h>
#include <msssettingsrefreshobserver.h>
#include <MSSSettingsRefreshHandler.h>
#include <MSatRefreshObserver.h>
#include <etelsat.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class RCustomerServiceProfileCache;
class RSSSettings;
class CSSSettingsRefreshContainer;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( MT_SSSettings )
     : public CEUnitTestSuiteClass,
       public MSSSettingsObserver,
       public MSSSettingsRefreshObserver,
       public MSSSettingsRefreshHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_SSSettings* NewL();
        static MT_SSSettings* NewLC();
        /**
         * Destructor
         */
        ~MT_SSSettings();

    private:    // Constructors and destructors

        MT_SSSettings();
        void ConstructL();

    public:     // From observer interface
    
          // From MSSSettingsObserver.
          void PhoneSettingChanged( TSSSettingsSetting /*aSetting*/, 
            TInt /*aNewValue*/ );
            
          // From MSSSettingsRefreshHandler.
          void NotifyFileChangeL( 
            MSSSettingsRefreshObserver& /*aObserver*/,
            TSatElementaryFiles /*aObservedFile*/,
            TSatRefreshType /*aObservedRefreshType*/);
            
          // From MSSSettingsRefreshHandler.
          void CancelNotify();
          
          // From MSSSettingsRefreshHandler.
          TInt ChangedCspTable( TDes8& /*aChangedCsps*/,
            TDes8& /*aNewValues*/ );
            
          // From   MSSSettingsRefreshObserver.
          TBool AllowRefresh(
            const TSatRefreshType /*aType*/,
            const TSatElementaryFiles /*aFiles*/ );
            
          // From   MSSSettingsRefreshObserver.
          void Refresh(
            const TSatRefreshType /*aType*/,
            const TSatElementaryFiles /*aFiles*/ );
        

    private:    // New methods
    
         void EmptySetupL();
        
         void EmptyTeardown();

         void SetupL();
        
         void Teardown();
         
         void CSPSetupL();
         
         void CSPTeardown();
        
         void MT_CSSSettingsRefreshContainer_NewLL();
        
        
         void MT_RCustomerServiceProfileCache_CspCallCompletionL();
        
        
         void MT_RCustomerServiceProfileCache_CspCallOfferingL();
        
        
         void MT_RCustomerServiceProfileCache_CspCallRestrictionL();
        
        
         void MT_RCustomerServiceProfileCache_CspCPHSTeleservicesL();
        
        
         void MT_RCustomerServiceProfileCache_HandleRefreshL();
        
        
         void MT_RCustomerServiceProfileCache_CspCPHSValueAddedServicesL();
        
        
         void MT_RCustomerServiceProfileCache_CspTeleServicesL();
        
        
         void MT_RCustomerServiceProfileCache_ChangedCspTableL();
        
        
         void MT_RSSSettings_RSSSettingsL();
        
        
         void MT_RSSSettings_OpenCloseL();
         
        
         void MT_RSSSettings_Open_1L();
        
        
         void MT_RSSSettings_RegisterAllL();
        
        
         void MT_RSSSettings_RegisterL();
        
        
         void MT_RSSSettings_CancelAllL();
        
        
         void MT_RSSSettings_CancelL();
        
        
         void MT_RSSSettings_GetL();
        
        
         void MT_RSSSettings_SetL();
        
        
         void MT_RSSSettings_HandleSIMChangedL();
        
        
         void MT_RSSSettings_PreviousCugValueL();
        
        
         void MT_RSSSettings_ResetPreviousCugValueL();
        
        
         void MT_RSSSettings_HandleRefreshL();
        
        
         void MT_RSSSettings_IsValueValidCugIndexL();
         
         static TInt DoCallBackL( TAny* aAny );
         
         void WaitTimer( TInt aWaitTime );
        
         void Stop();

    private:    // Data
            
        //MSSSettingsRefreshHandler* iRefreshHandler;
        
        CActiveSchedulerWait iWaitTimer;

        RSSSettings* iSettings;
        
        TInt iCugValue;
        TInt iClirValue;
        TInt iAlsValue;
        TInt iAlsBlockValue;
        TInt iCugDefValue;
        
        RCustomerServiceProfileCache* iCSP;
        
        
        
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_SSSETTINGS_H__

// End of file
