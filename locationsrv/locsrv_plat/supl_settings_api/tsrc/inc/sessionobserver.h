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
* Description:  Declaration of settings api classes
*
*/



#ifndef SESSIONOBSERVER_H
#define SESSIONOBSERVER_H

#include <epos_msuplsessionobserver.h>

class CSessionObserver: public CBase,public MSuplSessionObserver
{
	
	
    public:     
         /**
        * C++ Constructor
        */
        CSessionObserver();
        
        /**
        * C++ Destructor
        */
        ~CSessionObserver();

        /**
        * Two-phase constructor
        */
        static CSessionObserver* NewL();
        
    public: // From  MSuplSessionObserver
    
       void HandleSuplSessionChangeL(
                TSuplSessionEventType aEvent,TInt64 aSessionId );   

    private:
        /**
        * EPOC default constructor
        */
        void ConstructL();

       
	
};
#endif      // SESSIONOBSERVER_H           
// End of File


