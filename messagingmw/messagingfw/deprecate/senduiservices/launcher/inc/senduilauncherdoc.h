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



#ifndef C_SENDUILAUNCHERDOC_H
#define C_SENDUILAUNCHERDOC_H


#include <akndoc.h>


class CSendUiLauncherAppUi;
class CEikApplication;

/**
 *  Document for SendUiLauncher
 *
 *  @since S60 v.3.2
 */
class CSendUiLauncherDocument : public CAknDocument
    {

public:

    static CSendUiLauncherDocument* NewL(CEikApplication& aApp);

    static CSendUiLauncherDocument* NewLC(CEikApplication& aApp);

    virtual ~CSendUiLauncherDocument();


    /**
     * Create a CSendUiServerAppUi object and return a pointer to it
     *
     * @since S60 v3.2
     * @return a pointer to the created instance of the AppUi created
     */
     CEikAppUi* CreateAppUiL();
     
private:

    CSendUiLauncherDocument(CEikApplication& aApp);

    };
#endif // C_SENDUILAUNCHERDOC_H
