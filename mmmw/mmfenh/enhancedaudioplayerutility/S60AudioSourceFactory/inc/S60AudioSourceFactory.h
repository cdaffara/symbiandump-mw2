/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the stream source reader active object class
*
*/

#ifndef __CS60AUDIOSOURCEFACTORY_H
#define __CS60AUDIOSOURCEFACTORY_H

#include <e32std.h>
#include <e32base.h>
#include "MS60AudioStreamSourceObserver.h"

class CS60AudioStreamSource;

class CS60AudioSourceFactory : public CBase
	{
public:
	IMPORT_C static CS60AudioStreamSource* CreateStreamSource(MS60AudioStreamSourceObserver& aObserver,const TDesC8& aMimeType);
	};

#endif //__CS60AUDIOSOURCEFACTORY_H

//  End of File

