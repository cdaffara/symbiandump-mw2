/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Pointer holder for client's data
*
*/


#include <e32def.h>
#include <e32base.h>
#include <e32math.h>
#include "epos_suplterminalqop.h"
#include "epos_suplterminalconstants.h"

const TInt KMinHorAcc=0;
const TInt KMaxHorAcc=1800000;
const TInt KMinVerAcc=0;
const TInt KMaxVerAcc=65535;
const TInt KMinLocAge=0;
const TInt KMaxLocAge=65535;
const TInt KMinDelay=1;
const TInt KMaxDelay=65535;


// -----------------------------------------------------------------------------
// TSuplTerminaQop::TSuplTerminaQop()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
 EXPORT_C TSuplTerminalQop::TSuplTerminalQop():iHorizontalAccuracy(KUndefinedHorizontalAccuracy),
 											   iVerticalAccuracy(KUndefinedVerticalAccuracy),
 											   iMaxLocationAge(KUndefinedMaxLocationAge),
 											   iResponseTime(KUndefinedDelay)
 											   
 	{ 	
 	}
 	
 // ---------------------------------------------------------
// TSuplTerminaQop::Set
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop:: Set(
 					TReal32 aHorAccuracy, 
 					TReal32 aVerAccuracy, 
 					TInt aMaxLocationAge, 
 					TInt aDelay)
 	{
	if(aHorAccuracy<KMinHorAcc ||aHorAccuracy>KMaxHorAcc)
			return KErrArgument;
	if(aVerAccuracy<KMinVerAcc || aVerAccuracy>KMaxVerAcc)
			return KErrArgument;
	if(aMaxLocationAge<KMinLocAge || aMaxLocationAge>KMaxLocAge)
			return KErrArgument;
 	if(aDelay<KMinDelay||aDelay>KMaxDelay)
 			return KErrArgument;
 		

	iResponseTime=aDelay;
 	iHorizontalAccuracy=aHorAccuracy;
 	iVerticalAccuracy=aVerAccuracy;
 	iMaxLocationAge=aMaxLocationAge;
 	return KErrNone;
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::SetHorizontalAccuracy
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::SetHorizontalAccuracy(TReal32 aHorAccuracy)
 	{
	if(aHorAccuracy<KMinHorAcc || aHorAccuracy>KMaxHorAcc)
			return KErrArgument;
	
	iHorizontalAccuracy=aHorAccuracy;
	
	return KErrNone;
 	}
 	
 // ---------------------------------------------------------
// TSuplTerminaQop::SetVerticalAccuracy
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::SetVerticalAccuracy(TReal32 aVerAccuracy)
 	{
	if(aVerAccuracy<KMinVerAcc || aVerAccuracy>KMaxVerAcc)
		return KErrArgument;

	iVerticalAccuracy=aVerAccuracy;	
	return KErrNone;
 	}
 	
 // ---------------------------------------------------------
// TSuplTerminaQop::SetMaxLocationAge
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::SetMaxLocationAge(TInt aMaxLocationAge)
 	{ 	
 	if(aMaxLocationAge<KMinLocAge || aMaxLocationAge>KMaxLocAge)
		return KErrArgument; 	
 	
 	iMaxLocationAge=aMaxLocationAge;
 	return KErrNone;	
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::SetDelay
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::SetDelay(TInt aDelay)
 	{
	if(aDelay<KMinDelay||aDelay>KMaxDelay)
		return KErrArgument;
	
	iResponseTime=aDelay;
	return KErrNone;
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::Get
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::Get(
 					TReal32& aHorAccuracy, 
 					TReal32& aVerAccuracy, 
 					TInt& aMaxLocationAge, 
 					TInt& aDelay) const
 	{
 	if(iHorizontalAccuracy==KUndefinedHorizontalAccuracy && 
 				iVerticalAccuracy==KUndefinedVerticalAccuracy && 
 				iMaxLocationAge==KUndefinedMaxLocationAge && 
 				iResponseTime==KUndefinedDelay)
 		{
 		return KErrNotFound;
 		}
 	 	
 	aDelay=iResponseTime;
	
 	aHorAccuracy=iHorizontalAccuracy;
 	aVerAccuracy=iVerticalAccuracy;
 	aMaxLocationAge=iMaxLocationAge;
 	
 	return KErrNone; 	
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::GetHorizontalAccuracy
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::GetHorizontalAccuracy(TReal32& aHorAccuracy) const
 	{
 	if(iHorizontalAccuracy==KUndefinedHorizontalAccuracy )
 		return KErrNotFound;
 	
 	aHorAccuracy=iHorizontalAccuracy;
 	return KErrNone;
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::GetVerticalAccuracy
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::GetVerticalAccuracy(TReal32& aVerAccuracy) const
 	{
 	if(iVerticalAccuracy==KUndefinedVerticalAccuracy)
 		return KErrNotFound;
 	aVerAccuracy=iVerticalAccuracy;
 	return KErrNone;
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::GetMaxLocationAge
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::GetMaxLocationAge(TInt& aMaxLocationAge) const
 	{
 	if(iMaxLocationAge==KUndefinedMaxLocationAge)
 		return KErrNotFound;

 	aMaxLocationAge=iMaxLocationAge;
 	return KErrNone;
 	}
 	
// ---------------------------------------------------------
// TSuplTerminaQop::GetDelay
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 EXPORT_C TInt TSuplTerminalQop::GetDelay(TInt& aDelay) const
 	{
 	if(iResponseTime==KUndefinedDelay)
 		return KErrNotFound;

 	aDelay=iResponseTime;
 	return KErrNone;
 	}
