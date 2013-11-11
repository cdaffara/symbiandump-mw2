/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   _Defines_H 
*		Defines the parameter values used in the IMAAMI algorithms.
*
*/




#ifndef _Defines_H
#define _Defines_H

#define EXPO_SHIFT			13			// Accuracy in exponent table
#define EXPO_MULTI			(1 << EXPO_SHIFT)	// Multiplier for exponent
#define EXPO_SIZE			(3 * EXPO_MULTI)	// Size of exponent table
#define EXPO_ACCU			12			// Accuracy of weight value
#define EXPO_MULT			(1 << EXPO_ACCU)
#define EXPO_DIFF			(2 * SHIFT_PLACE - EXPO_SHIFT)

#define SHIFT_COEFF			(16)	// Accuracy of float to integer (coeff)
#define SHIFT_PLACE			( 8)	// Accuracy of float to integer (place)
#define SHIFT_DIFF			( 8)	// Accuracy reduction		(SHIFT_COEFF - SHIFT_PLACE)

#define EXPO_SIZE2			(384)	// Size of exponent table	((3 * (1 << EXPO_SHIFT2)) >> 1)
#define EXPO_DIFF2			(  8)	// Reduction of accuracy	(SHIFT_COEFF - EXPO_SHIFT2)


#endif // ifndef _Defines_H
//----IMAAMI----
