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
* Description: 
*      MPsetRequestObserver is abstract request completion observer class
*
*
*/


#ifndef     MPSETREQUESTOBS_H
#define     MPSETREQUESTOBS_H

//  CLASS DEFINITIONS 
class   MPsetRequestObserver
    {
    public:
        /**
        * Informs the receiver that request has been completed.
        */
        virtual void RequestComplete() = 0;

        /**
        * Informs the receiver that request state has changed.
        * @param new status, or when finished KErrNone or error code.
        */
        virtual void RequestStatusChanged( TInt aNewStatus ) = 0;
    };
#endif // MPSETREQUESTOBS_H
// end of file
