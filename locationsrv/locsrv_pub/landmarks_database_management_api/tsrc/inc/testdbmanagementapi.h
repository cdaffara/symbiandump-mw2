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
* Description:   Ctestdbmanagementapi class
*
*/




#ifndef TESTDBMANAGEMENTAPI_H
#define TESTDBMANAGEMENTAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// Logging path
_LIT( KtestdbmanagementapiLogPath, "\\logs\\testframework\\testdbmanagementapi\\" ); 
// Log file
_LIT( KtestdbmanagementapiLogFile, "testdbmanagementapi.txt" ); 


class Ctestdbmanagementapi;

/**
*  Ctestdbmanagementapi test class for STIF Test Framework TestScripter.
*  @lib testdbmanagementapi.lib
*  @since S60 v3.2
*/
NONSHARABLE_CLASS(Ctestdbmanagementapi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestdbmanagementapi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestdbmanagementapi();

  
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

   
   private:

        /**
        * C++ default constructor.
        */
        Ctestdbmanagementapi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        
        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * This is the test case for NewL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
         TInt CPosLmDatabaseManager_NewL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for DefaultDatabaseUriLC function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_DefaultDatabaseUriLC(CStifItemParser& /*aItem*/);
      
 	    /**
        * This is the test case for SetDefaultDatabaseUriL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
 		TInt CPosLmDatabaseManager_SetDefaultDatabaseUriL(CStifItemParser& /*aItem*/);
 		
 		/**
        * This is the test case for NotifyDatabaseEvent function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
 		TInt CPosLmDatabaseManager_NotifyDatabaseEventL(CStifItemParser& /*aItem*/);

 		/**
        * This is the test case for CancelNotifyDatabaseEvent function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
 		TInt CPosLmDatabaseManager_CancelNotifyDatabaseEventL(CStifItemParser& /*aItem*/);
	
		/**
        * This is the test case for DatabaseUriFromLastEventLC function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
 		TInt CPosLmDatabaseManager_DatabaseUriFromLastEventLC(CStifItemParser& /*aItem*/);
 		
 		/**
        * This is the test case for ListDatabasesLC function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_ListDatabasesLC(CStifItemParser& /*aItem*/);
        
 		/**
        * This is the test case for ListDatabasesLC function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_ListDatabasesL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for RegisterDatabaseL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_RegisterDatabaseL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for UnregisterDatabaseL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_UnregisterDatabaseL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for UnregisterAllDatabasesL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_UnregisterAllDatabasesL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for ModifyDatabaseSettingsL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_ModifyDatabaseSettingsL(CStifItemParser& /*aItem*/);
        
        
        /**
        * This is the test case for GetDatabaseInfoL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_GetDatabaseInfoL(CStifItemParser& /*aItem*/);

        /**
        * This is the test case for DatabaseExistsL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_DatabaseExistsL(CStifItemParser& /*aItem*/);

		/**
        * This is the test case for CreateDatabaseL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_CreateDatabaseL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for DeleteDatabaseL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_DeleteDatabaseL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for CopyDatabaseL function ofCPosLmDatabaseManager class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDatabaseManager_CopyDatabaseL(CStifItemParser& /*aItem*/);

		/**
        * This is the test case for NewLC function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_NewLC1L(CStifItemParser& /*aItem*/);  
      
        /**
        * This is the test case for overloaded NewLC function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_NewLC2L(CStifItemParser& /*aItem*/);  
		
		/**
        * This is the test case for NewL function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_NewL1L(CStifItemParser& /*aItem*/);  
 
 		/**
        * This is the test case for overloaded NewL function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_NewL2L(CStifItemParser& /*aItem*/);  
 
      	/**
        * This is the test case for DatabaseUri function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_DatabaseUriL(CStifItemParser& /*aItem*/);
		
		/**
        * This is the test case for Protocol function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_Protocol1L(CStifItemParser& /*aItem*/);  
        
        /**
        * This is the test case for Protocol function of HPosLmDatabaseInfo class.
        * Protocol function must return default protocol when it is not specified.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_Protocol2L(CStifItemParser& /*aItem*/);       
     
        /**
        * This is the test case for IsDefault function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_IsDefaultL(CStifItemParser& /*aItem*/);  
  		
  		/**
        * This is the test case for DatabaseMedia function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_DatabaseMediaL(CStifItemParser& /*aItem*/); 
        
        /**
        * This is the test case for DatabaseDrive function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_DatabaseDriveL(CStifItemParser& /*aItem*/); 
        
        /**
        * This is the test case for Settings function of HPosLmDatabaseInfo class.
        * Setting function here will return constant object.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_Settings1L(CStifItemParser& /*aItem*/);
        
         /**
        * This is the test case for Settings function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_Settings2L(CStifItemParser& /*aItem*/);
    
        /**
        * This is the test case for Size function of HPosLmDatabaseInfo class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt HPosLmDatabaseInfo_SizeL(CStifItemParser& /*aItem*/);
        
        /**
        * This is the test case for constructor of TPosLmDatabaseSettings class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt TPosLmDatabaseSettings_TPosLmDatabaseSettingsL(CStifItemParser& /*aItem*/); 
      	
      	/**
        * This is the test case for IsAttributeSet function of TPosLmDatabaseSettings class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt TPosLmDatabaseSettings_IsAttributeSetL(CStifItemParser& /*aItem*/); 
		
		/**
        * This is the test case for UnsetAttribute function of TPosLmDatabaseSettings class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt TPosLmDatabaseSettings_UnsetAttributeL(CStifItemParser& /*aItem*/); 
    	
    	/**
        * This is the test case for SetDatabaseName function of TPosLmDatabaseSettings class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt TPosLmDatabaseSettings_SetDatabaseNameL(CStifItemParser& /*aItem*/); 
        
        	/**
        * This is the test case for DatabaseName function of TPosLmDatabaseSettings class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt TPosLmDatabaseSettings_DatabaseNameL(CStifItemParser& /*aItem*/); 
        
        
   
       };

#endif      // TESTDBMANAGEMENTAPI_H

// End of File
