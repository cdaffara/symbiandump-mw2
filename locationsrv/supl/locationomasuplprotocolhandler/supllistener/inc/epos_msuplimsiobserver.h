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
* Description: Observer class 
*
*/

/*
 * epos_msuplimsiobserver.h
 *
 *  Created on: Dec 17, 2008
 *      Author: vittals
 */

#ifndef EPOS_MSUPLIMSIOBSERVER_H_
#define EPOS_MSUPLIMSIOBSERVER_H_

//  INCLUDES
#include <e32base.h>

// DATA TYPES
// CLASS DECLARATION

class MHandleImsiObserver
    {
        
    public:  // New functions
        
        virtual void HandleImsiObserverCompleteL() = 0;
    };

#endif /* EPOS_MSUPLIMSIOBSERVER_H_ */
