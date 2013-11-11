/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Rename objects
*
*/


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERRENAMEOBJECT_H
#define CABSTRACTMEDIAMTPDATAPROVIDERRENAMEOBJECT_H

#include "crenameobject.h"

class CAbstractMediaMtpDataProviderRenameObject: public CRenameObject
    {
public:
    /**
    * Two phase constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aWrapper Reference to CMmMtpDpMetadataAccessWrapper
    * @param aDummyArray Reference to abstract media dummy files array
    * @return A pointer to a new instance of the object
    */
    static CAbstractMediaMtpDataProviderRenameObject* NewL( MMTPDataProviderFramework& aFramework,
        CMmMtpDpMetadataAccessWrapper& aWrapper );

    /**
    * destructor
    */
    ~CAbstractMediaMtpDataProviderRenameObject();

protected:
    // from CRenameObject
    void PerformAdditionalActionL();

private:
    /**
    * Standard C++ Constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aWrapper Reference to CMmMtpDpMetadataAccessWrapper
    * @param aDummyArray Reference to abstract media dummy files array
    */
    CAbstractMediaMtpDataProviderRenameObject( MMTPDataProviderFramework& aFramework,
        CMmMtpDpMetadataAccessWrapper& aWrapper );

    void ConstructL();
    
    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERRENAMEOBJECT_H
