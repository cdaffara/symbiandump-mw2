/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common utility methods.
*
*/


#ifndef CLFUTILS_H
#define CLFUTILS_H

namespace CLFUtils
    {
    
    /**
     * Maps CLF mediatype to MdE object class
     *
     */
    const TDesC& MapClfType( TInt aType );
    }

#endif // CLFUTILS_H
