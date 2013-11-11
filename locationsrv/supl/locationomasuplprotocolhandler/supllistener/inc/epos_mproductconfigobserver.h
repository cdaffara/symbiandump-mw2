/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Observer class to receive notification when product configuration is complete 
*
*/



#ifndef __EPOS_MPRODUCTCONFIGOBSERVER_H__
#define __EPOS_MPRODUCTCONFIGOBSERVER_H__

//  INCLUDES
#include <e32base.h>

// DATA TYPES
// CLASS DECLARATION

class MProductConfigObserver
    {
        
    public:  // New functions
        
        virtual void HandleProductConfigCompleteL() = 0;
    };

#endif      // __EPOS_MPRODUCTCONFIGOBSERVER_H__  
            
// End of File
