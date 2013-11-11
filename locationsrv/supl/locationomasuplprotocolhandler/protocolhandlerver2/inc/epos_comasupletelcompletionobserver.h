/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Active object wrapper for ETEL Notifier 
*
*/


#ifndef C_COMASUPLSETTINGSETELOBSERVER_H
#define C_COMASUPLSETTINGSETELOBSERVER_H

/**
* This class provides a callback interface for handling the
* completion of ETEL events.  The Client derives a
* class from this interface and implements the methods
* that interest it.
*/
class MOMASuplEtelCompletionObserver
    {

    public:
		virtual void HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw) = 0;

		virtual void HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw) = 0;
   
    };
#endif // C_COMASUPLSETTINGSETELOBSERVER_H

