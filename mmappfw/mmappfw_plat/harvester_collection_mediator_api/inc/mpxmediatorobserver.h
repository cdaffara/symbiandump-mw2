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
* Description:  Mediator observer
*
*/


#ifndef MMPXMEDIATOROBSERVER_H
#define MMPXMEDIATOROBSERVER_H

class CMPXMedia;

/**
 *  Mediator observer to notify of any change in collection id for a file.
 *
 *  @lib mpxcollectionmediator.dll
 */
class MMPXMediatorObserver
    {

public:

    /**
    * Handles a change in the collection ID and 
    * updates the db based on mediator decisions.
	 *  @since S60 v3.2.3
    * @param aMedia Media Property of the item updated.
    * @param aOldPath Old collection id
    */
    virtual void HandleMediatorPathUpdatedL( CMPXMedia*& aMedia,
                                             TUid  aOldPath ) = 0;
    
    };

#endif // MMPXMEDIATOROBSERVER_H
