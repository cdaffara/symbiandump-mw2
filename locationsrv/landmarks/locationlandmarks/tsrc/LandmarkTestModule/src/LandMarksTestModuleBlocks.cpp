/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "LandMarksTestModule.h"

#include "FT_CPosTp1.h"
#include "FT_CPosTp2.h"
#include "FT_CPosTp3.h"
#include "FT_CPosTp4.h"
#include "FT_CPosTp5.h"
#include "FT_CPosTp6.h"
#include "FT_CPosTp7.h"
#include "FT_CPosTp8.h"
#include "FT_CPosTp9.h"

#include "FT_CPosTp10.h"
#include "FT_CPosTp11.h"
#include "FT_CPosTp12.h"
#include "FT_CPosTp13.h"
#include "FT_CPosTp18.h"
#include "FT_CPosTp19.h"

#include "FT_CPosTp20.h"
#include "FT_CPosTp21.h"
#include "FT_CPosTp23.h"
#include "FT_CPosTp24.h"
#include "FT_CPosTp25.h"
#include "FT_CPosTp26.h"
#include "FT_CPosTp28.h"

#include "FT_CPosTp35.h"
#include "FT_CPosTp36.h"
#include "FT_CPosTp37.h"
#include "FT_CPosTp38.h"

#include "FT_CPosTp42.h"
#include "FT_CPosTp43.h"
#include "FT_CPosTp44.h"
#include "FT_CPosTp45.h"
#include "FT_CPosTp47.h"
#include "FT_CPosTp48.h"

#include "FT_CPosTp51.h"
#include "FT_CPosTp52.h"

#include "FT_CPosTp100.h"
#include "FT_CPosTp101.h"
#include "FT_CPosTp102.h"
#include "FT_CPosTp103.h"
#include "FT_CPosTp104.h"
#include "FT_CPosTp106.h"

#include "FT_CPosTp116.h"
#include "FT_CPosTp117.h"
#include "FT_CPosTp118.h"
#include "FT_CPosTp119.h"

#include "FT_CPosTp120.h"
#include "FT_CPosTp121.h"
#include "FT_CPosTp122.h"
#include "FT_CPosTp123.h"
#include "FT_CPosTp124.h"
#include "FT_CPosTp125.h"
#include "FT_CPosTp126.h"

#include "FT_CPosTp130.h"
#include "FT_CPosTp131.h"
#include "FT_CPosTp132.h"
#include "FT_CPosTp134.h"
#include "FT_CPosTp133.h"
#include "FT_CPosTp135.h"
#include "FT_CPosTp136.h"
#include "FT_CPosTp137.h"
#include "FT_CPosTp139.h"

#include "FT_CPosTp141.h"
#include "FT_CPosTp143.h"

#include "FT_CPosTp144.h"
#include "FT_CPosTp145.h"
#include "FT_CPosTp146.h"
#include "FT_CPosTp147.h"

#include "FT_CPosTp148.h"
#include "FT_CPosTp149.h"
#include "FT_CPosTp150.h"
#include "FT_CPosTp151.h"
#include "FT_CPosTp152.h"
#include "FT_CPosTp153.h"
#include "FT_CPosTp154.h"
#include "FT_CPosTp155.h"
#include "FT_CPosTp156.h"
#include "FT_CPosTp157.h"
#include "FT_CPosTp158.h"
#include "FT_CPosTp159.h"
#include "FT_CPosTp160.h"

#include "FT_CPosTp161.h"
#include "FT_CPosTp162.h"
#include "FT_CPosTp163.h"
#include "FT_CPosTp164.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLandMarksTestModule::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -----------------------------------------------------------------------------
//
void CLandMarksTestModule::Delete()
    {
    }

// -----------------------------------------------------------------------------
// CLandMarksTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CLandMarksTestModule::RunMethodL(CStifItemParser& aItem )
    {
    	static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.

        ENTRY( "FT_CPosTp1", CLandMarksTestModule::FT_CPosTp1 ),
        ENTRY( "FT_CPosTp2", CLandMarksTestModule::FT_CPosTp2 ),
        ENTRY( "FT_CPosTp3", CLandMarksTestModule::FT_CPosTp3 ),
        ENTRY( "FT_CPosTp4", CLandMarksTestModule::FT_CPosTp4 ),
        ENTRY( "FT_CPosTp5", CLandMarksTestModule::FT_CPosTp5 ),
        ENTRY( "FT_CPosTp6", CLandMarksTestModule::FT_CPosTp6 ),
        ENTRY( "FT_CPosTp7", CLandMarksTestModule::FT_CPosTp7 ),
        ENTRY( "FT_CPosTp8", CLandMarksTestModule::FT_CPosTp8 ),
        ENTRY( "FT_CPosTp9", CLandMarksTestModule::FT_CPosTp9 ),
        
        ENTRY( "FT_CPosTp10", CLandMarksTestModule::FT_CPosTp10 ),
        ENTRY( "FT_CPosTp11", CLandMarksTestModule::FT_CPosTp11 ),
        ENTRY( "FT_CPosTp12", CLandMarksTestModule::FT_CPosTp12 ),
        ENTRY( "FT_CPosTp13", CLandMarksTestModule::FT_CPosTp13 ),
        ENTRY( "FT_CPosTp18", CLandMarksTestModule::FT_CPosTp18 ),
        ENTRY( "FT_CPosTp19", CLandMarksTestModule::FT_CPosTp19 ),
        
        ENTRY( "FT_CPosTp20", CLandMarksTestModule::FT_CPosTp20 ),
        ENTRY( "FT_CPosTp21", CLandMarksTestModule::FT_CPosTp21 ),
        ENTRY( "FT_CPosTp23", CLandMarksTestModule::FT_CPosTp23 ),
        ENTRY( "FT_CPosTp24", CLandMarksTestModule::FT_CPosTp24 ),
        ENTRY( "FT_CPosTp25", CLandMarksTestModule::FT_CPosTp25 ),
        ENTRY( "FT_CPosTp26", CLandMarksTestModule::FT_CPosTp26 ),
        ENTRY( "FT_CPosTp28", CLandMarksTestModule::FT_CPosTp28 ),
        
        ENTRY( "FT_CPosTp35", CLandMarksTestModule::FT_CPosTp35 ),
        ENTRY( "FT_CPosTp36", CLandMarksTestModule::FT_CPosTp36 ),
        ENTRY( "FT_CPosTp37", CLandMarksTestModule::FT_CPosTp37 ),
        ENTRY( "FT_CPosTp38", CLandMarksTestModule::FT_CPosTp38 ),
        
        ENTRY( "FT_CPosTp42", CLandMarksTestModule::FT_CPosTp42 ),
        ENTRY( "FT_CPosTp43", CLandMarksTestModule::FT_CPosTp43 ),
        ENTRY( "FT_CPosTp44", CLandMarksTestModule::FT_CPosTp44 ),
        ENTRY( "FT_CPosTp45", CLandMarksTestModule::FT_CPosTp45 ),
        ENTRY( "FT_CPosTp47", CLandMarksTestModule::FT_CPosTp47 ),
        ENTRY( "FT_CPosTp48", CLandMarksTestModule::FT_CPosTp48 ),
        
        ENTRY( "FT_CPosTp51", CLandMarksTestModule::FT_CPosTp51 ),
        ENTRY( "FT_CPosTp52", CLandMarksTestModule::FT_CPosTp52 ),
        
        ENTRY( "FT_CPosTp100", CLandMarksTestModule::FT_CPosTp100 ),
        ENTRY( "FT_CPosTp101", CLandMarksTestModule::FT_CPosTp101 ),
        ENTRY( "FT_CPosTp102", CLandMarksTestModule::FT_CPosTp102 ),
        ENTRY( "FT_CPosTp103", CLandMarksTestModule::FT_CPosTp103 ),
        ENTRY( "FT_CPosTp104", CLandMarksTestModule::FT_CPosTp104 ),
        ENTRY( "FT_CPosTp106", CLandMarksTestModule::FT_CPosTp106 ),
        
        ENTRY( "FT_CPosTp116", CLandMarksTestModule::FT_CPosTp116 ),
        ENTRY( "FT_CPosTp117", CLandMarksTestModule::FT_CPosTp117 ),
        ENTRY( "FT_CPosTp118", CLandMarksTestModule::FT_CPosTp118 ),
        ENTRY( "FT_CPosTp119", CLandMarksTestModule::FT_CPosTp119 ),
        
        ENTRY( "FT_CPosTp120", CLandMarksTestModule::FT_CPosTp120 ),
        ENTRY( "FT_CPosTp121", CLandMarksTestModule::FT_CPosTp121 ),
        ENTRY( "FT_CPosTp122", CLandMarksTestModule::FT_CPosTp122 ),
        ENTRY( "FT_CPosTp123", CLandMarksTestModule::FT_CPosTp123 ),
        ENTRY( "FT_CPosTp124", CLandMarksTestModule::FT_CPosTp124 ),
        ENTRY( "FT_CPosTp125", CLandMarksTestModule::FT_CPosTp125 ),
        ENTRY( "FT_CPosTp126", CLandMarksTestModule::FT_CPosTp126 ),
        
        ENTRY( "FT_CPosTp130", CLandMarksTestModule::FT_CPosTp130 ),
        ENTRY( "FT_CPosTp131", CLandMarksTestModule::FT_CPosTp131 ),
        ENTRY( "FT_CPosTp132", CLandMarksTestModule::FT_CPosTp132 ),
        ENTRY( "FT_CPosTp133", CLandMarksTestModule::FT_CPosTp133 ),
        ENTRY( "FT_CPosTp134", CLandMarksTestModule::FT_CPosTp134 ),
        ENTRY( "FT_CPosTp135", CLandMarksTestModule::FT_CPosTp135 ),
        ENTRY( "FT_CPosTp136", CLandMarksTestModule::FT_CPosTp136 ),
        ENTRY( "FT_CPosTp137", CLandMarksTestModule::FT_CPosTp137 ),
        ENTRY( "FT_CPosTp139", CLandMarksTestModule::FT_CPosTp139 ),
        
        ENTRY( "FT_CPosTp141", CLandMarksTestModule::FT_CPosTp141 ),
        ENTRY( "FT_CPosTp143", CLandMarksTestModule::FT_CPosTp143 ),
        ENTRY( "FT_CPosTp144", CLandMarksTestModule::FT_CPosTp144 ),
        ENTRY( "FT_CPosTp145", CLandMarksTestModule::FT_CPosTp145 ),
        ENTRY( "FT_CPosTp146", CLandMarksTestModule::FT_CPosTp146 ),
        ENTRY( "FT_CPosTp147", CLandMarksTestModule::FT_CPosTp147 ),
        
        ENTRY( "FT_CPosTp148", CLandMarksTestModule::FT_CPosTp148 ),
		ENTRY( "FT_CPosTp149", CLandMarksTestModule::FT_CPosTp149 ),
		ENTRY( "FT_CPosTp150", CLandMarksTestModule::FT_CPosTp150 ),
		ENTRY( "FT_CPosTp151", CLandMarksTestModule::FT_CPosTp151 ),
		ENTRY( "FT_CPosTp152", CLandMarksTestModule::FT_CPosTp152 ),
		ENTRY( "FT_CPosTp153", CLandMarksTestModule::FT_CPosTp153 ),
		ENTRY( "FT_CPosTp154", CLandMarksTestModule::FT_CPosTp154 ),
		ENTRY( "FT_CPosTp155", CLandMarksTestModule::FT_CPosTp155 ),
		ENTRY( "FT_CPosTp156", CLandMarksTestModule::FT_CPosTp156 ),
		ENTRY( "FT_CPosTp157", CLandMarksTestModule::FT_CPosTp157 ),
		ENTRY( "FT_CPosTp158", CLandMarksTestModule::FT_CPosTp158 ),
		ENTRY( "FT_CPosTp159", CLandMarksTestModule::FT_CPosTp159 ),
		ENTRY( "FT_CPosTp160", CLandMarksTestModule::FT_CPosTp160 ),
      
      	ENTRY( "FT_CPosTp161", CLandMarksTestModule::FT_CPosTp161 ),
      	ENTRY( "FT_CPosTp162", CLandMarksTestModule::FT_CPosTp162 ),
      	ENTRY( "FT_CPosTp163", CLandMarksTestModule::FT_CPosTp163 ),
      	ENTRY( "FT_CPosTp164", CLandMarksTestModule::FT_CPosTp164 ),

        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CLandMarksTestModule::RunTestProcedureL( 
    CLandmarkTestProcedureBase* tp,
    CStifItemParser* aItem )
    {
    TBuf<128> buf;
    TInt tpId = tp->iModuleId;
    
    CleanupStack::PushL( tp );

    _LIT(KTestInit, "---- Initializing TP %d ------");
    buf.Format( KTestInit, tpId );
    iLog->Log( buf );
    tp->InitTestL();

    _LIT(KTestStart, "---- Starting TP %d ------");
    buf.Format( KTestStart, tpId );
    iLog->Log( buf );
    if ( aItem )
        tp->StartL( *aItem );
    else
        tp->StartL();

    _LIT(KTestClose, "---- Closing TP %d ------");
    buf.Format( KTestClose, tpId );
    iLog->Log( buf );
    tp->CloseTest();

    CleanupStack::PopAndDestroy( tp );
    _LIT(KTestDestroy, "---- Destroyed TP %d ------");
    buf.Format( KTestDestroy, tpId );
    iLog->Log( buf );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
template <class T>
TInt CLandMarksTestModule::TestProcedure( TInt aId, CStifItemParser& )
    {
    iLog->Log(_L("---- Creating TP %d ------"), aId);
    T* tp = new (ELeave) T( aId, iLog );
    TRAPD( err, RunTestProcedureL( tp ) );
    return err;
    }

#define DEF_TP(_id) \
TInt CLandMarksTestModule::FT_CPosTp##_id( CStifItemParser& aItem ) \
    { return TestProcedure<CPosTp##_id>( _id, aItem ); }

DEF_TP(1)
DEF_TP(2)
DEF_TP(3)
DEF_TP(4)
DEF_TP(5)
DEF_TP(6)
DEF_TP(7)
DEF_TP(8)
DEF_TP(9)

DEF_TP(10)
DEF_TP(11)
DEF_TP(12)
DEF_TP(13)
DEF_TP(18)
DEF_TP(19)

DEF_TP(20)
DEF_TP(21)
DEF_TP(23)
DEF_TP(24)
DEF_TP(25)
DEF_TP(26)
DEF_TP(28)

DEF_TP(35)
DEF_TP(36)
DEF_TP(37)
DEF_TP(38)

DEF_TP(42)
DEF_TP(43)
DEF_TP(44)
DEF_TP(45)
DEF_TP(47)
DEF_TP(48)

DEF_TP(51)
DEF_TP(52)

DEF_TP(100)
DEF_TP(101)
DEF_TP(102)
DEF_TP(103)
DEF_TP(104)
DEF_TP(106)

DEF_TP(116)
DEF_TP(117)
DEF_TP(118)
DEF_TP(119)

DEF_TP(120)
DEF_TP(121)
DEF_TP(122)
DEF_TP(123)
DEF_TP(124)
DEF_TP(125)
DEF_TP(126)

DEF_TP(130)
DEF_TP(131)
DEF_TP(132)
DEF_TP(133)
DEF_TP(134)
DEF_TP(135)
DEF_TP(136)
DEF_TP(137)
DEF_TP(139)

DEF_TP(141)
DEF_TP(143)
DEF_TP(144)
DEF_TP(145)
DEF_TP(146)
DEF_TP(147)

DEF_TP(148)
DEF_TP(149)
DEF_TP(150)
DEF_TP(151)
DEF_TP(152)
DEF_TP(153)
DEF_TP(154)
DEF_TP(155)
DEF_TP(156)
DEF_TP(157)
DEF_TP(158)
DEF_TP(159)
DEF_TP(160)

DEF_TP(161)
DEF_TP(162)
DEF_TP(163)
DEF_TP(164)
