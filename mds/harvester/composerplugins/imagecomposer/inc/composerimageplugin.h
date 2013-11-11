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
* Description:  Composer image plug-in
*
*/


#include <mdeobject.h>

#include "composerplugin.h"
#include "imagecomposerao.h"
#include "locationrelationobserver.h"
#include "imagepresentobserver.h"
#include "mdeharvestersession.h"

#ifndef __CCOMPOSERIMAGEPLUGIN_H__
#define __CCOMPOSERIMAGEPLUGIN_H__

/**
* Composer image plugin.
* This class implements the composer plugin interface.
* Actual composing is done in class CComposerImagePluginAO.
*/
class CComposerImagePlugin : public CComposerPlugin
    {
    friend class CLocationRelationObserver;
    friend class CImagePresentObserver;
    
    public:

        /**
         * Construction.
         */
        static CComposerImagePlugin* NewL();

        /**
         * Destruction.
         */
        virtual ~CComposerImagePlugin();

        /**
         * Checks if composing is complete.
         */
        TBool IsComposingComplete();

    protected:

        /**
         * Set observers that will be notified of composing events.
         */
        void SetObservers();

        /**
         * Remove observers.
         */
        void RemoveObservers();

        /** 
         * Handle object modifications
         */
        void HandleObjectNotification(CMdESession& aSession, 
				TObserverNotificationType aType,
				const RArray<TItemId>& aObjectIdArray);
        
        void WriteGPSTagsL( TItemId aObjectId, TItemId locationId );
        
        void HandlePendingObjects( const RArray<TItemId>& aObjectIdArray );
        
    private:

        /**
         * Private constructor.
         */    
        CComposerImagePlugin();
        
        /**
         * 2nd phase construction.
         */    
        void ConstructL();

        /**
         * Private, leaving set observers method that is called by the public version of this method.
         */
        void SetObserversL();

    private:

        /**
         * Pointer to the active object that implements the asyncronous functionality
         * of this plug-in.
         */
        CImageComposerAO* iImageComposerAO;

        /**
         * Pointer to the observer listening for location relation changes in Mde.
         */
        CLocationRelationObserver* iLocationRelationObserver;
        
        CImagePresentObserver* iImagePresentObserver;
        
        CMdEHarvesterSession* iMdEHarvesterSession;
    };

#endif // __CCOMPOSERIMAGEPLUGIN_H__
