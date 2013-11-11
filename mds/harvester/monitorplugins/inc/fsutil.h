/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Some generic utilities.*
*/

#ifndef __FSUTIL_H__
#define __FSUTIL_H__

#include <e32base.h>

class CMdEHarvesterSession;

class FSUtil
    {
	public:
	    
	    static inline TUint32 MediaID( const RFs& aFs, TInt aDrive );
	        
	    static inline TUint32 MediaID( const RFs& aFs, const TDesC& aUri );

		/*
		 * Returns previous media id was inserted to a slot
		 * 
		 * @param aMdeSession	Session for database access
		 * @param aDrive		Drive letter which media id is wanted
		 */
		static inline TUint32 GetPreviousMediaIDL( const CMdEHarvesterSession* aMdeSession, 
												   TChar aDrive );
	    
	private:
	
	    static inline TInt DriveNumber( const TDesC& aUri );
    };

#include "fsutil.inl" // inlined methods

#endif // __FSUTIL_H__

