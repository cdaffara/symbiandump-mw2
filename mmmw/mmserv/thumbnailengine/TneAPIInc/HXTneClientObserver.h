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
* Description:   Client Observer for receiving events from TNE server
*
*/



#ifndef  _HXTNE_ClientObserver_H_
#define  _HXTNE_ClientObserver_H_

#include "HXTNEVideoClipInfoImp.h"


class CHXTNEVideoClipInfoImp;

class CTNEClientObserver : public CActive

{

public :

	static CTNEClientObserver* NewL(CHXTNEVideoClipInfoImp *aImp);
	~CTNEClientObserver();
	
	void Activate();
protected:
    virtual void RunL();
	virtual void DoCancel();	    
private :
	CTNEClientObserver(CHXTNEVideoClipInfoImp *aImp);
    CHXTNEVideoClipInfoImp* iImp;  

};

#endif  // _HXTNE_ClientObserver_H_
