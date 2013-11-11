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
* Description:  Header of ControlObserver interface.
*
*/


#ifndef CONTROLOBSERVER_H
#define CONTROLOBSERVER_H

namespace multimedia
    {
    
    class MControl;    
    class MControlObserver
        {
        public:    
        		static const TUint KErrorEvent          = 0;				
        		static const TUint KSpecificEventBase   = 10000;				

        public:
            virtual void Event( MControl* aObject,TUint aEvent, TAny* aData ) = 0;
        };
    } // namespace multimedia

#endif // CONTROLOBSERVER_H

// End of file
