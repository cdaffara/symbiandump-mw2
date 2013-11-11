/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/

#ifndef CMDETESTSCRIPTERAO_H
#define CMDETESTSCRIPTERAO_H

#include <e32base.h>
#include <StifTestEventInterface.h>
#include <TestScripterInternal.h>

class CMdETestScripter;

NONSHARABLE_CLASS(CMdETestScripterAO) : public CActive
    {
public:

    ~CMdETestScripterAO();

    static CMdETestScripterAO* NewL();

    static CMdETestScripterAO* NewLC();

public:
    void Start( CMdETestScripter* aScripter, TEventIf aEvent );

private:
    CMdETestScripterAO();

    void ConstructL();

private:
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);

private:
	CMdETestScripter* iScripter;
    TEventIf iEvent;
    };

#endif // CMDETESTSCRIPTERAO_H
