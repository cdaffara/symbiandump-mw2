/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvests metadata from sms/mms messages.*
*/


#ifndef __CHARVESTERMESSAGEPLUGIN_H__
#define __CHARVESTERMESSAGEPLUGIN_H__

#include <mdeobject.h>
#include <fbs.h>
#include <msvapi.h>

#include "harvesterplugin.h"
#include "monitorplugin.h"

// forward declarations
class CHarvesterMessagePluginAO;
class CMsvSession;
class CClientMtmRegistry;
class CMmsClientMtm;
class CSmsClientMtm;

/**
 * Message harvester plug-in.
 * Harvests metadata from sms/mms messages.
 * Implements CHarvesterPlugin.
 */
class CHarvesterMessagePlugin : public CHarvesterPlugin,
								public MMsvSessionObserver
    {
    public:

        /**
        * Construction
        * @return Harvester image plugin
        */
        static CHarvesterMessagePlugin* NewL();

        /**
        * Destruction
        */
        virtual ~CHarvesterMessagePlugin();

		/**
		* @param aHD harvester data type
		*/
		void HarvestL( CHarvesterData* aHD );
		
		/**
        * From MMsvSessionObserver
        */
        virtual void HandleSessionEventL( TMsvSessionEvent aEvent, 
                                          TAny* aArg1, 
                                          TAny* aArg2, 
                                          TAny* aArg3 );
		
    private:
    
        /**
         * Default constructor
         */
        CHarvesterMessagePlugin();
        
        /**
         * 2nd phase constructor.
         */
        void ConstructL();
        
        /**
         * The method that actually extracts the metadata
         * @param aMetadataObject  Metadata object which is filled with metadata
         * @return Error code which indicates status of the operation
         */                
         TInt GatherDataL( CMdEObject& aMetadataObject );
         
         /**
         * It takes const ref to two THarvestResult objects and returns zero if 
         * the iUri of objects are equal, a negative value if aFirst is less than aSecond and 
         * a positive value if aFirst is greater than aSecond. 
         * @param aFirst
         * @param aSecond   
         * @return returns zero if the objects are equal, a negative value if aFirst 
         * is less than aSecond and a positive value if aFirst is greater than aSecond. 
         */   
         static TInt CompareByUri( const THarvestResult& aFirst, const THarvestResult& aSecond );
         
         /**
         * It takes const ref to two CMdEObject objects and returns zero if 
         * the id of objects are equal, a negative value if aFirst is less than aSecond and 
         * a positive value if aFirst is greater than aSecond. 
         * @param aFirst
         * @param aSecond   
         * @return returns zero if the objects are equal, a negative value if aFirst 
         * is less than aSecond and a positive value if aFirst is greater than aSecond. 
         */   
         static TInt CompareById( const CMdEObject& aFirst, const CMdEObject& aSecond );    
         
         /**
          *
          */
         void SetPropertiesL(CHarvesterData& aHD);
    
    private:
    	
		/**
		 * Session Message server session, own
		 */
		 CMsvSession* iMsvSession;
    
		 /**
        * iMTMReg Accesses the MTM on the message server
        */
        CClientMtmRegistry* iMtmReg;    
        
        /**
        * iMmsMtm Access MMS messages on the message server
        */
        CMmsClientMtm* iMmsMtm;    
        
        /**
        * iSmsMtm Access SMS messages on the message server
        */
        CSmsClientMtm* iSmsMtm;
        
        /**
         *
         */
        HBufC* iFromOrTo;
        
        /**
         *
         */
        TInt64 iSize;
        
        /**
         *
         */
        TBool iIncoming;
        
        /**
         *
         */
        TTime iDate;
        
        /**
         *
         */
        TPtrC iItemType;
        
        /**
         *
         */
        TPtrC iSubject;
    };

#endif

