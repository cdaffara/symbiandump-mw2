// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Standard example header for EPOC/32 SDK examples running as text console applications
//

#ifndef __CMSTD_H
#define __CMSTD_H

#include <e32base.h>

class TestScheduler : public CActiveScheduler
	{
	public:
	void Error (TInt aError) const;
	};

class CActiveConsole : public CActive
	{
public:
	  // Construction
	static CActiveConsole* NewLC( CConsoleBase* aConsole );
	void ConstructL();

	  // Destruction
	~CActiveConsole();

	  // Issue request
	void RequestCharacter();
	
	  // Cancel request.
	  // Defined as pure virtual by CActive;
	  // implementation provided by this class.
	void DoCancel();

	  // Service completed request.
	  // Defined as pure virtual by CActive;
	  // implementation provided by this class,
	void RunL();

	  // Called from RunL() - an implementation must be provided
	  // by derived classes to handle the completed request
	virtual void ProcessKeyPress(TChar /*aChar*/) {}; 
	  
protected:
	  // Data members defined by this class
	CConsoleBase* iConsole; // A console for reading from
private:
	CActiveConsole( CConsoleBase* aConsole );
	};

#endif
