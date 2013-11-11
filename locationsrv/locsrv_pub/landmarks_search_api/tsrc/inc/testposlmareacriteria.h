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
* Description:  CTestPosLmAreaCriteria class
*
*/



#ifndef C_TESTPOSLMAREACRITERIA_H
#define C_TESTPOSLMAREACRITERIA_H

#include <e32base.h>

/**
 *  This class is used to test functions of CPosLmAreaCriteria class
 *
 *  @p  CTestPosLmAreaCriteria clss contians functions to test methods of
 *  CPosLmAreaCriteria class
 *
 *  @lib testlmksearchapi.lib
 *  @since S60 v3.2
 */
class CTestPosLmAreaCriteria : public CBase
    {

public:

    /**
     * Two-phased constructor.
     * 
     */
     static CTestPosLmAreaCriteria* NewLC();
    

    /**
    * Destructor.
    */
    virtual ~CTestPosLmAreaCriteria();
    
    /**
     * This class will test the NewLC function of CPosLmAreaCriteria class
     * @param[in] aSouthLat The southern latitude border of the search area.
     * @param[in] aNorthLat The northern latitude border of the search area.
     * @param[in] aWestLong The western longitude border of the search area.
     * @param[in] aEastLong The eastern longitude border of the search area
     * 
     */
    void TestNewLC(TReal64 aSouthLat,TReal64 aNorthLat,TReal64 aWestLong,TReal64 aEastLong);
	
	/**
     * This class will test the GetSearchArea function of CPosLmAreaCriteria class
	 */	
	void TestGetSearchAreaL();
	
	/**
     * This class will test the SetSearchArea function of CPosLmAreaCriteria class
     * @param[in] aSouthLat The southern latitude border of the search area.
     * @param[in] aNorthLat The northern latitude border of the search area.
     * @param[in] aWestLong The western longitude border of the search area.
     * @param[in] aEastLong The eastern longitude border of the search area
     * 
     */

    void TestSetSearchAreaL(TReal64 aSouthLat,TReal64 aNorthLat,TReal64 aWestLong,TReal64 aEastLong);
protected:


private:

	/**
    * Constructor.
    */
    CTestPosLmAreaCriteria();
    

    };



#endif // C_TESTPOSLMAREACRITERIA_H
