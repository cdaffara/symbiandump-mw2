/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Dummy class to get playlist objects initialized
*
*/


#ifndef C_MPXDUMMYPLAYBACKOBSERVER_H
#define C_MPXDUMMYPLAYBACKOBSERVER_H

#include <mpxcollectionobserver.h>

/**
 *  Dummy observer to initialize the collection playlist
 * 
 *  @lib mpxplaybackengine.lib
 *  @since S60 3.1
 */
NONSHARABLE_CLASS( CMPXPlaybackDummyMediaObserver ) : public CBase,
                                                      public MMPXCollectionMediaObserver
    {

public:
    
    /**
    * Constructor
    */
    CMPXPlaybackDummyMediaObserver();
    
    /**
    * Destructor
    */
    ~CMPXPlaybackDummyMediaObserver();
    
protected:
    
    /**
    * From MMPXCollectionMediaObserver
    */
    void HandleCollectionMediaL(const CMPXMedia& aMedia, 
                                TInt aError);    
    };

#endif // C_MPXDUMMYPLAYBACKOBSERVER_H
