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
* Description: AlwaysOnline server signaller
*
*/

class TOAManagerStartupSignaller
	{
///////////////////////////////////////////////////////////////////////////////////////
public:										// ENUMERATIONS
///////////////////////////////////////////////////////////////////////////////////////
	inline TOAManagerStartupSignaller();
	inline TOAManagerStartupSignaller(TRequestStatus& aStatus);

///////////////////////////////////////////////////////////////////////////////////////
public:										// ACCESS
///////////////////////////////////////////////////////////////////////////////////////
	inline TPtrC							AsCommand() const;
	TInt									GetCommand();
	void									SignalL();

///////////////////////////////////////////////////////////////////////////////////////
private:									// MEMBER DATA
///////////////////////////////////////////////////////////////////////////////////////
	TThreadId								iThreadId;
	TRequestStatus*							iStatus;
	};


///////////////////////////////////////////////////////////////////////////////////////
// ----> TOAManagerStartupSignaller (inlines)
///////////////////////////////////////////////////////////////////////////////////////

inline TOAManagerStartupSignaller::TOAManagerStartupSignaller()
	{
	}
inline TOAManagerStartupSignaller::TOAManagerStartupSignaller(TRequestStatus& aStatus)
:	iThreadId(RThread().Id()), iStatus(&aStatus)
	{
	aStatus = KRequestPending;
	}
inline TPtrC TOAManagerStartupSignaller::AsCommand() const
	{
	return TPtrC( reinterpret_cast<const TText*>( this ), 
		sizeof( TOAManagerStartupSignaller ) / sizeof( TText ) );
	}
