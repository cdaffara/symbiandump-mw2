/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  EdwinTestContro.h
*
*/

// Version : %version: 3.1.1 %



#ifndef EDWINTESTCONTROL_H_
#define EDWINTESTCONTROL_H_

#include <eikedwin.h>

class CAknsBasicBackgroundControlContext;

class CEdwinTestControl : public CCoeControl, public MCoeControlObserver
    {
public:
	static CEdwinTestControl* NewL(void);
	virtual ~CEdwinTestControl();
protected:
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId); // 
private:
	virtual void SizeChanged();
	virtual void HandleResourceChange(TInt aType);
	virtual TInt CountComponentControls() const;
	virtual CCoeControl* ComponentControl(TInt aIndex) const;
    void ConstructL(void);
	void Draw(const TRect& aRect) const;
	
	void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType);
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType );	
private:
	CAknsBasicBackgroundControlContext*	iBgContext; 
	CEikEdwin* iEditWin;
};


#endif /*EDWINTESTCONTROL_H_*/
