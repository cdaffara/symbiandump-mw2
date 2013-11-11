/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It defines dial data.
*
*/


#ifndef CPHCLTDIALDATA_H   
#define CPHCLTDIALDATA_H  


//  INCLUDES
#include <e32base.h>
#include <s32mem.h>
#include <bldvariant.hrh>
#include <phclttypes.h> 


// Constant used as window group id to indicate that we wish to go to idle.
const TInt KPhCltGoToIdle = 0;

// Constants used in Redial parameter
const TInt KPhCltRedialDefault = -1;          // Default redial.
const TInt KPhCltRedialOff = -2;              // Redial is off.
const TInt KPhCltRedialWithDefaultTime = -3;  // Redial on, with default time.
const TInt KPhCltRedial = 0;    // Forced redial.


// CONSTANTS
//None.


// FORWARD DECLARATIONS
//None

// CLASS DECLARATION
/**
* It defines the structure containing dial information.
*
* Fields:
*
*  window group    - When call is ended, set window group is actived.
*
*  show number     - Permission to show number. ETrue if it is allowed.
*
*  redial          - Automatic redial override.
*
*  name            - Name corresponding to number. Name must be defined
*                    if shown number equals to EFalse.
*  contact link      - Contact link. you should put always when you are dialing
*                    from phonebook contact.
*  tel number      - String that is to be dialled.
*                    number fields are supported.
*  allow match     - Permission to allow match. ETrue if match is to be done.
*                    If you are dialing from sim pd or fdn, then this should
*                    be EFalse.
*  number entry    - Returns whether to return to the number entry on failure 
*
*  end other calls - Information if the existing calls are ended before
*                    this new one is made.
*                    Not mandatory. Set only if you know that the call is an 
*                    emergency call.
*  subaddress      - The calling party subaddress. Supports SAT features.
*                    Defined in ITU-T I.330 and 3GPP TS 11.14.
*  SATCall         - Set whether this is SAT originated call or not. 
*                    Supports SAT features.
*  bearer          - Bearer capabilities. Supports SAT features.
*
*  CallType        - The type of the call, i.e. voice (default) or video.
*
*  SendKey         - Call made with SendKey, optional.
*
*  Redial maximum duration - Automatic redial maximum duration
*
*  Initiate call   - true if iniate call false otherwise 
*
*  UUI             - User to user information sent on call setup phase.
*
*  @lib phoneclient.lib
*  @since 5.0
*/
/**
*  Dial data set for PhoneClient.
*
*  @lib phoneclient.lib
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CPhCltDialData ): public CBase
    {
   
   public:
      
        /**
        * Destructor.
        */
        virtual ~CPhCltDialData();
        
              
        
   public: // Access - Getters
         
        /**
        * Defines if mathing is allowed.
        * @since Series 60 3.2
        * @return Returns the allow match setting information.
        */
        virtual TBool AllowMatch() const = 0;
        
        /**
        * Bearer capability.
        * @since Series 60 3.2
        * @return Returns bearer capability.
        */
        virtual const TPhCltBearerCapability& Bearer() const = 0;
        
        /**
        * Gives the current call type.
        * @since Series 60 3.2
        * @return Current call type.
        */
        virtual TPhCltCallType CallType() const  = 0;   
         
        /**
        * Contact link field.
        * @since Series 60 3.2
        * @return Returns contack link item.
        */
        virtual const TDesC8& ContactLink() const = 0;
        
        
        /**
        * End other calls param.
        * @since Series 60 3.2
        * @return Returns ETrue if other calls will be terminated.
        *                 EFalse if calls are not terminated.
        */
        virtual TBool EndOtherCalls() const = 0;
        
        /**
        * Intiate Call
        * @since Series 60 3.2
        * @return Returns ETrue if call is itiate call EFalse otherwise
        */
        virtual TBool InitiateCall() const = 0;
        
        /**
        * Name.
        * @since Series 60 3.2
        * @return Returns name.
        */
        virtual const TPhCltNameBuffer& Name() const = 0;
        
        
        /**
        * Redial information.
        * @since Series 60 3.2
        * @return Return the redial status,
        */
        virtual TInt Redial() const = 0;  
        
        /**
        * Redial maximum duration
        * @since Series 60 3.2
        * @return Returns redial max duration.
        */
        virtual const TTimeIntervalSeconds RedialMaximumDuration() const = 0;      
                   
        
        
        /**
        * SAT Call.
        * @since Series 60 3.2
        * @return Returns ETrue if SAT call EFalse if not.
        */
        virtual TBool SATCall() const = 0;
        
        /** 
        * SendKey pressed.
        * @since Series 60 3.2
        * @return Return ETrue if SendKey used.
        */
        virtual TBool SendKeyPressed() const = 0;
        
        /**
        * Show Number.
        * @since Series 60 3.2
        * @return Returns the number visibility.
        */
        virtual TBool ShowNumber() const = 0;
        
        /**
        * Sub address.
        * @since Series 60 3.2
        * @return Returns SAT call sub address.
        */
        virtual const TPhCltSubAddress& SubAddress() const = 0;
        
        /**
        * Gives the current phone number.
        * @since Series 60 3.2
        * @return Telephone number.
        */
        virtual const TPhCltTelephoneNumber& TelephoneNumber() const = 0;
        
        /**
        * Gives the current window group id.
        * @since Series 60 3.2
        * @return Window group id. 
        */     
        virtual TInt WindowGroup() const = 0; 
        
        /**
         * Gives the user to user information.
         * @since S60 v3.2
         * @return User to user information.
         */
        virtual const TDesC& UUI() const = 0;
        
    public: // Access - Setters
    
        /**
        * Set whether the name entered in SetName should be matched
        * against the personal directory.
        * @since Series 60 3.2
        * This should be set EFalse only if phone number has not been
        * taken from phonebook.
        *
        * @param aAllowMatch Whether to allow matches.
        */
        virtual void SetAllowMatch( TBool aAllowMatch ) = 0;
    
        /**
        * Set the bearer capability.
        * @since Series 60 3.2
        * @param aBearer The bearer capabilities as defined in GSM 04.08.
        * @leaves KErrArgument if aBearer's length is longer than KAiwBearerLength
        */
        virtual void SetBearerL( const TPhCltBearerCapability& aBearer ) = 0;
        
        /**
        * Sets call type.
        * @since Series 60 3.2
        * @param aCallType calls' type.
        */
        virtual void SetCallType( TPhCltCallType aCallType ) = 0;
        
        /**
        * Set contact link.
        * @since Series 60 3.2
        * @param aContactLink Contact link information.
        */
        virtual void SetContactLinkL( const TDesC8& aContactLink ) = 0;
        
        
        /**
        * Set parameter for end other calls. 
        * @since Series 60 3.2
        * If this is set to ETrue, other calls will be terminated before
        * dial. Default is EFalse.
        * @param aEndCalls Are the other calls ended.
        */
         virtual void SetEndOtherCalls( TBool aEndCalls  ) = 0;
        
        /**
        * Set Initiate call Etrue or EFalse
        *@since Series 60 3.2
        * @param aInitCall ETrue if call is intiate call
        */
        virtual void SetInitiateCall( TBool aInitCall ) = 0;
        
        /**
        * Set the name of the person who is to be called.
        * @since Series 60 3.2
        * @param aName The name of the person being called.
        * @leaves KErrArgument if aName length is over 50 characters.
        */
        virtual void SetNameL( const TPhCltNameBuffer& aName ) = 0;
        
        
        /**
        * Change the redial mode.
        * @since Series 60 3.2
        * @param aRedial The new redial status
        */
  
        virtual void SetRedial( TInt aRedial ) = 0;
        
        /**
        * Set redial max duration.
        * @since Series 60 3.2
        * @param aDuration redial max duration.
        */
        virtual void SetRedialMaximumDuration( TTimeIntervalSeconds aMaximumRedialDuration ) = 0;       
        
                        
        /**
        * Set wheteher this is a SAT originated call or not.
        * If this function is not called, the originator is assumed to 
        * be other than SAT.
        * @since Series 60 3.2
        * @param aSATCall ETrue if SAT originated the call, EFalse otherwise.
        */
        virtual void SetSATCall( TBool aSATCall ) = 0;
        
        
        /**
        * Set the value of SendKey.
        *  @since Series 60 3.2
        * @param aSendKey The value of SendKey.
        */
        virtual void SetSendKeyPressed( TBool aSendKey ) = 0;
        
        /**
        * Set the number visibility in the phone app.
        * @since Series 60 3.2
        * If this is set to EFalse, then number is not shown or written to 
        * log event.
        * @param aShowNumber Whether or not to show the number.
        */
        virtual void SetShowNumber( TBool aShowNumber ) = 0;
        
        /**
        * Set the calling party subaddress.
        * Defined in ITU-T I.330 and 3GPP TS 11.14.
        * @since Series 60 3.2
        * @param aSubAddress The calling party subaddress.
        * @leaves KErrArgument if aSubAddress length is longer than 
        * KAiwSubAddressLength
        */
        virtual void SetSubAddressL( const TPhCltSubAddress& aSubAddress ) = 0;
        
        /**
        * Sets the phone number.
        * @since Series 60 3.2
        * @param aNumber Phone number, which maximum length is
        * KMaximumPhoneNumberLength.
        * @leaves KErrArgument if aPhoneNumber length is over KMaximumPhoneNumberLength
        */
        virtual void SetTelephoneNumber( const TPhCltTelephoneNumber& aNumber ) = 0;
        
        /**
        * Sets the window group identifier. When call is ended, set window group is actived.
        * @since Series 60 3.2
        * @param aWindowGroup Window group id.
        */
        virtual void SetWindowGroup( TInt aWindowGroup ) = 0;
     
        /**
        * Set the value of xSP Service Id. 
        *
        * @param aServiceId The value of Service Id.
        */
        virtual void SetServiceId( TUint32 aServiceId ) = 0;
        
        /** 
        * Service Id.
        * @return Return service id or zero.
        */
        virtual TUint32 ServiceId() const = 0; 
        
        /**
         * Sets user to user information. Maximum length is 
         * KPhCltUUILength.
         * @param aUUI User to user information.
         * @since S60 v3.2
         */
        virtual void SetUUIL( const TDesC& aUUI ) = 0;
                      
    // Constructors
    
     protected: 
     
        /**
        * C++ default constructor.
        * @since Series 60 3.2
        */
        CPhCltDialData(); 

        /**
        * Construction that allocates memory.
        * @since Series 60 3.2
        */
        void ConstructL();     
        
        
       // Data
       
       protected:    
   
        // Telephony number storage.
        TPhCltTelephoneNumber     iTelNumber;
         
        // Call type.
        TPhCltCallType            iCallType;
        
        // The name information storage.
        TPhCltNameBuffer          iName;
        
        
        // The window group identifier storage.
        TInt                      iWindowGroup;
        
        // The new redial status storage.
        TInt                      iRedial;
        
        //Automatic redial maximum duration
        TTimeIntervalSeconds      iRedialMaximumDuration;
        
        
        //The contact Link storage.
        HBufC8*                   iContactLink;
        
        // The calling party subaddress.
        TPhCltSubAddress          iSubAddress;
       
         // The number visibility information storage.
        TBool                     iShowNumber;
        
        // Allow match information storage.
        TBool                     iAllowMatch;
        
        // The information whether this is SAT originated call or not.
        TBool                     iSATCall;
        
        // Information storage of the other call termination.
        TBool                     iEndOtherCalls;
        
        // The bearer capability.
        TPhCltBearerCapability    iBearer;
                 
        // SendKey
        TBool                     iSendKey;
        
        // Return to the number entry editor information storage.
        TBool                     iReturnToNumberEntryOnFailure;

        
        //Intialization call 
        TBool                     iInitCall;
       
        // Flags value
        TUint                     iFlags;

        // service id
        TUint32                   iServiceId;

        /**
         * User to user info.
         * own.
         */
        HBufC*                   iUUI;

    };

#endif      // CPHCLTEXTPHONEDIALDATA_H
            
// End of File
