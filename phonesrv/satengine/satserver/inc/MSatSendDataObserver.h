/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for DataAvailable event observer.
*
*/



#ifndef MSATSENDDATAOBSERVER_H
#define MSATSENDDATAOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for DataAvailable event observer. When DataAvailable event occurs, the
*  implementors of this class will be notified. However, this class must be send to 
*  MSatBIPEventNotifier to get the notification. 
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatSendDataObserver 
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        MSatSendDataObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatSendDataObserver() {};

    public: // New functions

        /**
        * Notification of data has been sent
        * @param aError Indicates the status of data sent. 
        * @param aLength Number of bytes sent
        */
        virtual void DataSentNotification(
            const TInt aError, 
            const TInt aLength ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSendDataObserver( const MSatSendDataObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSendDataObserver& operator=( const MSatSendDataObserver& );

    };

#endif      // MSATSENDDATAOBSERVER_H   
            
// End of File
