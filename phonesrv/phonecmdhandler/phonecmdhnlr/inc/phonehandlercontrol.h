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
* Description: Declaration of CPhoneHandlerControl class.
*
*/


#ifndef CPHONEHANDLERCONTROL_H
#define CPHONEHANDLERCONTROL_H

//  INCLUDES
#include "phonehandlerservice.h" 
#include <RemConExtensionApi.h> 
#include <RemConCallHandlingTargetObserver.h> 
#include <e32base.h>
#include <phclttypes.h> 
#include <e32property.h> 

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CRemConInterfaceSelector;
class CRemConCallHandlingTarget;
class CPhoneHandlerResponse;
class CPhoneHandlerCallState;

/**
* Receives call handling related key presses from accessories and executes 
* them.
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerControl ) : public CBase,
                                            public MRemConCallHandlingTargetObserver
    {
    public:  // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
        static CPhoneHandlerControl* NewL( 
            CRemConInterfaceSelector* aIfSelector = NULL );

        /**
        * Destructor.
        */
        virtual ~CPhoneHandlerControl();

    public: // New functions
    
        /**
        * Returns phone number for dial or speed dial.
        * @since S60 3.1
        * @param void
        * @return TPhCltTelephoneNumber phone number
        */
        const TPhCltTelephoneNumber& TelephoneNumber() const;
        
        /**
        * Returns voice dial status for voice dial.
        * @since S60 3.1
        * @param void
        * @return TBool status
        */
        const TBool& VoiceDialStatus() const;
        
        /**
        * Returns CHLD command for multiparty call.
        * @since S60 3.1
        * @param void
        * @return TInt CHLD command
        */
        const TInt& ChldCommand() const;
        
        /**
        * Returns CHLD call number for multiparty call.
        * @since S60 3.1
        * @param void
        * @return TInt CHLD command number
        */
        const TInt& ChldCallNumber() const;
        
        /**
        * Returns DTMF tone to be sent.
        * @since S60 3.1
        * @param void
        * @return TChar DTMF tone
        */
        const TChar& Tone() const;
        
        /**
        * Returns reference to API used to communicate with accessory who 
        * initiated key press.
        * @since S60 3.1
        * @param void
        * @return CRemConCallHandlingTarget& reference to API
        */  
        CRemConCallHandlingTarget& CommandInitiator() const;
        
        /**
        * Informs the latest call state.
        * @since S60 3.1
        * @param aState Call state from KTelephonyCallState P&S key
        * @return void
        */
        void NotifyCallState( const TInt aState );

        /**
        * Get iSwitchCall flag, if this flag is ETrue, it means
        * "Send" key in remote target is pressed in multiparty call,
        * otherwise EFalse
        * @since S60 3.1
        * @return TBool
        */
        TBool SwitchCall();

                    
    public: // Functions from base classes
                
    protected:  // New functions
        
    protected:  // Functions from base classes
    
    private:

        /**
        * C++ default constructor.
        */
        CPhoneHandlerControl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( CRemConInterfaceSelector* aIfSelector = NULL );
                
        /**
        * From MRemConCallHandlingTargetObserver. Answer an incoming phone call.
        */
        virtual void AnswerCall();

        /**
        * From MRemConCallHandlingTargetObserver. End an incoming/ongoing 
        * phone call.
        */
        virtual void EndCall();
    
        /**
        * From MRemConCallHandlingTargetObserver. Answer an incoming phone call, 
        * or end an ongoing call according to call status. 
        */
        virtual void AnswerEndCall();
        
        /**
        * From MRemConCallHandlingTargetObserver. Make a voice call.
        */
        virtual void VoiceDial( const TBool aActivate );
    
        /**
        * From MRemConCallHandlingTargetObserver. Redial last phone call.
        */
        virtual void LastNumberRedial();
    
        /**
        * From MRemConCallHandlingTargetObserver. Dial a phone call.
        */
        virtual void DialCall( const TDesC8& aTelNumber );
    
        /**
        * From MRemConCallHandlingTargetObserver. Make multiparty call.
        */
        virtual void MultipartyCalling( const TDesC8& aData );
    
        /**
        * Generates DTMF signal.
        */
        virtual void GenerateDTMF( const TChar aChar );
    
        /**
        * From MRemConCallHandlingTargetObserver. Make a speed dial call.
        */
        virtual void SpeedDial( const TInt aIndex );
        
        /**
        * Starts processing service corresponding to key press.
        */
        void StartProcessing( 
            const TRemConExtCallHandlingApiOperationId aOperation );
        
        /**
        * Creates service.
        */
        MPhoneHandlerService* CreateServiceL( 
            const TRemConExtCallHandlingApiOperationId aOperation );
        
        /**
        * Returns call status.
        */
        void CallStatusL( RPhone::TLineInfo& aLineInfo );
        
                
    private:    // Data
        
        // Provides e.g. service to listen accessory key presses.
        CRemConInterfaceSelector* iInterfaceSelector;   // owned
        
        //  API notifying PhoneCmdHandler about call handling key presses.
        CRemConCallHandlingTarget* iTarget; // owned
                
        // Phone number for dial call/speed dial.
        TPhCltTelephoneNumber iTelNumber;
        
        // Voice recognition status
        TBool iActivate;
        
        // Multiparty call command
        TInt iChldCommand;
        
        // Multiparty call number
        TInt iChldCallNumber;
        
        // Type of DTMF tone
        TChar iChar;
        
        //  Speed dial index.
        TInt iIndex;
                
        // Sends responses to RemCon FW
        CPhoneHandlerResponse* iResponse;
        
        // Interface to P&S key that returns call state
        RProperty iProperty;
        
        // Previous call state
        TInt iPrevState;
        
        // Number of active calls
        // Used to determine whether there's a multicall case when call state
        TInt iActiveCalls;
        
        // switch phonecall by press "Send" key
        TBool iSwitchCall;
        
        // Listens to phone call state changes.
        CPhoneHandlerCallState* iCallStateObserver;
        
                    
    public:     // Friend classes
    
    protected:  // Friend classes
    
    private:    // Friend classes
    
    };

#endif      // CPHONEHANDLERCONTROL_H   
            
// End of File
