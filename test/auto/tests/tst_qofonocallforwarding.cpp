/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Kanavin <alex.kanavin@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <QtTest/QtTest>
#include <QtCore/QObject>

#include "../../../src/qofonocallforwarding.h"
#include <QtDebug>

class TestQOfonoCallForwarding : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        m = new QOfonoCallForwarding(this);
        m->setModemPath("/phonesim");
        QCOMPARE(m->isValid(), true);

        QSignalSpy ready(m, SIGNAL(readyChanged()));
        QTRY_COMPARE(ready.count(), 1);
        QCOMPARE(m->isReady(), true);
    }

    void testQOfonoCallForwarding()
    {
        QSignalSpy voiceUnconditionalComplete(m, SIGNAL(voiceUnconditionalComplete(bool)));
        QSignalSpy voiceBusyComplete(m, SIGNAL(voiceBusyComplete(bool)));
        QSignalSpy voiceNoReplyComplete(m, SIGNAL(voiceNoReplyComplete(bool)));
        QSignalSpy voiceNoReplyTimeoutComplete(m, SIGNAL(voiceNoReplyTimeoutComplete(bool)));
        QSignalSpy voiceNotReachableComplete(m, SIGNAL(voiceNotReachableComplete(bool)));

        QSignalSpy voiceUnconditionalChanged(m, SIGNAL(voiceUnconditionalChanged(QString)));
        QSignalSpy voiceBusyChanged(m, SIGNAL(voiceBusyChanged(QString)));
        QSignalSpy voiceNoReplyChanged(m, SIGNAL(voiceNoReplyChanged(QString)));
        QSignalSpy voiceNoReplyTimeoutChanged(m, SIGNAL(voiceNoReplyTimeoutChanged(ushort)));
        QSignalSpy voiceNotReachableChanged(m, SIGNAL(voiceNotReachableChanged(QString)));
        QSignalSpy forwardingFlagOnSimChanged(m, SIGNAL(forwardingFlagOnSimChanged(bool)));

        QCOMPARE(m->voiceUnconditional(), QString(""));
        QCOMPARE(m->voiceBusy(), QString(""));
        QCOMPARE(m->voiceNoReply(), QString(""));
        QCOMPARE(m->voiceNoReplyTimeout(), quint16(20));
        QCOMPARE(m->voiceNotReachable(), QString(""));
        QCOMPARE(m->forwardingFlagOnSim(), false);

        m->setVoiceUnconditional("abc");
        QTRY_COMPARE(voiceUnconditionalComplete.count(), 1);
        QCOMPARE(voiceUnconditionalComplete.takeFirst().at(0).toBool(), false);
        m->setVoiceBusy("abc");
        QTRY_COMPARE(voiceBusyComplete.count(), 1);
        QCOMPARE(voiceBusyComplete.takeFirst().at(0).toBool(), false);
        m->setVoiceNoReply("abc");
        QTRY_COMPARE(voiceNoReplyComplete.count(), 1);
        QCOMPARE(voiceNoReplyComplete.takeFirst().at(0).toBool(), false);
        m->setVoiceNoReplyTimeout(-30);
        QTRY_COMPARE(voiceNoReplyTimeoutComplete.count(), 1);
        QCOMPARE(voiceNoReplyTimeoutComplete.takeFirst().at(0).toBool(), false);
        m->setVoiceNotReachable("abc");
        QTRY_COMPARE(voiceNotReachableComplete.count(), 1);
        QCOMPARE(voiceNotReachableComplete.takeFirst().at(0).toBool(), false);

        m->setVoiceBusy("12345678");
        QTRY_COMPARE(voiceBusyComplete.count(), 1);
        QCOMPARE(voiceBusyComplete.takeFirst().at(0).toBool(), true);
        QTRY_COMPARE(voiceBusyChanged.count(), 1);
        QCOMPARE(voiceBusyChanged.takeFirst().at(0).toString(), QString("12345678"));
        m->setVoiceNoReply("12345678");
        QTRY_COMPARE(voiceNoReplyComplete.count(), 1);
        QCOMPARE(voiceNoReplyComplete.takeFirst().at(0).toBool(), true);
        QTRY_COMPARE(voiceNoReplyChanged.count(), 1);
        QCOMPARE(voiceNoReplyChanged.takeFirst().at(0).toString(), QString("12345678"));
        m->setVoiceNoReplyTimeout(30);
        QTRY_COMPARE(voiceNoReplyTimeoutComplete.count(), 1);
        QCOMPARE(voiceNoReplyTimeoutComplete.takeFirst().at(0).toBool(), true);
        QTRY_COMPARE(voiceNoReplyTimeoutChanged.count(), 1);
        QCOMPARE(voiceNoReplyTimeoutChanged.takeFirst().at(0).value<quint16>(), quint16(30));
        m->setVoiceNotReachable("12345678");
        QTRY_COMPARE(voiceNotReachableComplete.count(), 1);
        QCOMPARE(voiceNotReachableComplete.takeFirst().at(0).toBool(), true);
        QTRY_COMPARE(voiceNotReachableChanged.count(), 1);
        QCOMPARE(voiceNotReachableChanged.takeFirst().at(0).toString(), QString("12345678"));
        m->setVoiceUnconditional("12345678");
        QTRY_COMPARE(voiceUnconditionalComplete.count(), 1);
        QCOMPARE(voiceUnconditionalComplete.takeFirst().at(0).toBool(), true);
        QTRY_COMPARE(voiceUnconditionalChanged.count(), 1);
        QCOMPARE(voiceUnconditionalChanged.takeFirst().at(0).toString(), QString("12345678"));

        QTRY_COMPARE(forwardingFlagOnSimChanged.count(), 1);
        QCOMPARE(forwardingFlagOnSimChanged.takeFirst().at(0).toBool(), true);

        QCOMPARE(m->voiceBusy(), QString("12345678"));
        QCOMPARE(m->voiceNoReply(), QString("12345678"));
        QCOMPARE(m->voiceNoReplyTimeout(), quint16(30));
        QCOMPARE(m->voiceNotReachable(), QString("12345678"));
        QCOMPARE(m->voiceUnconditional(), QString("12345678"));
        QCOMPARE(m->forwardingFlagOnSim(), true);

        m->disableAll("all");
        QTRY_COMPARE(voiceUnconditionalChanged.count(), 1);
        QCOMPARE(voiceUnconditionalChanged.takeFirst().at(0).toString(), QString(""));
        QTRY_COMPARE(voiceBusyChanged.count(), 1);
        QCOMPARE(voiceBusyChanged.takeFirst().at(0).toString(), QString(""));
        QTRY_COMPARE(voiceNoReplyChanged.count(), 1);
        QCOMPARE(voiceNoReplyChanged.takeFirst().at(0).toString(), QString(""));
        QTRY_COMPARE(voiceNoReplyTimeoutChanged.count(), 1);
        QCOMPARE(voiceNoReplyTimeoutChanged.takeFirst().at(0).toUInt(), uint(20));
        QTRY_COMPARE(voiceNotReachableChanged.count(), 1);
        QCOMPARE(voiceNotReachableChanged.takeFirst().at(0).toString(), QString(""));
        QTRY_COMPARE(forwardingFlagOnSimChanged.count(), 1);
        QCOMPARE(forwardingFlagOnSimChanged.takeFirst().at(0).toBool(), false);

        QCOMPARE(m->voiceUnconditional(), QString(""));
        QCOMPARE(m->voiceBusy(), QString(""));
        QCOMPARE(m->voiceNoReply(), QString(""));
        QCOMPARE(m->voiceNoReplyTimeout(), quint16(20));
        QCOMPARE(m->voiceNotReachable(), QString(""));
        QCOMPARE(m->forwardingFlagOnSim(), false);
    }

    void cleanupTestCase()
    {

    }

private:
    QOfonoCallForwarding *m;
};

QTEST_MAIN(TestQOfonoCallForwarding)
#include "tst_qofonocallforwarding.moc"
