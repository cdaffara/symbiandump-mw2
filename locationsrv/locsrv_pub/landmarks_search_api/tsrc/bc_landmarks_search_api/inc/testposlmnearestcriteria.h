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
* Description:     CTestPosLmNearestCriteria class
*
*/





#ifndef C_TESTPOSLMNEARESTCRITERIA_H
#define C_TESTPOSLMNEARESTCRITERIA_H

#include <e32base.h>

/**
 *  This class will test methods of CPosLmNearestCriteria class
 *  @p CTestPosLmNearestCriteria clss contians functions to test methods of
 *  CPosLmNearestCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */

class CTestPosLmNearestCriteria : public CBase
    {

public:

	/**
     * Two-phased constructor.
     */
     static CTestPosLmNearestCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmNearestCriteria();
    
    /**
     * This function is used to check NewLC function of CPosLmNearestCriteria class.
     *
     */
    void TestNewLC();
    
    /**
     * This function is used to check GetCoordinate function of CPosLmNearestCriteria class.
     *
     */
     void TestGetCoordinateL();
	
	/**
     * This function is used to check SetCoordinate function of CPosLmNearestCriteria class.
     *
     */
	 void TestSetCoordinateL();
	 
	 /**
     * This function is used to check UseCoverageRadius function of CPosLmNearestCriteria class.
     *
     */
	 void TestUseCoverageRadiusL(TInt);
	 
	 /**
     * This function is used to check SetUseCoverageRadius function of CPosLmNearestCriteria class.
     *
     */
	 void TestSetUseCoverageRadiusL(TInt);
	 
	 /**
     * This function is used to check MaxDistance function of CPosLmNearestCriteria class.
     *
     */
     void TestMaxDistanceL();
    
     /**
     * This function is used to check SetMaxDistance function of CPosLmNearestCriteria class.
     *
     */
	void TestSetMaxDistanceL();
	
private:
	
	/**
    * Constructor.
    */
    CTestPosLmNearestCriteria();


    };




#endif // C_TESTPOSLMNEARESTCRITERIA_H
