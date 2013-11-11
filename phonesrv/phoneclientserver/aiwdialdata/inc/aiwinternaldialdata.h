/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Dial data used with AIW for making calls.
*
*/


#ifndef CAIWINTERNALDIALDATA_H
#define CAIWINTERNALDIALDATA_H

//  INCLUDES
#include <s32mem.h>
#include <AiwGenericParam.h> 
#include "aiwdialdata.h"

/** Constants used by aiwinternaldialdata interface. */ 
namespace AIWInternalDialData
    {
    /** Maximum name length. */ 
    const TInt KMaximumNameLength = 50;

    /** Maximum phone number length same as  used by phone. */ 
    const TInt KMaximumPhoneNumberLength = 100;

    /** The subaddress length, see ITU-T I.330 and 3GPP TS 11.14. */ 
    const TInt KAiwSubAddressLength = 21;

    /** The maximum bearer length. The bearer capabilities as 
    defined in GSM 04.08. */ 
    const TInt KAiwBearerLength = 14;
    
    /** Maximum length of UUI data. */
    const TInt KAiwUUILength = 129;
    }

// CLASS DECLARATION

/**
*  Dial data for SAT application.
*
*  @lib aiwdialdata.lib
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CAiwInternalDialData ): public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor for implementation class. 
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwInternalDialData* NewL();
        
        /**
        * Two-phased constructor for implementation class. Leaves the 
        * pointer to the cleanup stack.
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwInternalDialData* NewLC();
           
           /**
        * Two-phased constructor for implementation class. 
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @param aPackage descriptor parameter.
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwInternalDialData* NewL( const TDesC8& aPackage );
        
           /**
        * Two-phased constructor for implementation class. Leaves the 
        * pointer to the cleanup stack.
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @param aPackage descriptor parameter.
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwInternalDialData* NewLC( const TDesC8& aPackage );
        
        /**
        * Destructor.
        */
        virtual ~CAiwInternalDialData();
        
    public: // New
    
        /**
        * Composes data set package and inserts data to aInParamList.
        *
        * @since S60 3.2
        * @param aInParamList In parameter used with CAiwService.
        */
        IMPORT_C void FillInParamListL( CAiwGenericParamList& aInParamList );

    public: // Access - Setters
    
        /**
        * Sets the phone number. If phone number and contact link are both 
        * defined call will be made to contact link.
        * 
        * @since S60 3.2
        * @param aPhoneNumber Phone number, which maximum length is
        *        KMaximumPhoneNumberLength. 
        *        Default: empty.
        * @leaves KErrArgument if aPhoneNumber length is over 
        *        KMaximumPhoneNumberLength.
        */
        IMPORT_C void SetPhoneNumberL( const TDesC& aPhoneNumber );
        
        /**
        * Sets call type.
        *
        * @since S60 3.2
        * @param aCallType defines the call type.
        *        Default: EAIWVoice.
        */
        IMPORT_C void SetCallType( CAiwDialData::TCallType aCallType );
        
        /**
        * Sets the window group identifier. When call is ended, 
        * set window group is actived.
        *
        * @since S60 3.2
        * @param aWindowGroup Window group id.
        *        Default: KAiwGoToIdle.
        */
        IMPORT_C void SetWindowGroup( TInt aWindowGroup );
        
        /**
        * Sets initiate call.
        *
        * @since S60 3.2
        * @param aInitCall ETrue if call is created by using
        *        long press of send key otherwise EFalse.
        *        Default: EFalse.
        */
        IMPORT_C void SetInitiateCall( TBool aInitCall );
        
        /**
        * Sets the name of the person who is to be called. If client sets name
        * then set name is shown not contact card defined name. Set name is shown
        * also in log event.
        *
        * @since S60 3.2
        * @param aName The name of the person being called.
        * @leaves KErrArgument if aName length is over KMaximumNameLength.
        *         Default: Empty.
        */
        IMPORT_C void SetNameL( const TDesC& aName );        
        
        /**
        * Change the redial mode. For used parameters see redial related 
        * constants above.
        *
        * @since S60 3.2
        * @param aRedial the new redial status.
        *        Default: KAIWRedialDefault.
        */
        IMPORT_C void SetRedial( TInt aRedial );
        
        /**
        * Set the number visibility in the phone app.
        * If this is set to EFalse, then number is not shown at all in callbubble
        * If number has defined contact card contact name is shown in call bubble.
        *
        * @since S60 3.2
        * @param aShowNumber Whether or not to show the number.
        *        Default: ETrue.
        */
        IMPORT_C void SetShowNumber( TBool aShowNumber );
        
        /**
        * Set whether the phone number should be matched
        * against the personal directory. 
        *
        * If ETrue and name is set then number is matched to
        * phonebook but set name is shown in call bubble.
        * If name is not set then no name is shown in call bubble
        * just phone number.
        *
        * This should be set EFalse only if phone number has not been
        * taken from phonebook.
        *
        * @since S60 3.2
        * @param aAllowMatch Whether to allow matches.
        *        Default: ETrue.
        */
        IMPORT_C void SetAllowMatch( TBool aAllowMatch );
                
        /**
        * Set contact link. If phone number and contact link are both 
        * defined call will be made to contact link.
        *
        * @since S60 3.2
        * @param aContactLink Contact link information.
        *        Default: Empty.
        */
        IMPORT_C void SetContactLinkL( const TDesC8& aContactLink );
        
        /**
        * Set wheteher this is a SAT originated call or not.
        * If this function is not called, the originator is assumed to 
        * be other than SAT.
        *
        * @since S60 3.2
        * @param aSATCall ETrue if SAT originated the call, EFalse otherwise.
        *        Default: EFalse.
        */
        IMPORT_C void SetSATCall( TBool aSATCall );

        /**
        * Set the calling party subaddress.
        * Defined in ITU-T I.330 and 3GPP TS 11.14.
        *
        * @since S60 3.2
        * @param aSubAddress The calling party subaddress.
        * @leaves KErrArgument if aSubAddress length is longer than 
        *         50 characters.
        *         Default: Empty.
        */
        IMPORT_C void SetSubAddressL( const TDesC& aSubAddress );
        
        /**
        * Set parameter for end other calls. 
        * If this is set to ETrue, other calls will be terminated 
        * before dial. 
        *
        * @since S60 3.2
        * @param aEndCalls Are the other calls ended.
        *        Default: EFalse.
        */
        IMPORT_C void SetEndOtherCalls( TBool aEndOtherCalls );
        
        /**
        * Set the bearer capability.
        *
        * @since S60 3.2
        * @param aBearer The bearer capabilities as defined in GSM 04.08.
        * @leaves KErrArgument if aBearer's length is longer than 14
        *         characters.
        *         Default: Empty.
        */
        IMPORT_C void SetBearerL( const TDesC8& aBearer );
                
        /**
        * Set redial max duration time.
        *
        * @since S60 3.2
        * @param aMaxDuration The value of duration in seconds.
        *        Default: Empty.
        */
        IMPORT_C void SetRedialMaximumDuration( 
                        const TTimeIntervalSeconds aMaximumRedialDuration );
                        
        /**
        * Set id of used call service.
        *
        * @param aServiceId The value of Service Id.
        *        Default: Null.
        */
        IMPORT_C void SetServiceId( TUint32 aServiceId );    

        /**
         * Sets the user to user information. If this is set then the UUI data
         * is sent to the recipient via UUS mechanism at call setup phase.
         *
         * Note that UUI is not sent if telephony local variation 
         * KTelephonyLVFlagUUS is not set.
         * 
         * @since S60 v3.2
         * @param aUUI User to user information. Maxmimum length of UUI is
         * defined by constant KAiwUUILength. Leaves with KErrArgument if 
         * given data is longer than KAiwUUILength.
         */
        IMPORT_C void SetUUIL( const TDesC& aUUI );
     
   public: // Access - Getters
        
        /**
        * Phone number.
        *
        * @since S60 3.2
        * @return Phone number.
        */
        IMPORT_C const TDesC& PhoneNumber() const;
        
        /**
        * Call type.
        *
        * @since S60 3.2
        * @return Current call type.
        */
        IMPORT_C CAiwDialData::TCallType CallType() const;
        
        /**
        * Window group id.
        *
        * @since S60 3.2
        * @return Window group id. 
        */    
        IMPORT_C TInt WindowGroup() const;
        
        /**
        * Initiate call.
        *
        * @since S60 3.2
        * @return Returns ETrue if client wants to initialize voice/video 
        *                 call by using long key press. EFalse otherwise.
        */
        IMPORT_C TBool InitiateCall() const;
        
        /**
        * Name.
        *
        * @since S60 3.2
        * @return Returns name.
        */
        IMPORT_C const TDesC& Name() const;
        
        /**
        * Redial information.
        *
        * @since S60 3.2
        * @return Return the redial status,
        */
        IMPORT_C TInt Redial() const;
        
        /**
        * Show Number.
        *
        * @since S60 3.2
        * @return Returns the number visibility.
        */
        IMPORT_C TBool ShowNumber() const;
        
       /**
        * Defines if mathing is allowed.
        *
        * @since S60 3.2
        * @return Returns the allow match setting information.
        */
        IMPORT_C TBool AllowMatch() const;
        
        /**
        * Contact link field.
        *
        * @since S60 3.2
        * @return Returns contack link item.
        */
        IMPORT_C const TDesC8& ContactLink() const;
        
        /**
        * SAT Call.
        *
        * @since S60 3.2
        * @return Returns ETrue if SAT call EFalse if not.
        */
        IMPORT_C TBool SATCall() const;
        
        /**
        * Sub address.
        *
        * @since S60 3.2
        * @return Returns SAT call sub address.
        */
        IMPORT_C const TDesC& SubAddress() const;
        
        /**
        * End other calls param.
        *
        * @since S60 3.2
        * @return Returns ETrue if other calls will be terminated.
        *                  EFalse if calls are not terminated.
        */
        IMPORT_C TBool EndOtherCalls() const;
        
        /**
        * Bearer capability.
        *
        * @since S60 3.2
        * @return Returns bearer capability.
        */
        IMPORT_C const TDesC8& Bearer() const;
        
        /**
        * Automatic redial maximum duration. 
        *
        * @since S60 3.2
        * @return  value of duration in seconds.
        */
        IMPORT_C const TTimeIntervalSeconds RedialMaximumDuration() const;
        
        /** 
        * Read used Service Id.
        *
        * @return Return service id.
        */
        IMPORT_C TUint32 ServiceId() const;

        /**
         * User to user information.
         * 
         * @since S60 v3.2
         * @return User to user information.
         */
        IMPORT_C const TDesC& UUI() const;

    private:
        
        TInt CalculatePackageSize() const;
        
        TInt CalculateDescriptorSize( const TDesC& aDescriptor ) const;
        
        TInt CalculateDescriptorSize( const TDesC8& aDescriptor ) const;
        
        void ReadDescriptorFromStreamL( RDesReadStream& aStream, 
                                        HBufC*& aDescriptor );
        
        void ReadDescriptorFromStreamL( RDesReadStream& aStream, 
                                        HBufC8*& aDescriptor );
        
        void WriteDescriptorToStreamL( RDesWriteStream& aStream, 
                                       const TDesC& aDescriptor );
        
        void WriteDescriptorToStreamL( RDesWriteStream& aStream, 
                                       const TDesC8& aDescriptor );
        
        void CopyDescriptorToMemberDescriptorL( const TDesC& aSource, 
                                       HBufC*& aMemberDescriptor );
        
        void CopyDescriptorToMemberDescriptorL( const TDesC8& aSource, 
                                       HBufC8*& aMemberDescriptor );
                
        /**
        * Leaves with KErrArgument if aDescriptorLength 
        * is greated that aMaxAllowedDescriptorLenght
        */
        void CheckDescriptorMaxLengthL( TInt aDescriptorLength, 
            TInt aMaxAllowedDescriptorLength ) const;
        
        HBufC8* PackLC();
        
    private:

        /**
        * C++ default constructor.
        */
        CAiwInternalDialData();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( const TDesC8& aPackage );
 
    private:    // Data
   
        HBufC* iPhoneNumber;
         
        CAiwDialData::TCallType iCallType;
        
        HBufC* iName;
        
        TInt iWindowGroup;
        
        TInt iRedial;
        
        TTimeIntervalSeconds iRedialMaximumDuration;
        
        HBufC8* iContactLink;
        
        HBufC* iSubAddress;
       
        TBool iShowNumber;
        
        TBool iAllowMatch;
        
        TBool iSATCall;
        
        TBool iEndOtherCalls;
        
        HBufC8* iBearer;
        
        TBool iInitCall;
        
        /*
         * Used service id.
         */
        TUint32 iServiceId;                 

        HBufC* iUUI;
    };

#endif      // CAIWINTERNALDIALDATA_H   
            
// End of File
