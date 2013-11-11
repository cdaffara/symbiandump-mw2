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
* Description:  CTestPosLmTextCriteria class
*
*/



#ifndef C_TESTPOSLMTEXTCRITERIA_H
#define C_TESTPOSLMTEXTCRITERIA_H


#include <e32base.h>

 
 /**
 *  This class will test methods of CPosLmTextCriteria class
 *  @p CTestPosLmTextCriteria clss contians functions to test methods of
 *  CPosLmTextCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */

class CTestPosLmTextCriteria : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
     static CTestPosLmTextCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmTextCriteria();

    /**
     * This function is used to check NewLC function of CPosLmTextCriteria class
     *
     */
     void TestNewLC();
     
     /**
     * This function is used to check Text function of CPosLmTextCriteria class
     *
     */
     void TestTextL();
     
     /**
     * This function is used to check SetTextL function of CPosLmTextCriteria class
     *
     */
     void TestSetTextL();
     
     /**
     * This function is used to check AttributesToSearc function of CPosLmTextCriteria class
     *
     */
	void TestAttributesToSearchL();
	
	/**
     * This function is used to check SetAttributesToSearch function of CPosLmTextCriteria class
     *
     */
     void TestSetAttributesToSearchL();
     
     /**
     * This function is used to check GetPositionFieldsToSearchL function of CPosLmTextCriteria class
     *
     */
     void TestGetPositionFieldsToSearchL();
     
     /**
     * This function is used to check SetPositionFieldsToSearchL function of CPosLmTextCriteria class
     *
     */
     void TestSetPositionFieldsToSearchL();
     
     /**
     * This function is used to check ClearPositionFieldsToSearch function of CPosLmTextCriteria class
     *
     */
     void TestClearPositionFieldsToSearchL();
private:
	
	/**
    * Constructor.
    */
    CTestPosLmTextCriteria();
    
    };




#endif // C_TESTPOSLMTEXTCRITERIA_H
