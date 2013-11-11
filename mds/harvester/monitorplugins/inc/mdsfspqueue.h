/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Queue/cache item for file server plugin events*
*/


#ifndef __MDSFSPQUEUE_H__
#define __MDSFSPQUEUE_H__

#include <e32base.h>
#include "mdsfileserverpluginclient.h"

class CMdsFSPQueueItem : public CBase
	{
	public: 
		inline static CMdsFSPQueueItem* NewL();
		inline static CMdsFSPQueueItem* NewL( TMdsFSPStatus& aStatus );
		
		inline virtual ~CMdsFSPQueueItem();
		
		inline void GetAsFspStatus( TMdsFSPStatus& aStatus );
		
		inline void SetStatusL( TMdsFSPStatus& aStatus );
				
	public:	
		TInt iFileEventType;
		TInt iDriveNumber;
		HBufC* iFileName;
		HBufC* iNewFileName;
        TUid iProcessId;
        TUint32 iDriveMediaId;
	};
	
#include "mdsfspqueue.inl"

#endif /*__MDSFSPQUEUE_H__*/
