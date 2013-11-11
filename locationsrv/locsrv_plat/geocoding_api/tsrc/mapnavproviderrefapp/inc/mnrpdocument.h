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
* Description:  CMnrpDocument class definition
*
*/


#ifndef MNRP_DOCUMENT_H
#define MNRP_DOCUMENT_H

#include <akndoc.h>

class CMnrpEngine;
class CEikApplication;

/** Document class of MnRefProvider component.
 *	It creates and owns Engine instance
 */
class CMnrpDocument : public CAknDocument
    {
    public:

        static CMnrpDocument* NewL( CEikApplication& aApp );
        ~CMnrpDocument();

		/** Returns reference to Engine instance */        
        CMnrpEngine& Engine();

    public: // from CAknDocument

        CEikAppUi* CreateAppUiL();

    private:

        CMnrpDocument( CEikApplication& aApp );
        void ConstructL();
    
    private:
        CMnrpEngine* iEngine;        
    };

#endif // MNRP_DOCUMENT_H

