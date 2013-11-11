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
* Description:  CTestPosLmDisplayItem class
*
*/



#ifndef C_TESTPOSLMDISPLAYITEM_H
#define C_TESTPOSLMDISPLAYITEM_H


#include <e32base.h>


/**
 *  This class will test methods of CPosLmDisplayItem class
 *  @p CTestPosLmDisplayItem clss contians functions to test methods of
 *  CPosLmDisplayItem class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */

class CTestPosLmDisplayItem : public CBase
    {

public:
     /**
     * Two-phased constructor.
     */
     static CTestPosLmDisplayItem* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmDisplayItem();
    
    /**
     * This function is used to check NewL function of CPosLmDisplayItem class.
     * This is tested by passing CPosLandmark pointer to the function. 
     */
     void TestNewL1L(TInt);

    /**
     * This function is used to check NewL function of CPosLmDisplayItem class.
     * This is tested by passing CPosLandmarkCategory pointer to the function. 
     */
     void TestNewL2L(TInt);
     
     /**
     * This function is used to check DisplayItemType function of CPosLmDisplayItem class.
     */
     void TestDisplayItemTypeL(TInt);
     
     /**
     * This function is used to check DatabaseIndex function of CPosLmDisplayItem class.
     */
     void TestDatabaseIndexL(TInt);
     
     /**
     * This function is used to check Category function of CPosLmDisplayItem class.
     */
     void TestCategoryL(TInt);
     
     /**
     * This function is used to check Landmark function of CPosLmDisplayItem class.
     */
     void TestLandmarkL(TInt);
     
     /**
     * This function is used to check GetDistance function of CPosLmDisplayItem class.
     */
     void TestGetDistanceL();

private:
	
	/**
    * Constructor.
    */
    CTestPosLmDisplayItem();


    };



#endif // C_TESTPOSLMDISPLAYITEM_H
