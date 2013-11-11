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
* Description:  CTestPosLmCompositeCriteria.cpp
*
*/



#ifndef C_TESTPOSLMCOMPOSITECRITERIA_H
#define C_TESTPOSLMCOMPOSITECRITERIA_H


#include <e32base.h>


/**
 *  This class will test methods of CPosLmCompositeCriteria class
 *  @p CTestPosLmCompositeCriteria clss contians functions to test methods of
 *  CPosLmCompositeCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmCompositeCriteria : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
     static CTestPosLmCompositeCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmCompositeCriteria();
    
    /**
    * This function is used to check NewLC function of CPosLmCompositeCriteria class with ECopmositionAND composition type.
    *
    */
    void TestNewLC_AND();
    /**
    * This function is used to check NewLC function of CPosLmCompositeCriteria class with ECompositionOR composition type.
    *
    */
    void TestNewLC_OR();

     /**
     * This function is used to check CompositionType function of CPosLmCompositeCriteria class.
     *
     */
     void TestCompositionTypeL();
     
     /**
     * This function is used to check SetCompositionType function of CPosLmCompositeCriteria class.
     *
     */
     void TestSetCompositionTypeL();
	
	 /**
     * This function is used to check NumOfArguments function of CPosLmCompositeCriteria class.
     *
     */
     void TestNumOfArgumentsL();
     
     /**
     * This function is used to check Argument function of CPosLmCompositeCriteria class
     * which return a constant pointer. 
     *
     */
	 void TestArgument1L();
	
	 /**
     * This function is used to check Argument function of CPosLmCompositeCriteria class
     * which return pointer which is not constant. 
     *
     */
	 void TestArgument2L();
	
	 /**
     * This function is used to check AddArgument function of CPosLmCompositeCriteria class. 
     *
     */
	 void TestAddArgumentL();
	 
	 /**
     * This function is used to check RemoveArgument function of CPosLmCompositeCriteria class. 
     *
     */
	 void TestRemoveArgumentL();
	 
	 /**
     * This function is used to check ClearArguments function of CPosLmCompositeCriteria class. 
     *
     */
	 void TestClearArgumentsL();
private:

	/**
    * Constructor.
    */
    CTestPosLmCompositeCriteria();
	

    };



#endif // C_TESTPOSLMCOMPOSITECRITERIA_H
