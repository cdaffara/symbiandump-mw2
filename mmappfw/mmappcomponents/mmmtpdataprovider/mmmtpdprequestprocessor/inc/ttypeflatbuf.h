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
* Description:  GetParitialObject operation helper function
*
*/


#ifndef TMTPTYPEFLATBUF_H
#define TMTPTYPEFLATBUF_H

#include <mtp/tmtptypeflatbase.h>

/**
* Defines the MTP flat data types
*/
class TMTPTypeFlatBuf: public TMTPTypeFlatBase
    {    
public:
    /**
    * Standard c++ constructor
    */
    TMTPTypeFlatBuf(const TDes8& aBuffer);
    
private:
    // from TMTPTypeFlatBase
    /**
    * Override to provide element info 
    */
    const TElementInfo& ElementInfo(TInt aElementId) const;
    
    };

#endif //TMTPTYPEFLATBUF_H
