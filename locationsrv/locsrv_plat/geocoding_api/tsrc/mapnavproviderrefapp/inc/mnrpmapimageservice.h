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
* Description:  CMnrpMapImageService class
*
*/


#ifndef MN_TPMAPIMAGESERVICE_H
#define MN_TPMAPIMAGESERVICE_H

#include "mnmapimageservicebase.h"

class CPosLandmark;
class CEikonEnv;
class CFont;
class CMnrpMapModel;

/** Implementation of geocoding service (KMnAppMapImageService). 
 */
class CMnrpMapImageService : public CMnMapImageServiceBase
    {
    public :
        static CMnrpMapImageService* NewL( CMnrpEngine& aEngine );
        
        void HandleTestCommandL( TInt aCommand );

    protected :
        /** C++ constructor */
        CMnrpMapImageService( CMnrpEngine& aEngine );
        /** Destructor */
        virtual ~CMnrpMapImageService();

        void ConstructL();

        void LeaveIfBusyL();
        void LeaveIfInstructedL();
        void StartProcessingL();
        static TInt MapImageProcessingCallBack( TAny* aPtr );
        void HandleMapImageCompletedL();
        void FinishProcessingL();

        TInt GetTestCommand();

        void RenderL();

    private:
        void DumpDataL();
        
        void CreateFonts();
        void DestroyFonts();

    protected: // from CMnMapImageServiceBase
    
        /** Handles client's request for finding address by given coordinate. */
        void HandleRenderingL();
        
    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

    private:
    
        enum TServiceType
            {
            ENone,
            ERender,
            };
            
        TServiceType            iCurrentService;
        CMnrpEngine&            iEngine;
        CMnrpMapModel*          iModel;
        CPeriodic*              iProcessor;
        CEikonEnv*              iEikon;
        CFont*                  iItemFont;
        CFont*                  iTextFont;
    };

#endif // MN_TPMAPIMAGESERVICE_H


