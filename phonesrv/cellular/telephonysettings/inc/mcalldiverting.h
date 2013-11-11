/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of MCallDiverting interface class.
*
*/


#ifndef     MCALLDIVERTING_H
#define     MCALLDIVERTING_H

/**
 *  MCallDiverting class is virtual interface class for divert 
 *  functionality.
 *
 *  @lib phonesettings
 *  @since S60 v5.1
 */
class MCallDiverting
    {
    public:
        /**
         * Sets observer which informs when request is completed
         */
        virtual void SetRequestObserver( MPsetRequestObserver* aObs ) = 0;
    
        /**
        * Sets diverting to the network.
        * 
        * @param aDivert New divert settings.
        * @param aBsc List of basic service groups .
        */
        virtual void SetDivertingL( const TCallDivertSetting& aDivert,  
                                    TBasicServiceGroups aBsc ) = 0;
        /**
        * Checks the diverting status from network.
        * 
        * @param aServiceGroup Service group indication.
        * @param aCondition Diverting conditions.
        * @param aBsc List of basic service groups.
        */
        virtual void GetDivertingStatusL( 
                                    const TServiceGroup aServiceGroup, 
                                    const TCallDivertingCondition aCondition, 
                                    TBasicServiceGroups aBsc ) = 0;
        /**
        * Cancels current request.
        * 
        * @return Returns error code.
        */
        virtual TInt CancelCurrentRequest() = 0;
        
        /**
        *  Reads used divert numbers (0-3) from central repository
        *  and fills array with them.
        *
        *  @param aType used line.
        *  @param aArray array of divert number.
        *  @return amount of numbers.
        */
        virtual TInt GetNumbersFromSharedDataL( 
                                    TInt aType, 
                                    CDesC16ArrayFlat& aArray ) = 0;
        /**
        * Sets new default number the the default numbers list, 
        * erases the oldest one.
        * 
        * @param aNumber phone number. 
        * @return Returns error code.
        */
        virtual TInt SetNewDefaultNumberL( TDes& aNumber ) = 0;
        
        /**
        *  Gets the default time for CF No Reply.
        *  No error checking for return values which should be
        *  5-30, in steps of 5.
        *
        * @return Returns the current timer value.
        */
        virtual TInt GetTimerValueL() = 0;
        
        /*
        *  Sets the default time for CF No Reply.
        *  No error checking to parameters, which should be on range 5-30,
        *  using steps of 5.
        *
        *  @param aIndex New value for timer.
        *  @return Returns error code.
        */
        virtual TInt SetTimerValueL( const TInt& aValue ) = 0;
        
        /**
        * Swaps the most recently used number to first in shared data file.
        *
        * @param  aLocation Used number's location (1-3) in shared data.
        */
        virtual void SwapDefaultNumberL( const TInt& aLocation ) = 0;
        
        /**
        * Saves a single value to central repository. Make sure that repository
        * is open before calling this function.
        */
        virtual TInt SaveKey( TUint32 aKeyId, const TDesC& aKeyValue ) = 0;
        
        /*
        * Virtual destructor. 
        */ 
        virtual ~MCallDiverting() {};
    };

#endif // MPCALLDIVERTING_H
