/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declarations for cce event storage.
*
*/

#ifndef CEVENTSTORAGE_H
#define CEVENTSTORAGE_H

// Includes

class MCCECall;

class TEvent
	{
	public:
	
		TInt iCallID;
		TInt iEventType;
		
		/* Event specific. 
		 * e.g. ECallObserverCallStateChanged, ECallObserverCallEventOccurred: see mccecallobserver.h   
         *      EConferenceCallStateChanged/EConferenceCallEvent: see mcceconferencecallobserver.h
		 */ 
		TInt iEventID;
		TInt iParam1;
		TInt iParam2;
		
		TBool operator==(const TEvent aEvent) const;
	
	};

NONSHARABLE_CLASS(CEventStorage) : public CBase
	{
	
	public:
	
		enum EObserverEventType
			{
			EObserverIncomingCall = 1,
			EObserverMOCallCreated = 2,
			EObserverConferenceCallCreated = 3,
			
			// Call observer
			ECallObserverCallStateChanged = 10,
			ECallObserverCallEventOccurred = 11,
			ECallObserverCallStateChangedWithInband = 12,
			ECallObserverErrorOccurred = 13,
			ECallObserverCallCapsChanged = 14,
			ECallObserverCallDurationChanged = 15,
			ECallObserverHandleTransfer = 16,
			ECallObserverPlayInbandTone = 17,
			
			// Conference call
			EConferenceCallStateChanged = 20,
			EConferenceCallEvent = 21,
			EConferenceCallCapsChanged = 22,
			EConferenceCallErrorOccurred = 23,
			
			EDtmfEvent = 30,
			
			// ext if obsever
			EHandleExtensionEvents = 40
		};
	
		static CEventStorage* NewL();
		virtual ~CEventStorage();
		
	    void StoreEvent( TEvent aEvent );
        void GetEvent();
		
	private:
		
        CEventStorage();
        void ConstructL();
        void SetIncomingCall( MCCECall& aIncomingCall );
	
	public: // Data members
		
        RArray <TEvent> iReceivedEvents;
        
	private: //data
	    
	    CActiveSchedulerWait* iWait;
	    TInt iMessageQueueCount;
	
	};
	
#endif //CEVENTSTORAGE_H