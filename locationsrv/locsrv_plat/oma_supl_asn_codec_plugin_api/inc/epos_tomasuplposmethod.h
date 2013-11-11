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
* Description:  Enumerations related to pos method
*
*/


#ifndef T_TOMASUPLPOSMETHOD_H
#define T_TOMASUPLPOSMETHOD_H

/**
*  Enumeration for holding the PosMethods
*  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

enum TOMASuplPosMethod
	{
		EOMAAgpsSETAssisted = 0,
	    EOMAAgpsSETBased    = 1,
	    EOMAAgpsSETAssistedPref,
	    EOMAAgpsSETBasedPref,
	    EOMAAutonomousGps,  
	    EOMAFLT,  
	    EOMAeCID,  		
	    EOMAeOTD,  		
	    EOMAoTDOA,
	    EOMANoPosition
	};
	
#endif //T_TOMASUPLPOSMETHOD_H