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
* Description:   Header file for Socket Read
*
*/


#ifndef __MConnectObserver_H__
#define __MConnectObserver_H__


/**
*  MConnectObserver is a an observer  
*  
*  *  @since Series 60 3.1M //e.g. Series 60 3.0
*/
class MConnectObserver
    {
    public:  // Interface methods
    
    virtual void CompleteConnect(TInt aHandle)=0;	
   
    
    
    private:
    
    //
    };

#endif      // __MConnectObserver_H__   
            
// End of File
