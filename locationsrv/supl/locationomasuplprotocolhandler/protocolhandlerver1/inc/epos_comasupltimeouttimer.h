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
* Description:   General purpose class for timeout notification.
*
*/



#ifndef C_COMASUPLTIMEOUTTIMER_H
#define C_COMASUPLTIMEOUTTIMER_H

#include <e32base.h>

class MOMASuplTimeOutNotifier;

enum TTimerPanicCode
{
	ESuplTimeoutTimerFailed
};

/**
*  This class will notify an object after a specified timeout.
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/

class COMASuplTimeoutTimer : public CTimer
    {
public:
			/**
			*NewL,Create a COMASuplTimeoutTimer object
			*@since S60 v3.1u
			*@param aTimeOutNotify object to notify of timeout event
			*@return a pointer to the created instance of COMASuplTimeoutTimer
			*/
			static COMASuplTimeoutTimer* NewL(MOMASuplTimeOutNotifier& aTimeOutNotify);
			
			/**
			*NewLC
			*Create a COMASuplTimeoutTimer object
			*@since S60 v3.1u
			*@param aTimeOutNotify object to notify of timeout event
			*@return a pointer to the created instance of COMASuplTimeoutTimer
			*/
			static COMASuplTimeoutTimer* NewLC(MOMASuplTimeOutNotifier& aTimeOutNotify);
			
			/**
			*~COMASuplTimeoutTimer,Destroy the object and release all memory objects
			*@since S60 v3.1u
			*@param None
			*@return None
			*/
			~COMASuplTimeoutTimer();

protected: // From CTimer

			/**
			*RunL,Invoked when a timeout occurs
			*@since S60 v3.1u
			*@param None
			*@return None
			*/
			virtual void RunL();

public:

			/**
			*StartTimer,Starts the timer with specified timeout value
			*@since S60 v3.1u
			*@param aTimeOut 
			*@return None
			*/
			void StartTimer( const TInt aTimeOut);
			
			/**
			*StopTimer,Cancels  the timer started timer
			*@since S60 v3.1u
			*@param None 
			*@return None
			*/
			void StopTimer();
			
			/**
			*IsTimerExpired,Checks if Timer expired on last call
			*@since S60 v3.1u
			*@param TBool, ETrue if it's expired else EFalse
			*@return None
			*/
			TBool IsTimerExpired();

private:

			/**
			*COMASuplTimeoutTimer,Perform the first phase of two phase construction 
			*@since S60 v3.1u
			*@param aTimeOutNotify An observer to notify
			*/
			COMASuplTimeoutTimer(MOMASuplTimeOutNotifier& aTimeOutNotify);
			
			/**
			*ConstructL,Perform the second phase construction of a COMASuplTimeoutTimer 
			*@since S60 v3.1u
			*@param None
			*@return None
			*/
			void ConstructL();

private: // Member variables

			/** The observer for this objects events */
			MOMASuplTimeOutNotifier& iNotify;
			
			//Is TimerExpired on last call
			TBool iTimerExpired;
    
    };

#endif // C_COMASuplTimeoutTimer_H


