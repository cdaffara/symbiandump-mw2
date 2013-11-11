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
* Description:  Implementation of TestSuplTerminalQop
*
*/



// INCLUDE FILES

#include <StifParser.h>
#include <Stiftestinterface.h>
#include <epos_suplterminalqop.h>
#include "testsuplterminalqop.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::TestSuplTerminalQop
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TestSuplTerminalQop::TestSuplTerminalQop( CStifLogger* aLog )				
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::~TestSuplTerminalQop
// 
// 
// -----------------------------------------------------------------------------
//
TestSuplTerminalQop::~TestSuplTerminalQop()
	{
	
	}
	
// -----------------------------------------------------------------------------
// TestSuplTerminalQop::SetL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::SetL( CStifItemParser& aItem )
	{
	// Hor Accuracy
	TPtrC horAccPtr;
	User::LeaveIfError( aItem.GetNextString( horAccPtr ) );
	TLex lex1( horAccPtr );
	TReal32 horAccuracy;
	lex1.Val( horAccuracy );
	// Ver Accuracy
	TPtrC verAccPtr;
	User::LeaveIfError( aItem.GetNextString( verAccPtr ) );
	TLex lex2( verAccPtr );
	TReal32 verAccuracy;
	lex2.Val( verAccuracy );
    // Max location age
    TInt age;
	User::LeaveIfError( aItem.GetNextInt( age ) );
	// Delay
	TInt delay;
	User::LeaveIfError( aItem.GetNextInt( delay ) );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.Set( horAccuracy, verAccuracy, age, delay );
	return err;
	}

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::SetHorAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::SetHorAccuracyL( CStifItemParser& aItem )
	{
	// Hor Accuracy
	TPtrC horAccPtr;
	User::LeaveIfError( aItem.GetNextString( horAccPtr ) );
	TLex lex( horAccPtr );
	TReal32 horAccuracy;
	lex.Val( horAccuracy );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetHorizontalAccuracy( horAccuracy );
	return err;
	}
	
// -----------------------------------------------------------------------------
// TestSuplTerminalQop::SetVerAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::SetVerAccuracyL( CStifItemParser& aItem )
	{
	// Ver Accuracy
	TPtrC verAccPtr;
	User::LeaveIfError( aItem.GetNextString( verAccPtr ) );
	TLex lex( verAccPtr );
	TReal32 verAccuracy;
	lex.Val( verAccuracy );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetVerticalAccuracy( verAccuracy );
	return err;
	}	
	
// -----------------------------------------------------------------------------
// TestSuplTerminalQop::SetMaxLocationAgeL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::SetMaxLocationAgeL( CStifItemParser& aItem )
	{
	// Max location age
    TInt age;
	User::LeaveIfError( aItem.GetNextInt( age ) );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetMaxLocationAge( age );
	return err;
	}

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::SetDelayL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::SetDelayL( CStifItemParser& aItem )
	{
	// Delay
    TInt delay;
	User::LeaveIfError( aItem.GetNextInt( delay ) );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetDelay( delay );
	return err;
	}

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::GetL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::GetL( CStifItemParser& aItem )
	{
	// Get params to be set from cfg
	// Hor Accuracy
	TPtrC horAccPtr;
	User::LeaveIfError( aItem.GetNextString( horAccPtr ) );
	TLex lex1( horAccPtr );
	TReal32 horAccuracy;
	lex1.Val( horAccuracy );
	// Ver Accuracy
	TPtrC verAccPtr;
	User::LeaveIfError( aItem.GetNextString( verAccPtr ) );
	TLex lex2( verAccPtr );
	TReal32 verAccuracy;
	lex2.Val( verAccuracy );
    // Max location age
    TInt age;
	User::LeaveIfError( aItem.GetNextInt( age ) );
	// Delay
	TInt delay;
	User::LeaveIfError( aItem.GetNextInt( delay ) );
	// Set parameters
	TSuplTerminalQop suplTermQop;
	TInt err = 	suplTermQop.Set( horAccuracy, verAccuracy, age, delay );
	// Get parameters
	TReal32 getHorAcc;
	TReal32 getVerAcc;
	TInt getAge;
	TInt getDelay;
	err = suplTermQop.Get( getHorAcc, getVerAcc, getAge, getDelay );
	return err;
	}

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::GetHorAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::GetHorAccuracyL( CStifItemParser& aItem )
	{
	// Hor Accuracy
	TPtrC horAccPtr;
	User::LeaveIfError( aItem.GetNextString( horAccPtr ) );
	TLex lex( horAccPtr );
	TReal32 horAccuracy;
	lex.Val( horAccuracy );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetHorizontalAccuracy( horAccuracy );
	// Get Hor Accuracy
	TReal32 getHorAcc;
	err = suplTermQop.GetHorizontalAccuracy( getHorAcc);
	return err;
	}
	
// -----------------------------------------------------------------------------
// TestSuplTerminalQop::GetVerAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::GetVerAccuracyL( CStifItemParser& aItem )
	{
	// Ver Accuracy
	TPtrC verAccPtr;
	User::LeaveIfError( aItem.GetNextString( verAccPtr ) );
	TLex lex( verAccPtr );
	TReal32 verAccuracy;
	lex.Val( verAccuracy );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetVerticalAccuracy( verAccuracy );
	// Get Ver Accuracy
	TReal32 getVerAcc;
	err = suplTermQop.GetVerticalAccuracy( getVerAcc);
	return err;
	}	
	
// -----------------------------------------------------------------------------
// TestSuplTerminalQop::GetMaxLocationAgeL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::GetMaxLocationAgeL( CStifItemParser& aItem )
	{
	// Max location age
    TInt age;
	User::LeaveIfError( aItem.GetNextInt( age ) );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetMaxLocationAge( age );
	// Get max location age
	TInt getAge;
	err = suplTermQop.GetMaxLocationAge( getAge );
	return err;
	}

// -----------------------------------------------------------------------------
// TestSuplTerminalQop::GetDelayL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestSuplTerminalQop::GetDelayL( CStifItemParser& aItem )
	{
	// Delay
    TInt delay;
	User::LeaveIfError( aItem.GetNextInt( delay ) );
	//
	TSuplTerminalQop suplTermQop;
	TInt err = suplTermQop.SetDelay( delay );
	// Get Delay
	TInt getDelay;
	err = suplTermQop.GetDelay( getDelay );
	return err;
	}

