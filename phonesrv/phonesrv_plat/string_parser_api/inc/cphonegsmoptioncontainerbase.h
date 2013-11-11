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
* Description:  Base class for options container that can be used 
*                to configure the parsing process. 
*
*/


#ifndef CPHONEGSMOPTIONCONTAINERBASE_H_
#define CPHONEGSMOPTIONCONTAINERBASE_H_

// INCLUDES
#include    <e32base.h>

// CONSTANTS

/**
* Option is true if operation is send. False if it is a code check.
*/
const TInt KPhoneOptionSend = 1;

/**
* Option is true if there is a call ongoing. Otherwise false.
*/
const TInt KPhoneOptionInCall = 2;

/**
* Option is true if two digit calling is supported. Otherwise false.
* Client should not try to set this option with SetOptionStatus() -method because 
* Two Digit Calling is variable feature.
*/
const TInt KPhoneOptionTwoDigitCalling = 3;

/**
* Option is true if voip call. Otherwise false.
*/
const TInt KPhoneOptionVoipCall = 4;

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* It is container for options. Each option is identified by 
* a single uid and each option has a boolean status.
*
* @since 5.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmOptionContainerBase ) 
    : public CBase
    {
    
    public: // New functions
                       
        /**
        * Sets status of option.
        *        
        * @param aOptionUid It is the uid of the option.
        * @param aStatus It is the new status for the option.
        */
        virtual void SetOptionStatus( TInt aOptionUid, TBool aStatus ) = 0;    
        
        /**
        * Finds status of option.
        *
        * @param aOptionUid It is the uid of the option.
        * @return Returne status of the option.
        */
        virtual TBool FindOptionStatus( TInt aOptionUid ) const = 0;

    };

#endif /*CPHONEGSMOPTIONCONTAINERBASE_H_*/
