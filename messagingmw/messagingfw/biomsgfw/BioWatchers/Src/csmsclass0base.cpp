// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <csmsclass0base.h>

/**
It is a default implementation, this needs to be overridden by derived class.
@param aSmsMessage will contains Starting & Ending PDU position of decoded message,
and boolean value indicates that, is this last incomplete message. And it contains Class0SMS data.
This needs be Internalize at client side in same order (i.e TInt aStartPos, TInt endPos, TBool aIsLastMessage, TPtr aSmsData).
@param aComplete indicates, is it a partial messsage or full message. 
*/

void CSmsClass0Base::DisplayMessageHandler(TDesC8& /*aSmsMessage*/, TBool /*aComplete*/)
	{
	//nothing to do here
	}

//constructor.
CSmsClass0Base::CSmsClass0Base()
	{
	//nothing to do here
	}

//destructor
CSmsClass0Base::~CSmsClass0Base()
	{
	//nothing to do here
	}
