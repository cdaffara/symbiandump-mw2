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
* Description:  This file contains the header file of the MCbsDbSettingsObserver class.
*                Virtual class defining an interface for a settings observer.
*
*/



#ifndef    MCBSDBSETTINGSOBSERVER_H
#define    MCBSDBSETTINGSOBSERVER_H

//  CLASS DECLARATION 

/**
*   This class defines the interface for settings observers.
*/
class    MCbsDbSettingsObserver
    {
    public:        // New functions
        /**
        *   The method is used to indicate that the topic detection 
        *   status has been changed.
        */
        virtual void TopicDetectionStatusChangedIndL() = 0;

        /**
        *   The method is used to indicate that the reception status 
        *   has been changed.
        */
        virtual void ReceptionStatusChangedIndL() = 0;

        /**
        *   The method is used to indicate that the preferred languages 
        *   has been changed.
        */
        virtual void LanguagesChangedIndL() = 0;

        /**
        *   The method is used to indicate that the limited reception 
        *   status has been changed.
        */
        virtual void LimitedReceptionStatusChangedIndL() = 0;

        /**
        *   The method is used to indicate that the clean up interval 
        *   of read messages has been changed.
        */
        virtual void CleanupTimeChangedIndL() = 0;
    };

#endif      //  MCBSDBSETTINGSOBSERVER_H   
            
// End of File
