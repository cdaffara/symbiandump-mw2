/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitor the set up idle mote text P&S key.
*
*/



#ifndef CSATPLUGINMONITOR_H
#define CSATPLUGINMONITOR_H

//  INCLUDES
#include    <e32property.h>             // For monitor the P&S key   

// CLASS DECLARATION
NONSHARABLE_CLASS( CSatPluginMonitor ) : public CActive
    {
    public: // Constructors and destructor
        
        /**
         * Part of the two phased constuction
         *
         * @return Pointer to the created CSatPluginMonitor object
         */
        static CSatPluginMonitor* NewL();
    
        /**
         * Destructor
         */
        virtual ~CSatPluginMonitor();
        
    public: // New function
    
        /**
         * Start monitor the P&S key.
         * 
         * @since 5.1
         * @param aStatus Request to be completed on update.
         * @return Error code indicating the status of starting the monitor.
         */
        TInt Start( TRequestStatus& aStatus );
        
        /**
         * Get idle mode text via P&S key.
         * 
         * @since 5.1
         * @param aText the idle mode text.
         * @return Error code indicating the status of GetIdleModeText. 
         */      
        TInt GetIdleModeText( HBufC*& aText );
    
        /**
         * Get idle mode icon qualifier via P&S key.
         * 
         * @since 5.1
         * @param aIconQualifier the idle mode icon qualifier.
         * @return Error code indicating the status of GetIdleModeIconQualifier.
         */     
        TInt GetIdleModeIconQualifier( TInt& aIconQualifier );
    
        /**
         * Get idle mode icon id via P&S key.
         * 
         * @since 5.1
         * @param aRecordNumber the icon mode icon id.
         * @return Error code indicating the status of GetIdleModeIconId.
         */     
        TInt GetIdleModeIconId( TInt& aRecordNumber );        
        
    protected: // Functions from CActive
    
        /**
         * From CActive.
         *  
         * Handles the request completion.
         */
        void RunL();
    
        /**
         * From CActive.
         * 
         * Implements the cancel protocol.
         */
        void DoCancel();
       
        /**
         * From CActive
         * 
         * Handles a leave occurring in the request completion event 
         * handler RunL()
         * @param aError Leave from RunL().
         * @return Error code
         */    
        TInt RunError( TInt aError );
        
    private: // Constructors
            
        /**
         * Part of the two phased construction
         */
        void ConstructL();
        
        /**
         * Constructor to use in the object creation. 
         * Initializes the necessary data.
         */
        CSatPluginMonitor();        
        
    private: // data
    
        /**
         * Reference of idle application TRequestStatus.
         * Notify the idle application when P&S key changed.
         *
         * Not own
         */
        TRequestStatus* iIdleAppStatus;  
          
        /**
         * For monitor the P&S key.
         */    
        RProperty iIdleMode;
    };

#endif // CSATPLUGINMONITOR_H