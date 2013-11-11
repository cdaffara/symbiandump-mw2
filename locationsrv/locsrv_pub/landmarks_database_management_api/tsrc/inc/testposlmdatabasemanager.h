/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:    CTestPosLmDatabaseManager class
*
*/



#ifndef  C_POSLMKDATABASEMANAGER_H
#define  C_POSLMKDATABASEMANAGER_H


#include <e32base.h>


/**
 *  This class is used to test methods of CPosLmDatabaseManager class
 *  @p ctlmkdatabasemanager contians functions to test methods of
 *  CPosLmDatabaseManager class
 *
 *  @lib testdbmanagementapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmDatabaseManager: public CActive
    {
    public:
    	/**
    	* Static two phase constructor
    	*/
    	static CTestPosLmDatabaseManager* NewLC();
	
		/**
        * Destructor
        */
        virtual ~CTestPosLmDatabaseManager();        
        
	    /**
	    * This function will test the NewL function of CPosLmDatabaseManager
	    * class.
	    */
	    void TestNewL();
	    
	    /**
	    * This function will test the DefaultDatabaseUriLC function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestDefaultDatabaseUriLC();

	    
	    /**
	    * This function will test the SetDefaultDatabaseUriL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestSetDefaultDatabaseUriL();
	    
	    /**
	    * This function will test the NotifyDatabaseEvent function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestNotifyDatabaseEventL();
	    
	    /**
	    * This function will test the CancelNotifyDatabaseEvent function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestCancelNotifyDatabaseEventL();
	    
	    /**
	    * This function will test the DatabaseUriFromLastEventLC function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestDatabaseUriFromLastEventLC();
	    
	    /**
	    * This function will test the ListDatabasesLC function of 
	    * CPosLmDatabaseManager class.
	    */
		void TestListDatabasesLC();
		
		/**
	    * This function will test the ListDatabasesL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestListDatabasesL();
	    
	    /**
	    * This function will test the RegisterDatabaseL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestRegisterDatabaseL();
	    
	    /**
	    * This function will test the UnregisterDatabaseL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestUnregisterDatabaseL();
	    
	    /**
	    * This function will test the UnregisterAllDatabasesL function of 
	    * CPosLmDatabaseManager class.
	    */
	  	void TestUnregisterAllDatabasesL();
	  	
	  	/**
	    * This function will test the ModifyDatabaseSettingsL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestModifyDatabaseSettingsL();
		
		
		/**
	    * This function will test the GetDatabaseInfoL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestGetDatabaseInfoL();
	    
	    /**
	    * This function will test the DatabaseExistsL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestDatabaseExistsL();
	    
	    /**
	    * This function will test the CreateDatabaseL function of 
	    * CPosLmDatabaseManager class.
	    */
		void TestCreateDatabaseL();
		
		/**
	    * This function will test the DeleteDatabaseL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestDeleteDatabaseL();
	    
	    /**
	    * This function will test the CopyDatabaseL function of 
	    * CPosLmDatabaseManager class.
	    */
	    void TestCopyDatabaseL();
		
	  	    


private:
	/**
    * Constructor
    */
    CTestPosLmDatabaseManager();
	
protected:
	/**
	* RunL implementation of active object,
	*/
	void RunL();
	
	/**
	* DoCancel implementation of active object,
	*/
	void DoCancel();
	


    };


//*** put the inline functions, if any, in a file included here:

//#include "?include_file.inl"


#endif // C_POSLMKDATABASEMANAGER_H
