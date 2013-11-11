/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*/

#include <e32base.h>

#ifndef __MDSOBJECTLOCKLIST_H__
#define __MDSDATABASEUPDATER_H__

class CMdSDatabaseUpdater : public CBase
    {
    public: // Constructors and destructor

        virtual ~CMdSDatabaseUpdater();

        static CMdSDatabaseUpdater* NewL();

    private: // Private constructors

        void ConstructL();

    public: // Public functions

        TBool UpdateDatabaseL( TInt64 aMinorVersion );

    private:

    };

#endif  // __MDSDATABASEUPDATER_H__
