// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __MHTTPREQUESTOBSERVER_H__
#define __MHTTPREQUESTOBSERVER_H__

#include <e32std.h>

class MHttpRequestObserver
	{
public:

	virtual void SendRequestDataL(const TDesC8& aData) =0;
	
	virtual void RequestComplete() =0;
	
	virtual void SendingBodyData(TBool aValue) =0;
	};

#endif	//  __MHTTPREQUESTOBSERVER_H__
