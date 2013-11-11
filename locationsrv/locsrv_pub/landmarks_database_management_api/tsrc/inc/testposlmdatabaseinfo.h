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
* Description:   CTestPosLmDatabaseInfo class
*
*/




#ifndef C_TESTPOSLMDATABASEINFO_H
#define C_TESTPOSLMDATABASEINFO_H


#include <e32base.h>

/**
 *  This class is used to test methods of HPosLmDatabaseInfo class
 *  @p CTestPosLmDatabaseInfo contians functions to test methods of
 *  HPosLmDatabaseInfo class
 *
 *  @lib testdbmanagementapi.lib
 *  @since S60 v3.2
 */

class CTestPosLmDatabaseInfo : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
     static CTestPosLmDatabaseInfo* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmDatabaseInfo();
    
    /**
	 * This function will test the NewLC function of HPosLmDatabaseInfo class.
	 */
	 void TestNewLC1L();
	 
    /**
	 * This function will test the overloaded NewLC function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestNewLC2L();

  	 /**
	 * This function will test the NewL function of HPosLmDatabaseInfo class.
	 */
	 void TestNewL1L();
	 
	 /**
	 * This function will test the overloaded NewL function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestNewL2L();
	 
	 /**
	 * This function will test the DatabaseUri function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestDatabaseUriL();
	 
	 /**
	 * This function will test the Protocol function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestProtocol1L();
	 
	 /**
	 * This function will test the Protocol function of HPosLmDatabaseInfo 
	 * Protocol function must return default protocol when it is not specified.
	 * class.
	 */
	 void TestProtocol2L();
	 
	 /**
	 * This function will test the IsDefault function of HPosLmDatabaseInfo 
	 * class.
	 */
	  void TestIsDefaultL();
	  
	 /**
	 * This function will test the DatabaseMedia function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestDatabaseMediaL();

	 /**
	 * This function will test the DatabaseMedia function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestDatabaseDriveL();
	 
	 /**
	 * This function will test the Settings function of HPosLmDatabaseInfo 
	 * class which returns constant reference.
	 */
	 void TestSettings1L();
	 
	 /**
	 * This function will test the Settings function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestSettings2L();

	 
	 /**
	 * This function will test the Size function of HPosLmDatabaseInfo 
	 * class.
	 */
	 void TestSizeL();

private:


    /**
    * Constructor.
    */
    CTestPosLmDatabaseInfo();


    };




#endif // C_TESTPOSLMDATABASEINFO_H
