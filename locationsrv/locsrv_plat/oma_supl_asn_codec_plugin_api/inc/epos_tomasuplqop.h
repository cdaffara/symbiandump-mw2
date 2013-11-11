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
* Description:  Header file for qop field in message
*
*/

#ifndef T_TOMASUPLQOP_H
#define T_TOMASUPLQOP_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>


/**
 *  Class for providing the Qop support in supl message
 *  For holding the Qop in message
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1
 */
class TOMASuplQop 
 {
 public: 
        /**
        * Performs construction of an object
        * @since Series 60 3.1U
        * @param None
        * @return Returns instance of TOMASuplQop
        */

     	IMPORT_C TOMASuplQop();

     
public:	//Getter Methods

        /**
        * Getter method for Horizontal Accuracy in Qop
        * @since Series 60 3.1
        * @aHorizAccuracy Horizontal Accuracy
        * @return None											 
        */														 
		IMPORT_C TInt SuplQop() const;
		
		/**
        * Getter method for Vertical Accuracy in Qop
        * @since Series 60 3.1
        * @aVertAccuracy Vertical Accuracy
        * @return Error code in case parameter is not set											
        */
		IMPORT_C TInt VerticalAccuracy(TInt& aVertAccuracy)const;
		
		/**
        * Getter method for Maximum Location Age in Qop
        * @since Series 60 3.1
        * @aMaxLocationAge Maximum Location Age
        * @return Error code in case parameter is not set											
        */
		IMPORT_C TInt MaxLocationAge(TInt& aMaxLocationAge)const;
		
		/**
        * Getter method for Delay in Qop
        * @since Series 60 3.1
        * @aDelay Delay
        * @return Error code in case parameter is not set											
        */
		IMPORT_C TInt Delay(TInt& aDelay)const;
        
public:		//Setter Methods

        /**
        * Setter method for Horizontal Accuracy in Qop
        * @since Series 60 3.1
        * @aHorizAccuracy Horizontal Accuracy
        * @return None											 
        */														 
														 
		IMPORT_C void SetSuplQop(TInt aHorizAccuracy);
		
		/**
        * Setter method for Vertical Accuracy in Qop
        * @since Series 60 3.1
        * @aVertAccuracy Vertical Accuracy
        * @return None											
        */ 
		IMPORT_C void SetVerticalAccuracy(TInt aVertAccuracy);
		
		/**
        * Setter method for Maximum Location Age in Qop
        * @since Series 60 3.1
        * @aMaxLocationAge Maximum Location Age
        * @return None
        */
		IMPORT_C void SetMaxLocationAge(TInt aMaxLocationAge);
		
		/**
        * Setter method for Delay in Qop
        * @since Series 60 3.1
        * @aDelay Delay
        * @return None											
        */
		IMPORT_C void SetDelay(TInt aDelay);
		
private :	
		// Horizontal Accuracy in Qop
		TInt iHorizAccuracy;
		// Vertical Accuracy in Qop
	    TInt iVertAccuracy;
	    // Maximum Location Age in Qop
	    TInt iMaxLocationAge;
	    // Delay in Qop
	    TInt iDelay;
	    //Optional Mask
	    TInt iOptionalMask;
    };


#endif // T_TOMASUPLQOP_H