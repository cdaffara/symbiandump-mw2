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
* Description:  Call diverting class. Sets and gets call diverting status. 
*
*
*/


#ifndef     PSETCALLDIVERTING_H
#define     PSETCALLDIVERTING_H

//  INCLUDES           
#include <mmretrieve.h>
#include "nwdefs.h"             
#include "psetconstants.h" 
#include "mpsetcalldiverting.h" 

//  FORWARD DECLARATIONS
class CDesC16ArrayFlat;
class MPsetDivertObserver;
class MPsetRequestObserver;
class RVmbxNumber;
class CRepository;
class TCallDivertNotifySetting;
class MPSetCallDiverting;
class CPSetCallDivertingFactory;
class MCallDiverting;

enum TSelectedLine
    {
    EPrimaryLine,       // ALS is supported by ME & SIM and Line 1 (Primary Line) is the current line
    EAuxiliaryLine,     // ALS is supported by ME & SIM and Line 2 (Auxiliary Line) is the current line
    EUnknownLine,       // ALS is supported by ME & SIM but TSY can not determine line selection
    ENotSupportedLine   // ALS is not supported either by ME or SIM or both.
    };

//  CLASS DECLARATION  
/**
*  CPsetCallDiverting class is call diverting SS handler. 
*  @lib phonesettings.lib
*  @since 1.0
*/
class   CPsetCallDiverting : public CBase,
                             public MPsetCallDiverting
    {
    public: // constructor & destructor

        /**
        * Symbian OS 2-phase Constructor.
        *
        * @param aObserver Call divert observer.
        * @param aPhone Provides client access to 
        *               mobile phone functionality provided by TSY.
        * @return Created CPsetCallDiverting object.
        */
        IMPORT_C static CPsetCallDiverting* NewL( 
            MPsetDivertObserver& aObserver, 
            RMobilePhone& aPhone);
        
        /* 
        * Destructor.
        */
        IMPORT_C ~CPsetCallDiverting();
                
    public: // From base class.

        /**
        * @see MPsetCallDiverting::SetDivertingL().
        */
        IMPORT_C void SetDivertingL( const TCallDivertSetting& aDivert, 
            TBasicServiceGroups aBsc, TBool aVmbx = EFalse ); 

        /**
        * @see MPsetCallDiverting::GetDivertingStatusL().
        */ 
        IMPORT_C void GetDivertingStatusL( const TServiceGroup aServiceGroup, 
            const TCallDivertingCondition aCondition, 
            TBasicServiceGroups aBsc );
        
        /**
        * @see MPsetCallDiverting::CancelCurrentRequest().
        */ 
        IMPORT_C TInt CancelCurrentRequest(); 
    
    public: //new

        /**
        *  Gets the default time for CF No Reply.
        *  No error checking for return values which should be
        *  5-30, in steps of 5.
        *
        * @return Returns the current timer value.
        */
        IMPORT_C TInt GetTimerValueL();    
    
        /**
        *  Sets the default time for CF No Reply.
        *  No error checking to parameters, which should be on range 5-30,
        *  using steps of 5.
        *
        *  @param aIndex New value for timer.
        *  @return Returns error code.
        */
        IMPORT_C TInt SetTimerValueL( const TInt& aIndex );    
        
        /**
        * The default (last forwarded-to) numbers can be retrieved 
        * through this method. 
        *
        * @param aDefNumbers Read default numbers from shared data.
        * @return Returns error code or amount of numbers read.
        */
        IMPORT_C TInt GetDefaultNumbersL( CDesC16ArrayFlat& aDefNumbers );            

        /**
        * Sets new default number to the default numbers list, 
        * erases the oldest one automatically, if three numbers already in the
        * shared data.
        *
        * @param aNumber New number to be added.
        * @return Returns error code.
        */
        IMPORT_C TInt SetNewDefaultNumberL( TDes& aNumber );    

        /**
        * Swaps the most recently used number to first in shared data file.
        *
        * @param  aLocation Used number's location (1-3) in shared data.
        */
        IMPORT_C void SwapDefaultNumberL( const TInt& aLocation );

        /**
        * Saves last used data number to shared data.
        *
        * @param  aNumber Number used for data call forwarding.
        * @return Returns error code.
        */
        IMPORT_C TInt SetUsedDataNumberL( TTelNumber& aNumber );

        /**
        * Saves last used fax number to shared data.
        *
        * @param  aNumber Number used for data fax forwarding.
        * @return Returns error code.
        */
        IMPORT_C TInt SetUsedFaxNumberL( TTelNumber& aNumber );

        /**
        * Gets the phone number used for fax call forwarding.
        *
        * @return Number used for data call forwarding.
        */
        IMPORT_C HBufC* GetUsedFaxNumberLC();

        /**
        * Gets the phone number used for data call forwarding.
        *
        * @return Number used for data call forwarding.
        */
        IMPORT_C HBufC* GetUsedDataNumberLC();

        /** 
        * Sets request observer.
        *
        * @param aObs Where the request completion should be informed to.
        */        
        IMPORT_C void SetRequestObserver( MPsetRequestObserver* aObs );

        /** 
        * Queries for voice mail box number. If number is undefined, number 
        * entry pops up.
        * @since 2.0
        * 
        * @param aTelNumber Vmbx number.
        */        
        IMPORT_C void VoiceMailQueryL( TDes& aTelNumber );
        
        /** 
        * Queries for video mailbox number. If number is undefined, number 
        * entry pops up.
        * @since 3.1
        * 
        * @param aTelNumber Video mailbox number.
        */         
        IMPORT_C void VideoMailQueryL( TDes& aTelNumber );

    private: //new
        
        TInt OpenVideoMailboxLC( TDes& aTelNumber, RVmbxNumber& aVmbx );
        
    private: // constructors
              
        void ConstructL( MPsetDivertObserver& aObserver );
        
        CPsetCallDiverting( RMobilePhone& aPhone );
               
    private: //member data
        
        // Observer for Call divert requests
        MPsetDivertObserver* iObserver;
        
        // Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iPhone;
        
        // Request observer
        MPsetRequestObserver* iReqObserver;

        // Central repository.
        CRepository* iRepository;
        
        // Call Divert implementation.
        MCallDiverting* iDivert;
     };

#endif      //  PSETCALLDIVERTING_H

// End of File
