/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/

#ifndef UT_CPHCLTUSSDNOTECONTROLLER_H
#define UT_CPHCLTUSSDNOTECONTROLLER_H


#include <QtTest/QtTest>
#include <QObject>
#include "MPhCltUssdNoteControllerCallBack.h" 



// Class forwards
class CPhCltUssdNoteController;

/**
 *  Satapp unit test class.
 * 
 *  @since S60 <TB10.1>
 */
class Ut_CphCltUssdNoteController : public QObject,
                                    public MPhCltUssdNoteControllerCallBack
{
    Q_OBJECT

public:

    Ut_CphCltUssdNoteController(QObject *parent = 0);

    virtual ~Ut_CphCltUssdNoteController();

     /**
     * From MPhCltUssdNoteControllerCallBack.
     * 
     */    
    void GlobalWaitNoteHidden();

private slots:
     /**
     * Called before the first testfunction is executed.
     */
    void initTestCase();

     /**
     * Called after the last testfunction has been executed.
     * 
     */
    void cleanupTestCase();

     /**
     * Test CPhCltUssdNoteController's constructor.
     * 
     */
    void testCreateMainHandler();

    /**
     * Test CPhCltUssdNoteController's testShowGlobalInformationNote param list.
     * 
     */
    void testShowGlobalInformationNote_data();

     /**
     * Test CPhCltUssdNoteController's ShowGlobalInformationNoteL.
     * 
     */
    void testShowGlobalInformationNote();

     /**
     * Test CPhCltUssdNoteController's ShowGlobalWaitNoteL.
     * 
     */
    void testShowGlobalWaitNote();

     /**
     * Test CPhCltUssdNoteController's DestroyGlobalWaitNote.
     * 
     */
    void testDestroyGlobalWaitNote();

     /**
     * Test CPhCltUssdNoteController's cancel GlobalWaitNote.
     * 
     */
    void testCancelGlobalWaitNote();

private:

     /**
     * Own.
     */
    CPhCltUssdNoteController *mNoteController;

};

#endif // UT_CPHCLTUSSDNOTECONTROLLER_H
