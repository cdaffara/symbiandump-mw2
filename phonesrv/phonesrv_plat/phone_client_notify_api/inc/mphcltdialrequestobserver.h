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
* Description:  It defines the interface for the observers of 
*                the dial requests.
*
*/


#ifndef MPHCLTDIALREQUESTOBSERVER_H   
#define MPHCLTDIALREQUESTOBSERVER_H   

class CPhCltDialData;

// CLASS DECLARATION

/**
*  Defines the interface for the observers of the CPhCltExtPhoneBase.
*
*  @lib phoneclient.lib
*  @since 5.0
*/
class MPhCltDialRequestObserver
    {          
    public:  // New functions   
        
        /**
        * It is called whenever client's dial request is completed.
        *
        * @param aDialData (transfers ownership)
        */       
        virtual void DialRequest( CPhCltDialData* aDialData ) = 0;
        
    };

#endif      // MPHCLTDIALREQUESTOBSERVER_H   
            
// End of File
