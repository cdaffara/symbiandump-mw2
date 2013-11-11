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
* Description:  CMnrpApplication class definition 
*
*/


#ifndef MNRP_APPLICATION_H
#define MNRP_APPLICATION_H

#include <eikapp.h>

/**
 *  An instance of CMnrpApplication is the application part of the Eikon
 *  application framework for the MnRefProvider.
 *	MnRefProvider is an Application Server, which implements MapView and Navigation
 *	services of M&N FW.
 */
class CMnrpApplication : public CEikApplication
    {
    public:  // from CEikApplication

        /** 
        * AppDllUid returns the application DLL UID value.
        *
        *  @return the UID of this Application/Dll
        */
        TUid AppDllUid() const;

    protected: // from CEikApplication

        /** 
        * CreateDocumentL creates a CApaDocument object and return a pointer to it
        *
        * @return a pointer to the created document
        */
        CApaDocument* CreateDocumentL();
        
        void NewAppServerL(CApaAppServer *&aAppServer);

    };

#endif // MNRP_APPLICATION_H

