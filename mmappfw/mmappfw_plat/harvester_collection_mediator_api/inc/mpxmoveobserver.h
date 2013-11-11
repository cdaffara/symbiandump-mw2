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
* Description:  Move helper observer
*
*/


#ifndef MMPXMOVEOBSERVER_H
#define MMPXMOVEOBSERVER_H

/**
* Observer to call back for move events.
* @lib mpxcollectionmediator.dll
*/
class MMPXMoveItemObserver 
    {
public:
    /**
    * Handles the completion of the move event, just a proxy.
    	*@since 3.2.3
    * @param aErr error code for the operation
    */
    virtual void HandleMoveCompleteL( TInt aErr ) = 0;
    };

#endif // MMPXMOVEOBSERVER_H
