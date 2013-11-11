/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester server's P&S pause observer active object
*
*/


#ifndef PAUSEOBSERVERAO_H_
#define PAUSEOBSERVERAO_H_

#include <e32base.h>
#include <e32property.h>

class CHarvesterServer;

class CPauseObserverAO : public CActive
	{	
	public:
		static CPauseObserverAO* NewL( CHarvesterServer& aHarvesterServer );
		virtual ~CPauseObserverAO();
	
	private:
		CPauseObserverAO( CHarvesterServer& aHarvesterServer );
		void ConstructL();
		void RunL();
		void DoCancel();
		TInt RunError( TInt aError );
		
	private:
		CHarvesterServer& iHarvesterServer;
		RProperty iPauseProperty;
	};

#endif /*PAUSEOBSERVERAO_H_*/
