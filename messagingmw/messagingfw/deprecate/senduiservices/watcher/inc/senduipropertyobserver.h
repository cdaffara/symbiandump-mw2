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
* Description:   Launches SendUi Service Resolver in the event of:
*                - refresh request.
*                - MTM amount has changed since previous boot
*                - Phone language has change since previous boot*
*/



#ifndef C_CSENDUIPROPERTYOBSERVER_H
#define C_CSENDUIPROPERTYOBSERVER_H

#include <e32base.h>
#include <e32property.h>

class CSendUiWatcher;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSendUiPropertyObserver : public CActive
    {

public:

    static CSendUiPropertyObserver* NewL( CSendUiWatcher* aParent );

    virtual ~CSendUiPropertyObserver();

// from base class CActive

    /**
     * From CActive.
     *
     * @since S60 v3.2
     */
    void RunL();

    /**
     * From CActive.
     *
     * @since S60 v3.2
     */
    void DoCancel();
    
private:

    CSendUiPropertyObserver( CSendUiWatcher* aWatcher );

    void ConstructL( );

private: // data

    /**
     * Property for making update requests
     */
    RProperty iPropertyUpdateRequest;

    /**
     * Parent.
     * Not own.
     */
     CSendUiWatcher*   iParent;
     
    };

#endif // C_CSENDUIPROPERTYOBSERVER_H
