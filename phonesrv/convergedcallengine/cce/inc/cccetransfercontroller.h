/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains transfer handling logic
*
*/


#ifndef CCCETRANSFERCONTROLLER_H
#define CCCETRANSFERCONTROLLER_H

#include <e32base.h>

#include "ccpdefs.h" // TCCPError defs

#include "mccecallobserver.h"
#include "mccpcallobserver.h"

class CCCECallContainer;
class MCCPCall;
class MCCECall;
class CCCECall;

/**
 *  Contains transfer controlling logic
 *
 *  
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCETransferController) : public CBase,
                                            public MCCPCallObserver
    {
public:
    
    /** Defines all possible transfer types */
    enum TTransferType
        {
        ECCETypeNone,
        ECCETypeAttendedMT,
        ECCETypeAttendedMO,
        ECCETypeUnattended
        };
        
public:
    /**
     * Two-phased constructor
     * @param aCallContainer Reference to callcontainer object
     */
    static CCCETransferController* NewL( CCCECallContainer& aCallContainer );

    /**
     * Destructor.
     */
    virtual ~CCCETransferController();


// from base class MCCPCallObserver
public:
    /**
    * An error has occurred concerning a specific call.
    * @since S60 3.2
    * @param aError Error code.
    * @param aCall Current MCCPCall.
    * @return none
    */
    void ErrorOccurred( const TCCPError aError, MCCPCall* aCall ); 
      
    /**
    * The state of the call has changed.
    * @since S60 3.2
    * @param aState Call state.
    * @return none
    */
    void CallStateChanged( const MCCPCallObserver::TCCPCallState aState,
                           MCCPCall* aCall); 

    /**
    * The state of the call has changed.
    * @since S60 3.2
    * @param aState Call state.
    * @param aCall Current MCCPCall.
    * @return none
    */
    void CallStateChangedWithInband( TCCPCallState aState, MCCPCall* aCall ); 

    /**
    * An event has occurred concerning a specific call.
    * @since S60 3.2
    * @param aEvent Occurred event.
    * @param aCall Current MCCPCall.
    * @return none
    */
    void CallEventOccurred( const MCCPCallObserver::TCCPCallEvent aEvent, 
                            MCCPCall* aCall ); 

    /**
    * NOTE: change to:
    * virtual void CallCapsChanged( const MCCPCallObserver::TCCPCallControlCaps aCapsFlags ) = 0;
    * Notifies changed call capabilities
    * @since S60 3.2
    * @param aCaps New capabilities for the call
    * @param aCall Current MCCPCall.
    * @return none
    */
    void CallCapsChanged( const TUint32 aCapsFlags, MCCPCall* aCall ); 
    
public: // New methods
    
    /**
     * Handles transfer request coming from remote end
     * @since S60 3.2
     * @param aNewTransferCall New call which is used in transfer
     * @param aOriginator Call to be transferred
     * @param aAttended ETrue Attended EFalse Unattended
     * @return none
     */
    void HandleRemoteTransferRequest( MCCPCall* aNewTransferCall,
                                      MCCPCall* aOriginator, 
                                      TBool aAttented );
     
    /**
     * Handles incoming attended transfer call which replaces other call
     * @since S60 3.2
     * @param aNewTransferCall Incoming transfer call
     * @param aOriginator Call to be transferred (replaced)
     * @return none
     */
    void TransferCallIncoming( MCCPCall* aNewTransferCall,
                               MCCPCall* aOriginator );
    
    /**
     * Releases transfer call 
     * @since S60 3.2
     * @return none
     */
    void ReleaseTransferCall();
    
    /**
     * Returns ongoing transfer type 
     * @since S60 3.2
     * @return Ongoing transfer type
     */
    TTransferType OngoingTransferType() const;
    
    /**
     * Returns pointer to transfer call 
     * @since S60 3.2
     * @return Transfer call pointer
     */
    MCCPCall* TransferCall() const;
    
    /**
     * Returns pointer to call to be transferred 
     * @since S60 3.2
     * @return Original call pointer
     */
    MCCPCall* OriginatorCall() const;
    
    /**
     * Sets transfer accepted flag 
     * @since S60 3.2
     * @param aAccepted ETrue if user accepted unattended transfer
     * otherwise EFalse 
     * @return None
     */
    void SetTransferAccepted( TBool aAccepted );
    
    /**
     * Returns transfer accepted flag 
     * @since S60 3.2
     * @return Transfer accepted flag
     */
    TBool TransferAccepted() const;
    
    /**
     * Set flag indicating that transfer dial is pending 
     * @since S60 3.2
     * @return None
     */
    void SetTransferDialPending( TBool aDialPending );
    
    /**
     * Handles call state changes of transfer call and originator call 
     * @since S60 3.2
     * @param aCall Points to call which received status change
     * @param aStatus New status of the call
     * @return None
     */
    void HandleCallStateChanged( CCCECall* aCall,
            CCPCall::TCallState aStatus );
    
private:
    
    /** Constructor */
    CCCETransferController( CCCECallContainer& aCallContainer );
        
    /**
     * Resets transfer controller to its initial state
     * @return None
     */
    void Reset();
   
private:
    /**
     * Reference to call container 
     */
    CCCECallContainer& iCallContainer;
    
    /**
     * Ongoing transfer type 
     */
    TTransferType iOngoingTransferType;
    
    /**
     * CCE Call Id of the originator call 
     */
    TInt iOriginatorCallId;
    
    /**
     * Pointer to transfer call 
     * Not own
     */
    MCCPCall* iTransferCall;
    
    /**
     * Pointer to call to be transferred 
     * Not own
     */
    MCCPCall* iOriginatorCall;
    
    /**
     * Flag indicating transfer acception state 
     */
    TBool iTransferAccepted;
    
    /**
     * Flag indicating pending transfer dial 
     */
    TBool iTransferDialPending;
    
    /**
     * Pointer of a call which is currently being observed for call state
     * changes
     */
    MCCPCall* iObservedCall;
    };
    
    
#endif
