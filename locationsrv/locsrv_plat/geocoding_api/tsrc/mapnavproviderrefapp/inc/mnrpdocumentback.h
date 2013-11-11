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
* Description:  CMnrpDocumentBack class definition
*
*/


#ifndef MNRP_DOCUMENTGEO_H
#define MNRP_DOCUMENTGEO_H

#include <akndoc.h>

class CEikApplication;

/** Document class of MnRefGeocoder component */
class CMnrpDocumentBack : public CAknDocument
    {
    public:

        static CMnrpDocumentBack* NewL( CEikApplication& aApp );

    public: // from CAknDocument

        CEikAppUi* CreateAppUiL();

    private:

        CMnrpDocumentBack( CEikApplication& aApp );
    };

#endif // MNRP_DOCUMENTGEO_H

