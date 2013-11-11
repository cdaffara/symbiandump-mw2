/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Defines the data types related to packet 
*
*/



#ifndef C_CSUPLPACKETINFO_H
#define C_CSUPLPACKETINFO_H

// INCLUDES
#include <e32base.h>


class CSuplPacketInfo : public CBase
    {
    private:            
        //Default Constructor
        CSuplPacketInfo(const TDesC8& aPacket, TRequestStatus& aStatus);

    public:
  		static CSuplPacketInfo* NewL(const TDesC8& aPacket, TRequestStatus& aStatus);

    public: // New functions
    
        void GetPacketData(TPtrC8 &aPacket);
        void CompleteRequest(TInt err);
        TBool IsStatusMatches(TRequestStatus& aStatus);
        
    private:
		void ConstructL();
		
    private:    // Data
        const TDesC8& iPacket;

        TRequestStatus* iStatus;
};

#endif      // CSUPLPACKETINFO  
            
// End of File
