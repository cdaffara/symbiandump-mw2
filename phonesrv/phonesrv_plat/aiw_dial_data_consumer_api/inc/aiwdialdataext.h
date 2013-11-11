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


#ifndef CAIWDIALDATAEXT_H
#define CAIWDIALDATAEXT_H

//  INCLUDES
#include <s32mem.h>
#include "aiwdialdata.h"

/** Constant used by aiwdialdataext interface. */ 
namespace AIWDialDataExt
    {
    /** Maximum name length. */ 
    const TInt KMaximumNameLength = 50;
    
    /** Maximum phone number length same as used by phone. */ 
    const TInt KMaximumPhoneNumberLength = 100;
    
    /**  Constants used in Redial parameter. */
    const TInt KAIWRedialDefault = -1;          // Default redial.
    const TInt KAIWRedialOff = -2;              // Redial is off.
    const TInt KAIWRedialWithDefaultTime = -3;  // Redial on, with default time.
    const TInt KAIWRedial = 0;                  // Forced redial.
    }

// FORWARD DECLARATIONS
class CAiwGenericParamList;
class CAiwInternalDialData;

// CLASS DECLARATION

/**
*  Dial data for external applications.
* 
*  @lib aiwdialdata.lib
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CAiwDialDataExt ): public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor for implementation class.
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwDialDataExt* NewL();
       
        /**
        * Two-phased constructor for implementation class. Leaves the 
        * pointer to the cleanup stack.
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwDialDataExt* NewLC();
        
        /**
        * Destructor.
        */
        virtual ~CAiwDialDataExt();
    
    public: // New
    
        /**
        * From CAiwDialData: Composes data set package and inserts 
        * data to aInParamList.
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
        * Sets the window group identifier.
        * When call is ended, set window group is actived.
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
        * Set contact link. If phone number and contact link are both 
        * defined call will be made to contact link.
        *
        * @since S60 3.2
        * @param aContactLink Contact link information.
        *        Default: Empty.
        */
        IMPORT_C void SetContactLinkL( const TDesC8& aContactLink );

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
        * Contact link field.
        *
        * @since S60 3.2
        * @return Returns contack link item.
        */
        IMPORT_C const TDesC8& ContactLink() const;
   

        /**        
        * Set id of used call service.
        *
        * @param aServiceId The value of Service Id.
        */
        IMPORT_C void SetServiceId( TUint32 aServiceId );

        /** 
        * Service Id.
        *
        * @return Return service id.
        */
        IMPORT_C TUint32 ServiceId() const;

    private:

        /**
        * C++ default constructor.
        */
        CAiwDialDataExt();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:
        
        CAiwInternalDialData* iInternalDialData;   
    };

#endif      // CAIWDIALDATAEXT_H   
            
// End of File
