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
* Description:  CTestPosLmCategoryCriteria class
*
*/


#ifndef C_TESTPOSLMCATEGORYCRITERIA_H
#define C_TESTPOSLMCATEGORYCRITERIA_H


#include <e32base.h>


/**
 *  This class is used to test the functions of CPosLmCategoryCriteria class
 *	@p CTestPosLmCategoryCriteria clss contians functions to test methods of
 *  CPosLmCategoryCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmCategoryCriteria : public CBase
    {

 public:
 
    /**
     * Two-phased constructor.
     */
    static CTestPosLmCategoryCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmCategoryCriteria();
	
	/**
     * This function is used to check NewLC function of CPosLmCategoryCriteria class.
     *
     */
    void TestNewLC();
    
    /**
     * This function is used to check CategoryItemId function of CPosLmCategoryCriteria class.
     *
     */
    void TestCategoryItemIdL();
    
    /**
     * This function is used to check GlobalCategory function of CPosLmCategoryCriteria class.
     *
     */
    void TestGlobalCategoryL();
    
    /**
     * This function is used to check CategoryName function of CPosLmCategoryCriteria class.
     *
     */
    void TestCategoryNameL();
    
    /**
     * This function is used to check SetCategoryItemId function of CPosLmCategoryCriteria class.
     *
     */
    void TestSetCategoryItemIdL();
    
    /**
     * This function is used to check SetCategoryItemId function of CPosLmCategoryCriteria class.
     *
		 */
    void TestSetGlobalCategoryL();
    /**
     * This function is used to check SetCategoryNameL function of CPosLmCategoryCriteria class.
     *
		 */
    void TestSetCategoryNameL(TPtrC);

protected:



private:
	
	/**
   * constructor.
   */

    CTestPosLmCategoryCriteria();

    

    };


#endif // C_TESTPOSLMCATEGORYCRITERIA_H
