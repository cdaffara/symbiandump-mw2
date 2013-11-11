/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the MCbsDbObserver class.
*    
*                Virtual class defining an interface for a database observer.
*
*/



#ifndef MCBSDBOBSERVER_H
#define MCBSDBOBSERVER_H

//  INCLUDES

#include <e32base.h>

//  CLASS DEFINITIONS 

/**
*   This class defines the interface for database observers.
*/
class MCbsDbObserver
    {
    public:     // New functions
        /**
        *   Indicates that the database has been reinitialised.
        */
        virtual void DatabaseInitialisedInd() = 0;

        /**
        *   Indicates that the database has been closed, which should happen 
        *   only when the database manager is to be destroyed.
        */
        virtual void DatabaseClosedInd() = 0;
    };

#endif      //  CBSDBOBSERVER_H   
            
// End of File
