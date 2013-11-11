/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/



#ifndef LANDMARKSTESTMODULE_H
#define LANDMARKSTESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


#include "FT_CLandmarkTestProcedureBase.h"

// Logging path
_LIT( KLandMarksTestModuleLogPath, "\\logs\\testframework\\LandMarksTestModule\\" ); 
// Log file
_LIT( KLandMarksTestModuleLogFile, "LandMarksTestModule.txt" ); 


class CLandMarksTestModule;


// CLASS DECLARATION

/**
*  CLandMarksTestModule test class for STIF Test Framework TestScripter.
*/
NONSHARABLE_CLASS(CLandMarksTestModule) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CLandMarksTestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CLandMarksTestModule();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
        CLandMarksTestModule( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */
        void RunTestProcedureL( 
            CLandmarkTestProcedureBase* tp,
            CStifItemParser* aItem = NULL );

        template <class T>
        TInt TestProcedure( TInt aId, CStifItemParser& );

        TInt FT_CPosTp1( CStifItemParser& aItem );
        TInt FT_CPosTp2( CStifItemParser& aItem );
        TInt FT_CPosTp3( CStifItemParser& aItem );
        TInt FT_CPosTp4( CStifItemParser& aItem );
        TInt FT_CPosTp5( CStifItemParser& aItem );
        TInt FT_CPosTp6( CStifItemParser& aItem );
	    TInt FT_CPosTp7( CStifItemParser& aItem );
	    TInt FT_CPosTp8( CStifItemParser& aItem );
	    TInt FT_CPosTp9( CStifItemParser& aItem );

        TInt FT_CPosTp10( CStifItemParser& aItem );
	   	TInt FT_CPosTp11( CStifItemParser& aItem );
        TInt FT_CPosTp12( CStifItemParser& aItem );
	   	TInt FT_CPosTp13( CStifItemParser& aItem );
//        TInt FT_CPosTp15( CStifItemParser& aItem );
//        TInt FT_CPosTp16( CStifItemParser& aItem );
//        TInt FT_CPosTp17( CStifItemParser& aItem );
	   	TInt FT_CPosTp18( CStifItemParser& aItem );
	   	TInt FT_CPosTp19( CStifItemParser& aItem );

        TInt FT_CPosTp20( CStifItemParser& aItem );
	   	TInt FT_CPosTp21( CStifItemParser& aItem );
	   	TInt FT_CPosTp23( CStifItemParser& aItem );
	   	TInt FT_CPosTp24( CStifItemParser& aItem );
	   	TInt FT_CPosTp25( CStifItemParser& aItem );
		TInt FT_CPosTp26( CStifItemParser& aItem );
//        TInt FT_CPosTp27( CStifItemParser& aItem );
		TInt FT_CPosTp28( CStifItemParser& aItem );
//        TInt FT_CPosTp29( CStifItemParser& aItem );

//        TInt FT_CPosTp30( CStifItemParser& aItem );
//        TInt FT_CPosTp31( CStifItemParser& aItem );
//        TInt FT_CPosTp33( CStifItemParser& aItem );
//        TInt FT_CPosTp34( CStifItemParser& aItem );
		TInt FT_CPosTp35( CStifItemParser& aItem );
		TInt FT_CPosTp36( CStifItemParser& aItem );
        TInt FT_CPosTp37( CStifItemParser& aItem );
		TInt FT_CPosTp38( CStifItemParser& aItem );
//        TInt FT_CPosTp39( CStifItemParser& aItem );

//        TInt FT_CPosTp40( CStifItemParser& aItem );
//        TInt FT_CPosTp41( CStifItemParser& aItem );
        TInt FT_CPosTp42( CStifItemParser& aItem );
		TInt FT_CPosTp43( CStifItemParser& aItem );
		TInt FT_CPosTp44( CStifItemParser& aItem );
		TInt FT_CPosTp45( CStifItemParser& aItem );
//        TInt FT_CPosTp46( CStifItemParser& aItem );
		TInt FT_CPosTp47( CStifItemParser& aItem );
		TInt FT_CPosTp48( CStifItemParser& aItem );
//        TInt FT_CPosTp49( CStifItemParser& aItem );

//        TInt FT_CPosTp50( CStifItemParser& aItem );
        TInt FT_CPosTp51( CStifItemParser& aItem );
        TInt FT_CPosTp52( CStifItemParser& aItem );
//        TInt FT_CPosTp53( CStifItemParser& aItem );
//        TInt FT_CPosTp54( CStifItemParser& aItem );
//        TInt FT_CPosTp55( CStifItemParser& aItem );
//        TInt FT_CPosTp56( CStifItemParser& aItem );
//        TInt FT_CPosTp57( CStifItemParser& aItem );
//        TInt FT_CPosTp58( CStifItemParser& aItem );

//        TInt FT_CPosTp76( CStifItemParser& aItem );
//        TInt FT_CPosTp77( CStifItemParser& aItem );
//        TInt FT_CPosTp78( CStifItemParser& aItem );
//        TInt FT_CPosTp79( CStifItemParser& aItem );

//        TInt FT_CPosTp80( CStifItemParser& aItem );
//        TInt FT_CPosTp81( CStifItemParser& aItem );

        TInt FT_CPosTp100( CStifItemParser& aItem );
        TInt FT_CPosTp101( CStifItemParser& aItem );
		TInt FT_CPosTp102( CStifItemParser& aItem );
		TInt FT_CPosTp103( CStifItemParser& aItem );
		TInt FT_CPosTp104( CStifItemParser& aItem );
//        TInt FT_CPosTp105( CStifItemParser& aItem );
        TInt FT_CPosTp106( CStifItemParser& aItem );
//        TInt FT_CPosTp107( CStifItemParser& aItem );
//        TInt FT_CPosTp108( CStifItemParser& aItem );
//        TInt FT_CPosTp109( CStifItemParser& aItem );

//        TInt FT_CPosTp111( CStifItemParser& aItem );
//        TInt FT_CPosTp114( CStifItemParser& aItem );
		TInt FT_CPosTp116( CStifItemParser& aItem );
        TInt FT_CPosTp117( CStifItemParser& aItem );
        TInt FT_CPosTp118( CStifItemParser& aItem );
        TInt FT_CPosTp119( CStifItemParser& aItem );

        TInt FT_CPosTp120( CStifItemParser& aItem );
	    TInt FT_CPosTp121( CStifItemParser& aItem );
		TInt FT_CPosTp122( CStifItemParser& aItem );
		TInt FT_CPosTp123( CStifItemParser& aItem );
		TInt FT_CPosTp124( CStifItemParser& aItem );
		TInt FT_CPosTp125( CStifItemParser& aItem );
        TInt FT_CPosTp126( CStifItemParser& aItem );
//        TInt FT_CPosTp127( CStifItemParser& aItem );
		
        TInt FT_CPosTp130( CStifItemParser& aItem );
        TInt FT_CPosTp131( CStifItemParser& aItem );
        TInt FT_CPosTp132( CStifItemParser& aItem );
        TInt FT_CPosTp134( CStifItemParser& aItem );
        TInt FT_CPosTp133( CStifItemParser& aItem );
        TInt FT_CPosTp135( CStifItemParser& aItem );
        TInt FT_CPosTp136( CStifItemParser& aItem );
        TInt FT_CPosTp137( CStifItemParser& aItem );
        TInt FT_CPosTp139( CStifItemParser& aItem );

        TInt FT_CPosTp141( CStifItemParser& aItem );
        TInt FT_CPosTp143( CStifItemParser& aItem );
        TInt FT_CPosTp144( CStifItemParser& aItem );
        TInt FT_CPosTp145( CStifItemParser& aItem );
        TInt FT_CPosTp146( CStifItemParser& aItem );
        TInt FT_CPosTp147( CStifItemParser& aItem );
        
       	TInt FT_CPosTp148( CStifItemParser& aItem );
       	TInt FT_CPosTp149( CStifItemParser& aItem );
       	TInt FT_CPosTp150( CStifItemParser& aItem );
       	TInt FT_CPosTp151( CStifItemParser& aItem );
       	TInt FT_CPosTp152( CStifItemParser& aItem );
       	TInt FT_CPosTp153( CStifItemParser& aItem );
       	TInt FT_CPosTp154( CStifItemParser& aItem );
       	TInt FT_CPosTp155( CStifItemParser& aItem );
       	TInt FT_CPosTp156( CStifItemParser& aItem );
       	TInt FT_CPosTp157( CStifItemParser& aItem );
       	TInt FT_CPosTp158( CStifItemParser& aItem );
       	TInt FT_CPosTp159( CStifItemParser& aItem );
       	TInt FT_CPosTp160( CStifItemParser& aItem );
      	TInt FT_CPosTp161( CStifItemParser& aItem );
      	TInt FT_CPosTp162( CStifItemParser& aItem );
      	TInt FT_CPosTp163( CStifItemParser& aItem );
      	TInt FT_CPosTp164( CStifItemParser& aItem );
    };

#endif      // LANDMARKSTESTMODULE_H

// End of File
