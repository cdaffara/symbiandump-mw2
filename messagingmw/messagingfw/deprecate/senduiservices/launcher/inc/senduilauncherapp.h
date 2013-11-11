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
* Description:   Server application for launching SendUi with low 
*                capability requirements.
*
*/




#ifndef C_CSENDUILAUNCHERAPP_H
#define C_CSENDUILAUNCHERAPP_H


#include <aknapp.h>
#include <AknServerApp.h>


/**
 *  Returns a new application server and a document
 *
 *  @since S60 v.3.2
 */
class CSendUiLauncherApplication : public CAknApplication
    {

public:

    TUid AppDllUid() const;

// from base CAknApplication

protected:

    /**
     * Creates document
     *
     * @since S60 v.3.2
     */
    CApaDocument* CreateDocumentL();

    /**
     * Return new Application Server
     *
     */
    void NewAppServerL( CApaAppServer* & aAppServer);

    };


#endif // C_SENDUILAUNCHERAPP_H
