// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#ifndef __CMSVCONVERTERWAITER_H__
#define __CMSVCONVERTERWAITER_H__

#include "MSVSERV.H"
#include <f32file.h>
#include "e32std.h"
#include "e32const.h"
#include <e32property.h>


// Constants. These are used in public and subscribe mechanism that is used to handle 
// conversion cancel request
const TUint KMyPropertyName = 0x0003;
const TUid KMyPropertyCat = {0x10034567};


/**
CMsvConverterWaiter
This acts as a waiter class and is responsible for creating, initiating and communicating with the
converter thread. It initiates message store conversion on the specified drive and upon successful 
completion, updates the drive status to supported in preferred drive list. This class provides a
way of asynchronous handling of store conversion.

@internalAll 
@released
*/
NONSHARABLE_CLASS (CMsvConverterWaiter) : public CActive 
	{
	public:
		static CMsvConverterWaiter* InstanceL(CMsvServer* aServer);
		static CMsvConverterWaiter* Instance();
		~CMsvConverterWaiter();
		void StartMessageStoreConversionL(const RMessage2& aMessage,TBool aQueuedRequest);
		void CancelConversionL(const RMessage2& aMessage);
		void GetConversionStatus(const RMessage2& aMessage);
		TBool isRunningMessageStoreConverter();
		void QueueConversionRequestL(const RMessage2& aMessage);
		
	private:
		CMsvConverterWaiter(CMsvServer* aServer);
		TInt ValidateRequestedDriveL(TDriveNumber aDrive);
		TInt ValidateDriveInPreferredDriveListL(TDriveNumber aDrive);
		
		TInt Start();
		void Completed(const TInt aStatus);
		// active object
		void DoCancel();
		void RunL();
		TInt DefineProperty();
		TInt PublishProperty(TInt aValue);
	private:
		
		// conversion queue structure		
		struct TConversionQueue
			{
			TDriveNumber iDriveNumber;
			RMessage2 iMessage;				
			};
	
		static CMsvConverterWaiter* iConverterWaiter;
		RArray<TConversionQueue> iConverterQueue;
		RThread iConverterThread;
		CMsvServer *iServer;
		TDriveNumber iDriveNumber;
		RProperty iProperty;
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)	
	friend class CTestMessageConverterWaiter;
#endif
	};

#endif   //__CMSVCONVERTERWAITER_H__




