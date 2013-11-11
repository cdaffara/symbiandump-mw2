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


#ifndef CAIWDIALDATA_H
#define CAIWDIALDATA_H

//  INCLUDES
#include <e32base.h>
                        
// FORWARD DECLARATIONS
class CAiwGenericParamList;
class CAiwInternalDialData;

/** Constant used by aiwdialdata interface. */ 
namespace AIWDialData
    {
    /** Maximum phone number length same as used by phone. */ 
    const TInt KMaximumPhoneNumberLength = 100;
    
    /** Default windowgroup value */ 
    const TInt KAiwGoToIdle(0);
    }

// CLASS DECLARATION

/**
*   Creating call using CAiwDialData API. 
*
*   @code
*
*   - Client must be Service Handler client. For more information see
*     service handler desing document.
*
*   - Client creates intance of CAiwDialData:
*
*   CAiwDialData* dialData = CAiwDialData::NewLC();
*
*   - Define call parameters(phone number is compulsory parameter ):
*   
*   dialData->SetCallType( CAiwDialData::EAIWVoice );
*   _LIT(phoneNumber, "050123456");
*   dialData->SetPhoneNumberL( phoneNumber );
*   dialData->SetWindowGroup( AIWDialData::KAiwGoToIdle );
*   
*   - Client creates reference to CAiwGenericParamList and fills paramlist:
*
*   CAiwGenericParamList& paramList = iServiceHandler->InParamListL();
*   dialData->FillInParamListL( paramList );
*
*  - Now client can give execute command to service handler.
*
*  iServiceHandler->ExecuteServiceCmdL( KAiwCmdCall, paramList,
*                                 iServiceHandler->OutParamListL(),
*                                 0, NULL );
*  @endcode
*  
*  Dial data for SDK applications.
* 
*  @lib aiwdialdata.lib
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CAiwDialData ): public CBase
    {
    public:  // Enum.
    
        /**  Different type of calls. */ 
        enum TCallType
            {
            /* Voice call. If there is a VoIP service that is registered and set as preferred, the 
             * call is created as VoIP call, otherwise CS call. */
            EAIWVoice           = 0,
            /** Video call.             */
            EAIWVideo           = 1,
            /** Forced video call, creates video call directly without
             *  queries.                */
            EAIWForcedVideo     = 2, 
            /** Voip call.              */
            EAIWVoiP            = 3,
            /* Forced voice call, creates always CS voice call. Use this value if the call type is
             * not allowed to be changed */
            EAIWForcedCS        = 4
            };
            
    public:  // Constructors and destructor
    
        /**
        * Two-phased constructor for implementation class. 
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwDialData* NewL();
        
        /**
        * Two-phased constructor for implementation class. Leaves the 
        * pointer to the cleanup stack.
        * Use this method for creating a instance with null items.
        *
        * @since S60 3.2
        * @return A pointer to the new object.
        */
        IMPORT_C static CAiwDialData* NewLC();
        
        /**
        * Destructor.
        */
        virtual ~CAiwDialData();
        
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
        * Sets the phone number.
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
        IMPORT_C void SetCallType( TCallType aCallType );
   
        /**
        * Sets the window group identifier. When call is ended, 
        * set window group is actived.
        *
        * @since S60 3.2
        * @param aWindowGroup Window group id.
        *        Default: KAiwGoToIdle.
        */
        IMPORT_C void SetWindowGroup( TInt aWindowGroup );
        
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
        IMPORT_C TCallType CallType() const;      
          
        /**
        * Window group id.
        *
        * @since S60 3.2
        * @return Window group id. 
        */     
        IMPORT_C TInt WindowGroup() const;

    private:

        /**
        * C++ default constructor.
        */
        CAiwDialData();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    private:
        
        CAiwInternalDialData* iInternalDialData;   
    };

#endif      // CAIWDIALDATA_H   
            
// End of File
