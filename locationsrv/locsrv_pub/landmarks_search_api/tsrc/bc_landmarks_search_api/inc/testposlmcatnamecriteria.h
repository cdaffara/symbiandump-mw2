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
* Description:     CTestPosLmCatNameCriteria class
*
*/






#ifndef C_TESTPOSLMCATNAMECRITERIA_H
#define C_TESTPOSLMCATNAMECRITERIA_H

#include <e32base.h>


/**
 *  This class is used to test the functions of CPosLmCatNameCriteria class
 *	@p CTestPosLmCatNameCriteria clss contians functions to test methods of
 *  CPosLmCatNameCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmCatNameCriteria : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
    static CTestPosLmCatNameCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmCatNameCriteria();

   	/**
     * This function is used to check NewLC function of CPosLmCatNameCriteria class.
     *
     */
     void TestNewLC();
    
     /**
     * This function is used to check SearchPattern function of CPosLmCatNameCriteria class.
     *
     */
		
	void TestSearchPatternL();
	
	/**
   * This function is used to check SetSearchPatternL function of CPosLmCatNameCriteria class.
   *
   */
	void TestSetSearchPatternL(TPtrC);
	
private:
	
	/**
   * Constructor.
   */
    CTestPosLmCatNameCriteria();


    };





#endif //  C_TESTPOSLMCATNAMECRITERIA_H
