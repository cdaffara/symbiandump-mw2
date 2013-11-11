/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TESTUTILITIES_H
#define TESTUTILITIES_H

#include <qobject.h>
#include <qglobal.h>
#include <qlist.h>
#include <qmetaobject.h>
#include <QDebug>

/*! Provides services for OOM simulation and memory leak detection. */
class MemoryAllocator : public QObject
{
    Q_OBJECT
    
public:
    
    /*! Activates OOM simulation. */
    static void enableOomSimulation();
    
    /*! Disables OOM simulation. */
    static void disableOomSimulation();
    
    /*! Checks whether OOM simulation is active. */
    static bool isOomSimulationEnabled();
    
    /*! Returns current alloc fail index. */
    static int currentAllocFailIndex();
    
    /*! Allocates memory. */
    static void* alloc(std::size_t sz);
    
    /*! Frees allocated memory. */
    static void free(void *memoryAddress);
    
    /*! Checks that all memory allocated through this allocator is freed. */
    static void verifyMemoryAllocations();

private:
    
    static bool m_isOomSimulationEnabled;
    static int m_numOfAllocsSinceLastFail;
    static int m_allocFailIndex;
    static QList<void*> m_allocList;
};

/*! Provides services to run QTest cases with OOM simulation. */
class OomTestExecuter : public QObject
{
    Q_OBJECT
    
public:
    
    /*! Runs specified test case with OOM simulation. */
    template <typename CLASS>
    static void runTest(CLASS& testObject, void(CLASS::*testMethod)())
    {
        MemoryAllocator::enableOomSimulation();

        bool exceptionCaught = false;
        do {
            exceptionCaught = false;
            int currentAllocFailIndex = 
                MemoryAllocator::currentAllocFailIndex();
            
            try {
                try {
                    QMetaObject::invokeMethod(
                        &testObject, "init", Qt::DirectConnection);
                    (testObject.*testMethod)();
                } catch (const std::bad_alloc &ex) {
                    exceptionCaught = true;
                    QMetaObject::invokeMethod(
                        &testObject, "cleanup", Qt::DirectConnection);
                }
            // TODO: for some reason bad_alloc exception is corrupted to 
            // unknown exception and nested catch block is needed to be able to
            // handle situation. One level try-catch does not work for some reason.
            } catch (...) {
                exceptionCaught = true;
                QMetaObject::invokeMethod(
                    &testObject, "cleanup", Qt::DirectConnection);
                if (currentAllocFailIndex == MemoryAllocator::currentAllocFailIndex()) {
                    qDebug() << "OomTestExecuter::runTest, ERROR: unexpected exception!";
                    throw;
                }
            }
        } while(exceptionCaught);
        
        QMetaObject::invokeMethod(&testObject, "cleanup", Qt::DirectConnection);
        MemoryAllocator::disableOomSimulation();
    }

    /*! Runs specified test case with OOM simulation. */
    static void runTest(
        QObject &testObject, 
        const char *testMethod);
    
    /*! Runs all test cases in given QTest object with OOM simulation. */
    static void runAllTests(
        QObject &testObject, 
        const char *callingTestMethod);

private:
    
    static bool isValidSlot(const QMetaMethod &sl);
};

#define EXPECT_EXCEPTION(statements)    \
{                                       \
    bool exceptionDetected = false;     \
    try {                               \
        statements                      \
    } catch (...) {                     \
        exceptionDetected = true;       \
    }                                   \
    QVERIFY(true == exceptionDetected); \
}

#define QTEST_MAIN_S60(TestObject) \
int main(int argc, char *argv[]) \
{ \
    char *new_argv[3]; \
    QCoreApplication app(argc, argv); \
    \
    QString str = "C:\\data\\" + QFileInfo(QCoreApplication::applicationFilePath()).baseName() + ".log"; \
    QByteArray   bytes  = str.toAscii(); \
    \
    char arg1[] = "-o"; \
    \
    new_argv[0] = argv[0]; \
    new_argv[1] = arg1; \
    new_argv[2] = bytes.data(); \
    \
    TestObject tc; \
    int result = QTest::qExec(&tc, 3, new_argv); \
    \
    MemoryAllocator::verifyMemoryAllocations(); \
    \
    return result;\
}

#endif  // TESTUTILITIES_H
