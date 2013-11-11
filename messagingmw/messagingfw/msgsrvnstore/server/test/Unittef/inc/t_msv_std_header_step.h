//
// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef T_MSV_STD_HEADER_H
#define T_MSV_STD_HEADER_H

#include <msvapi.h>



class CTestSessionObserver:public CBase, public MMsvSessionObserver
	{
public:
	inline virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*);

	TMsvSessionEvent iType;
	TInt iArg1;
	TInt iArg2;
	};





class CTestEntryObserver:public CBase, public MMsvEntryObserver
	{
public:
	inline virtual void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*);
	
	TMsvEntryEvent iType;
	TInt iArg1;
	TInt iArg2;
	};
	
	


void CTestSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
		{
		iType = aEvent;
		if (aArg1)
			{
			if(iType == MMsvSessionObserver::EMsvEntriesCreated)
				{
				CMsvEntrySelection* selection = (CMsvEntrySelection*) aArg1;
				iArg1 = selection->At(0);
				}
			else
				{
				iArg1 = *(TInt*)aArg1;
				}
			}			
		else
			{
			iArg1 = NULL;
			}
			
		if (aArg2)
			{
			iArg2 = *(TInt*)aArg2;
			}			
		else
			{
			iArg2 = NULL;
			}			
		
		CActiveScheduler::Stop();
		}



void CTestEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
		{
		iType = aEvent;
		if (aArg1)
			iArg1 = *(TInt*)aArg1;
		else
			iArg1 = NULL;
		
		if (aArg2)
			iArg2 = *(TInt*)aArg2;
		else
			iArg2 = NULL;
		}

	
#endif		// T_MSV_STD_HEADER_H
