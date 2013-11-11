/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Resolves sending service information and stores it to 
*                Central Repository.    
*
*/



#ifndef C_CSENDUISRAPP_H
#define C_CSENDUISRAPP_H

#include <aknapp.h>

/**
 *  Application class for SendUiServiceResolver application
 *
 *  @since S60 S60 v3.2
 */
class CSendUiSrApp : public CAknApplication
    {
public:  // from base class CAknApplication

    /**
     * From CAknApplication.
     * Return unique application identifier
     *
     * @since S60 v3.2
     */
    TUid AppDllUid() const;

protected: // from base class CAknApplication
    /**
     * From CAknApplication.
     * Creates document
     *
     * @since S60 v3.2
     */
    CApaDocument* CreateDocumentL();

    };

#endif // C_SENDUISRAPP_H
