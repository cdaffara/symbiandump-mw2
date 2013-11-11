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
* Description:  Observer interface for collection helper
*
*/


#ifndef MMPXCOLLECTIONHELPEROBSERVER_H
#define MMPXCOLLECTIONHELPEROBSERVER_H

/**
 *  Interface class for collection helper observer. Lets the client know when the
 *  operation is complete.
 *
 *  @lib mpxcollectionhelper.lib
 */
NONSHARABLE_CLASS( MMPXCollectionHelperObserver )
    {
public:
    /**
    * Callback function to signal that adding a file is complete.
 	* @since S60 3.2.3
    * @param aErr error of the operation
    */
    virtual void HandleAddFileCompleteL( TInt aErr ) = 0;    
    };    

#endif // MMPXCOLLECTIONHELPEROBSERVER_H
