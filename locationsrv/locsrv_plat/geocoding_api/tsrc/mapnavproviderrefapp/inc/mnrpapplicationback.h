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
* Description:  CMnrpApplicationBack class definition
*
*/


#ifndef MNRP_APPLICATIONBACK_H
#define MNRP_APPLICATIONBACK_H

#include <eikapp.h>

/**
 *  An instance of CMnrpApplicationBack is the application part of the Eikon
 *  application framework for the MnRefGeocoder.
 *	MnRefGeocoder is a hidden application and works only as geocoder server.
 *	It accepts geocoding requests from M&N FW, serves them and exits, when 
 *	all clients are disconnected.
 */
class CMnrpApplicationBack : public CEikApplication
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

#endif // MNRP_APPLICATIONBACK_H

