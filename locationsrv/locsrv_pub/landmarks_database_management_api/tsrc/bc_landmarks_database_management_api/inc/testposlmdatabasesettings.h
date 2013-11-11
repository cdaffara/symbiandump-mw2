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
* Description:   TTestPosLmDataBaseSettings class
*
*/




#ifndef T_TESTPOSLMDATABASESETTINGS_H
#define T_TESTPOSLMDATABASESETTINGS_H


#include <e32std.h>


/**
 *  This class is used to test methods of TPosLmDatabaseSettings class
 *  @p TTestPosLmDataBaseSettings contians functions to test methods of
 *  TPosLmDatabaseSettings class
 *
 *  @lib testdbmanagementapi.lib
 *  @since S60 v3.2
 */
class TTestPosLmDataBaseSettings 
    {

public:

    /**
     * Constructor.
     */
    TTestPosLmDataBaseSettings();
    
    /**
	 * This function will test the constructor of TPosLmDatabaseSettings class.
	 */
     void TestTPosLmDatabaseSettingsL();
     
     /**
	 * This function will test the IsAttributeSet function of TPosLmDatabaseSettings class.
	 */
	 void TestIsAttributeSetL(TInt);
	 
	 /**
	 * This function will test the UnsetAttribute function of TPosLmDatabaseSettings class.
	 */
	 void TestUnsetAttributeL();
	 
	 /**
	 * This function will test the SetDatabaseName function of TPosLmDatabaseSettings class.
	 */
	 void TestSetDatabaseNameL();
	 
	 /**
	 * This function will test the DatabaseName function of TPosLmDatabaseSettings class.
	 */
	 void TestDatabaseNameL();

    };


#endif // T_TESTPOSLMDATABASESETTINGS_H
