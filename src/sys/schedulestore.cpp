#include "schedulestore.h"
#include "holidaystore.h"
#include "schedulecolumn.h"
#include "schedulerow.h"
#include "databasemanager.h"
#include "entry.h"
#include "workerstore.h"
#include <QtMath>
#include <QImage>
#include "xlsxdocument.h"
#include "xlsxformat.h"

ScheduleStore::ScheduleStore(WorkerStore *workers, HolidayStore *holidays, DatabaseManager *manager, Error *error)
    : ErrorHandler(error)
{
    this->oHolidays = holidays;
    this->oDatabase = manager;
    this->oWorkers = workers;
    this->oDate.setDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    int prevYear = this->oDate.year();
    int prevMonth = this->oDate.month() - 1;
    if(this->oDate.month() == 1) {
        prevYear--;
        prevMonth = 12;
    }
    QDate previous(prevYear, prevMonth, 1);
    int nextYear = this->oDate.year();
    int nextMonth = this->oDate.month() + 1;
    if(this->oDate.month() == 12) {
        nextYear++;
        nextMonth = 1;
    }
    QDate next(nextYear, nextMonth, 1);
    this->getNote();
    this->populateDayNumbers(previous, next);
    this->populateDayColors(previous, next);
    this->populateShifts(previous, next);
    this->populateTitle(this->oDate.year(), this->oDate.month());
}

ScheduleStore::~ScheduleStore()
{
    qDeleteAll(this->aColumns);
    qDeleteAll(this->aSchedules);
}

void ScheduleStore::getNote()
{
    QSqlQuery query = this->oDatabase->query();
    query.prepare("select * from notes where year=:Year and month=:Month");
    query.bindValue(":Year", this->oDate.year());
    query.bindValue(":Month", this->oDate.month());
    query.exec();
    if(query.next()) {
        QString note = query.value(1).toString();
        this->sNote = note;
        emit this->noteChanged();
    } else {
        QSqlQuery query = this->oDatabase->query();
        query.prepare("insert into notes(data, year, month) values(:Data, :Year, :Month)");
        query.bindValue(":Year", this->oDate.year());
        query.bindValue(":Month", this->oDate.month());
        query.bindValue(":Data", "");
        query.exec();
        this->sNote = "";
        emit this->noteChanged();
    }
}

void ScheduleStore::populateDayNumbers(const QDate &previous, const QDate &next)
{
    this->aDayNumbers.clear();
    for(int i=previous.daysInMonth() - this->oDate.dayOfWeek() + 2;i<=previous.daysInMonth();i++) {
        this->aDayNumbers.append(QString::number(i));
    }
    for(int i=1;i<=this->oDate.daysInMonth();i++) {
        this->aDayNumbers.append(QString::number(i));
    }
    int dayCounter = previous.daysInMonth() - previous.daysInMonth() - this->oDate.dayOfWeek() + 2 + this->oDate.daysInMonth();
    for(int i=1;i<=42-dayCounter;i++) {
        this->aDayNumbers.append(QString::number(i));
    }
}

void ScheduleStore::populateDayColors(const QDate &previous, const QDate &next)
{
    this->aDayColors.clear();
    for(int i=previous.daysInMonth() - this->oDate.dayOfWeek() + 2;i<=previous.daysInMonth();i++) {
        QDate date(previous.year(), previous.month(), i);
        if(date.dayOfWeek() == 6) {
            this->aDayColors.append("#1B7E5A");
        } else if(date.dayOfWeek() == 7) {
            this->aDayColors.append("#166549");
        } else if(this->oHolidays->isHoliday(date)) {
            this->aDayColors.append("#166549");
        } else {
            this->aDayColors.append("white");
        }
    }
    for(int i=1;i<=this->oDate.daysInMonth();i++) {
        QDate date(this->oDate.year(), this->oDate.month(), i);
        if(date.dayOfWeek() == 6) {
            this->aDayColors.append("#1B7E5A");
        } else if(date.dayOfWeek() == 7) {
            this->aDayColors.append("#166549");
        } else if(this->oHolidays->isHoliday(date)) {
            this->aDayColors.append("#166549");
        } else {
            this->aDayColors.append("white");
        }
    }
    int dayCounter = previous.daysInMonth() - previous.daysInMonth() - this->oDate.dayOfWeek() + 2 + this->oDate.daysInMonth();
    for(int i=1;i<=42-dayCounter;i++) {
        QDate date(next.year(), next.month(), i);
        if(date.dayOfWeek() == 6) {
            this->aDayColors.append("#1B7E5A");
        } else if(date.dayOfWeek() == 7) {
            this->aDayColors.append("#166549");
        } else if(this->oHolidays->isHoliday(date)) {
            this->aDayColors.append("#166549");
        } else {
            this->aDayColors.append("white");
        }
    }
}

void ScheduleStore::populateTitle(int year, int month)
{
    switch(month) {
    case 1:
        this->setMonthName(this->tr("January").append(QString(" %1").arg(year)));
        break;
    case 2:
        this->setMonthName(this->tr("February").append(QString(" %1").arg(year)));
        break;
    case 3:
        this->setMonthName(this->tr("March").append(QString(" %1").arg(year)));
        break;
    case 4:
        this->setMonthName(this->tr("April").append(QString(" %1").arg(year)));
        break;
    case 5:
        this->setMonthName(this->tr("May").append(QString(" %1").arg(year)));
        break;
    case 6:
        this->setMonthName(this->tr("June").append(QString(" %1").arg(year)));
        break;
    case 7:
        this->setMonthName(this->tr("July").append(QString(" %1").arg(year)));
        break;
    case 8:
        this->setMonthName(this->tr("August").append(QString(" %1").arg(year)));
        break;
    case 9:
        this->setMonthName(this->tr("September").append(QString(" %1").arg(year)));
        break;
    case 10:
        this->setMonthName(this->tr("October").append(QString(" %1").arg(year)));
        break;
    case 11:
        this->setMonthName(this->tr("November").append(QString(" %1").arg(year)));
        break;
    case 12:
        this->setMonthName(this->tr("December").append(QString(" %1").arg(year)));
        break;
    }
}

void ScheduleStore::populateShifts(const QDate &previous, const QDate &next)
{
    qDeleteAll(this->aColumns);
    qDeleteAll(this->aSchedules);
    this->aColumns.clear();
    this->aSchedules.clear();

    QSqlQuery query = this->oDatabase->query();
    query.prepare("select * from entries where end>=:Start and start<=:End order by end");
    query.bindValue(":Start", this->oDate.toString("yyyy-MM-dd").append(" 00:00:00"));
    query.bindValue(":End", next.addDays(-1).toString("yyyy-MM-dd").append(" 23:59:59"));
    query.exec();
    int year = this->oDate.year();
    int month = this->oDate.month();
    QDateTime scheduleStart(QDate(year, month, 1), QTime(0,0,0));
    QDateTime scheduleEnd(QDate(year, month, scheduleStart.date().daysInMonth()), QTime(23,59,59));
    while(query.next()) {
        int id = query.value(0).toInt();
        int workerId1 = query.value(1).toInt();
        QString name1 = query.value(2).toString();
        int workerId2 = query.value(3).toInt();
        QString name2 = query.value(4).toString();
        int hours1 = query.value(5).toInt();
        int bonus1 = query.value(6).toInt();
        int load1 = query.value(7).toInt();
        int hours2 = query.value(8).toInt();
        int bonus2 = query.value(9).toInt();
        int load2 = query.value(10).toInt();
        QDateTime start = QDateTime::fromString(query.value(11).toString(), "yyyy-MM-dd HH:mm:ss");
        QDateTime end = QDateTime::fromString(query.value(12).toString(), "yyyy-MM-dd HH:mm:ss");
        if(start < scheduleStart) {
            this->aSchedules.append(new Entry(id, workerId1, workerId2, name1, name2, load2, hours2, bonus2, start, end));
        } else if(end > scheduleEnd) {
            this->aSchedules.append(new Entry(id, workerId1, workerId2, name1, name2, load1, hours1, bonus1, start, end));
        } else {
            this->aSchedules.append(new Entry(id, workerId1, workerId2, name1, name2, load1, hours1, bonus1, start, end));
        }
    }
    if(this->aSchedules.count() > 2) {
        ScheduleColumn *col = new ScheduleColumn();
        int prefixDays = previous.daysInMonth() - (previous.daysInMonth() - this->oDate.dayOfWeek() + 1);
        int hourCounter = 0;
        if(prefixDays > 0) {
            col->add(new ScheduleRow(prefixDays * 24 - 5));
            hourCounter += prefixDays * 24 - 5;
        }
        for(int i=0;i<this->aSchedules.count();i++) {
            if(hourCounter + 12 > 24 * 7) {
                col->add(new ScheduleRow(5, this->aSchedules[i], "white", true));
                this->aColumns.append(col);
                col = new ScheduleColumn();
                col->add(new ScheduleRow(7, this->aSchedules[i], "white", false, true));
                hourCounter = 7;
            } else {
                if(i == 0 && prefixDays == 0) {
                    col->add(new ScheduleRow(7, this->aSchedules[i], "white"));
                    hourCounter += 7;
                } else {
                    col->add(new ScheduleRow(12, this->aSchedules[i], "white"));
                    hourCounter += 12;
                }
            }
        }
        col->add(new ScheduleRow(24 * 7 - hourCounter));
        this->aColumns.append(col);
    }
}

void ScheduleStore::updateCurrent()
{
    int year = this->oDate.year();
    int month = this->oDate.month();
    int prevYear = this->oDate.year();
    int prevMonth = this->oDate.month() - 1;
    if(this->oDate.month() == 1) {
        prevYear--;
        prevMonth = 12;
    }
    QDate previous(prevYear, prevMonth, 1);
    int nextYear = this->oDate.year();
    int nextMonth = this->oDate.month() + 1;
    if(this->oDate.month() == 12) {
        nextYear++;
        nextMonth = 1;
    }
    QDate next(nextYear, nextMonth, 1);
    this->getNote();
    this->populateDayNumbers(previous, next);
    this->populateDayColors(previous, next);
    this->populateTitle(year, month);
    this->populateShifts(previous, next);
    emit this->dayNumbersChanged();
    emit this->dayColorsChanged();
    emit this->columnCountChanged();
    emit this->columnsChanged();
}

void ScheduleStore::changeWorker(int entryId, int workerNumber, int workerId)
{
    int year = this->oDate.year();
    int month = this->oDate.month();
    QDateTime scheduleStart(QDate(year, month, 1), QTime(0,0,0));
    QDateTime scheduleEnd(QDate(year, month, scheduleStart.date().daysInMonth()), QTime(23,59,59));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("select * from entries where id=:Id");
    query.bindValue(":Id", entryId);
    query.exec();
    if(query.next()) {
        int id = query.value(0).toInt();
        int workerId1 = query.value(1).toInt();
        QString name1 = query.value(2).toString();
        int workerId2 = query.value(3).toInt();
        QString name2 = query.value(4).toString();
        int hours1 = query.value(5).toInt();
        int bonus1 = query.value(6).toInt();
        int load1 = query.value(7).toInt();
        int hours2 = query.value(8).toInt();
        int bonus2 = query.value(9).toInt();
        int load2 = query.value(10).toInt();
        QDateTime start = QDateTime::fromString(query.value(11).toString(), "yyyy-MM-dd HH:mm:ss");
        QDateTime end = QDateTime::fromString(query.value(12).toString(), "yyyy-MM-dd HH:mm:ss");
        QList<Worker *> workers = this->oWorkers->getData();
        Worker *newWorker = 0, *oldWorker = 0;
        for(int i=0;i<workers.count();i++) {
            Worker *current = workers[i];
            if(current->id() == workerId)
                newWorker = current;
            if(workerNumber == 0) {
                if(workerId1 == current->id()) {
                    oldWorker = current;
                }
            } else {
                if(workerId2 == current->id()) {
                    oldWorker = current;
                }
            }
        }
        if(start < scheduleStart) {
            oldWorker->setLoad(oldWorker->getLoad(start.date().year(), start.date().month()) - load1, start.date().year(), start.date().month());
            oldWorker->setBonus(oldWorker->getBonus(start.date().year(), start.date().month()) - bonus1, start.date().year(), start.date().month());
            oldWorker->setLoad(oldWorker->getLoad(year, month) - load2, year, month);
            oldWorker->setBonus(oldWorker->getBonus(year, month) - bonus2, year, month);
            newWorker->setLoad(newWorker->getLoad(start.date().year(), start.date().month()) + load1, start.date().year(), start.date().month());
            newWorker->setBonus(newWorker->getBonus(start.date().year(), start.date().month()) + bonus1, start.date().year(), start.date().month());
            newWorker->setLoad(newWorker->getLoad(year, month) + load2, year, month);
            newWorker->setBonus(newWorker->getBonus(year, month) + bonus2, year, month);
        } else if(end > scheduleEnd) {
            oldWorker->setLoad(oldWorker->getLoad(end.date().year(), end.date().month()) - load2, end.date().year(), end.date().month());
            oldWorker->setBonus(oldWorker->getBonus(end.date().year(), end.date().month()) - bonus2, end.date().year(), end.date().month());
            oldWorker->setLoad(oldWorker->getLoad(year, month) - load1, year, month);
            oldWorker->setBonus(oldWorker->getBonus(year, month) - bonus1, year, month);
            newWorker->setLoad(newWorker->getLoad(end.date().year(), end.date().month()) + load2, end.date().year(), end.date().month());
            newWorker->setBonus(newWorker->getBonus(end.date().year(), end.date().month()) + bonus2, end.date().year(), end.date().month());
            newWorker->setLoad(newWorker->getLoad(year, month) + load1, year, month);
            newWorker->setBonus(newWorker->getBonus(year, month) + bonus1, year, month);
        } else {
            oldWorker->setLoad(oldWorker->getLoad(year, month) - load1, year, month);
            oldWorker->setBonus(oldWorker->getBonus(year, month) - bonus1, year, month);
            newWorker->setLoad(newWorker->getLoad(year, month) + load1, year, month);
            newWorker->setBonus(newWorker->getBonus(year, month) + bonus1, year, month);
        }
        query = this->oDatabase->query();
        if(workerNumber == 0) {
            query.prepare("update entries set worker_name_1=:WorkerName, worker_id_1=:WorkerId where id=:Id");
            query.bindValue(":Id", entryId);
            query.bindValue(":WorkerName", newWorker->name());
            query.bindValue(":WorkerId", newWorker->id());
            query.exec();
        } else {
            query.prepare("update entries set worker_name_2=:WorkerName, worker_id_2=:WorkerId where id=:Id");
            query.bindValue(":Id", entryId);
            query.bindValue(":WorkerName", newWorker->name());
            query.bindValue(":WorkerId", newWorker->id());
            query.exec();
        }
        int prevYear = this->oDate.year();
        int prevMonth = this->oDate.month() - 1;
        if(this->oDate.month() == 1) {
            prevYear--;
            prevMonth = 12;
        }
        QDate previous(prevYear, prevMonth, 1);
        int nextYear = this->oDate.year();
        int nextMonth = this->oDate.month() + 1;
        if(this->oDate.month() == 12) {
            nextYear++;
            nextMonth = 1;
        }
        QDate next(nextYear, nextMonth, 1);
        this->populateShifts(previous, next);
        emit this->columnCountChanged();
        emit this->columnsChanged();
    }
}

void ScheduleStore::exportSchedule(const QString &filename)
{
    QString clean = QString(filename);
    clean = clean.replace("file:///", "");
    QXlsx::Document xlsx;
    xlsx.setDocumentProperty("title", "WorkSchedule");
    xlsx.setDocumentProperty("creator", "WorkSchedule");
    xlsx.setSheetName(this->sMonthName);
    xlsx.setColumn(1, 1, 25.0);
    QXlsx::Format format;
    format.setFontColor(QColor(35, 35, 35));
    format.setFontSize(18);
    format.setBorderStyle(QXlsx::Format::BorderThick);
    format.setPatternBackgroundColor(QColor(22, 101, 73));
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format.setFontBold(true);
    format.setBorderColor(QColor(64, 64, 64));
    QXlsx::Format format2;
    format2.setFontColor(QColor(35, 35, 35));
    format2.setFontSize(14);
    format2.setBorderStyle(QXlsx::Format::BorderThick);
    format2.setPatternBackgroundColor(QColor(Qt::white));
    format2.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format2.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format2.setFontBold(true);
    format2.setBorderColor(QColor(64, 64, 64));
    QXlsx::Format format3;
    format3.setFontColor(QColor(35, 35, 35));
    format3.setFontSize(18);
    format3.setBorderStyle(QXlsx::Format::BorderThick);
    format3.setPatternBackgroundColor(QColor(27, 126, 90));
    format3.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format3.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format3.setFontBold(true);
    format3.setBorderColor(QColor(64, 64, 64));
    QXlsx::Format format4;
    format4.setFontColor(QColor(35, 35, 35));
    format4.setFontSize(12);
    format4.setBorderStyle(QXlsx::Format::BorderThick);
    format4.setPatternBackgroundColor(QColor(Qt::white));
    format4.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    format4.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format4.setTextWarp(true);
    format4.setBorderColor(QColor(64, 64, 64));
    xlsx.write("A1", this->sMonthName);
    xlsx.write("A3", this->tr("Monday"));
    xlsx.write("A6", this->tr("Tuesday"));
    xlsx.write("A9", this->tr("Wednesday"));
    xlsx.write("A12", this->tr("Thursday"));
    xlsx.write("A15", this->tr("Friday"));
    xlsx.write("A18", this->tr("Saturday"));
    xlsx.write("A21", this->tr("Sunday"));
    xlsx.mergeCells("A1:P1", format);
    xlsx.mergeCells("A3:A5", format2);
    xlsx.mergeCells("A6:A8", format2);
    xlsx.mergeCells("A9:A11", format2);
    xlsx.mergeCells("A12:A14", format2);
    xlsx.mergeCells("A15:A17", format2);
    xlsx.mergeCells("A18:A20", format);
    xlsx.mergeCells("A21:A23", format3);
    QStringList letters;
    letters.append("B");
    letters.append("C");
    letters.append("D");
    letters.append("E");
    letters.append("F");
    letters.append("G");
    letters.append("H");
    letters.append("I");
    letters.append("J");
    letters.append("K");
    letters.append("L");
    letters.append("M");
    letters.append("N");
    letters.append("O");
    letters.append("P");
    letters.append("Q");
    letters.append("R");
    letters.append("S");
    for(int i=0;i<this->aColumns.count();i++) {
        ScheduleColumn *current = qobject_cast<ScheduleColumn *>(this->aColumns[i]);
        QString merge = QString(letters[i*3]).append("%1:").append(letters[i*3]).append("%2");
        xlsx.mergeCells(merge.arg("3").arg("5"), this->aDayColors[i*7 + 0] == "white" ? format2 : format3);
        xlsx.mergeCells(merge.arg("6").arg("8"), this->aDayColors[i*7 + 1] == "white" ? format2 : format3);
        xlsx.mergeCells(merge.arg("9").arg("11"), this->aDayColors[i*7 + 2] == "white" ? format2 : format3);
        xlsx.mergeCells(merge.arg("12").arg("14"), this->aDayColors[i*7 + 3] == "white" ? format2 : format3);
        xlsx.mergeCells(merge.arg("15").arg("17"), this->aDayColors[i*7 + 4] == "white" ? format2 : format3);
        xlsx.mergeCells(merge.arg("18").arg("20"), format);
        xlsx.mergeCells(merge.arg("21").arg("23"), format3);
        xlsx.setColumn(2 + i*3, 2 + i*3, 13.0);
        xlsx.write(QString(letters[i*3]).append("3"), this->aDayNumbers[i*7 + 0]);
        xlsx.write(QString(letters[i*3]).append("6"), this->aDayNumbers[i*7 + 1]);
        xlsx.write(QString(letters[i*3]).append("9"), this->aDayNumbers[i*7 + 2]);
        xlsx.write(QString(letters[i*3]).append("12"), this->aDayNumbers[i*7 + 3]);
        xlsx.write(QString(letters[i*3]).append("15"), this->aDayNumbers[i*7 + 4]);
        xlsx.write(QString(letters[i*3]).append("18"), this->aDayNumbers[i*7 + 5]);
        xlsx.write(QString(letters[i*3]).append("21"), this->aDayNumbers[i*7 + 6]);
        int k = 3;
        for(int j=0;j<current->rowCount();j++) {
            ScheduleRow *row = qobject_cast<ScheduleRow *>(current->aRows[j]);
            if(row->workerCount() == 1) {
                int duration = row->duration();
                while(duration - 24 > 0) {
                    xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), "", format2);
                    xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), "", format2);
                    k++;
                    xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), "", format2);
                    xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), "", format2);
                    k++;
                    xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), "", format2);
                    xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), "", format2);
                    k++;
                    duration -= 24;
                }
                while(duration - 9 > 0) {
                    xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), "", format2);
                    xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), "", format2);
                    k++;
                    duration -= 24;
                }
                if(duration != 0) {
                    xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), "", format2);
                    xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), "", format2);
                    k++;
                }
            } else {
                if(row->isStartEndEqual()) {
                    xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), row->workers().at(0), format2);
                    xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), row->workers().at(1), format2);
                    k++;
                } else {
                    if(k == 3 || k == 23) {
                        xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), row->workers().at(0), format2);
                        xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), row->workers().at(1), format2);
                        k++;
                    } else {
                        xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), row->workers().at(0), format2);
                        xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), row->workers().at(1), format2);
                        k++;
                        xlsx.write(QString("%1%2").arg(letters[i*3+1]).arg(k), row->workers().at(0), format2);
                        xlsx.write(QString("%1%2").arg(letters[i*3+2]).arg(k), row->workers().at(1), format2);
                        k++;
                    }
                }
            }
        }
    }
    xlsx.write("A25", this->tr("NOTES:"));
    xlsx.mergeCells("A25:A26", format);
    xlsx.write("A27", this->sNote);
    xlsx.mergeCells("A27:H30", format4);
    QImage i(":/ElektroCeljeLogotipGS.jpg");
    xlsx.insertImage(24, 9, i);
    xlsx.saveAs(clean);
}

QString ScheduleStore::note() const
{
    return this->sNote;
}

void ScheduleStore::setNote(const QString &note)
{
    this->sNote = note;
    QSqlQuery query = this->oDatabase->query();
    query.prepare("update notes set data=:Data where year=:Year and month=:Month");
    query.bindValue(":Year", this->oDate.year());
    query.bindValue(":Month", this->oDate.month());
    query.bindValue(":Data", note);
    query.exec();
    emit this->noteChanged();
}

void ScheduleStore::setDate(int year, int month)
{
    this->oDate.setDate(year, month, 1);
    int prevYear = this->oDate.year();
    int prevMonth = this->oDate.month() - 1;
    if(this->oDate.month() == 1) {
        prevYear--;
        prevMonth = 12;
    }
    QDate previous(prevYear, prevMonth, 1);
    int nextYear = this->oDate.year();
    int nextMonth = this->oDate.month() + 1;
    if(this->oDate.month() == 12) {
        nextYear++;
        nextMonth = 1;
    }
    QDate next(nextYear, nextMonth, 1);
    this->getNote();
    this->populateDayNumbers(previous, next);
    this->populateDayColors(previous, next);
    this->populateTitle(year, month);
    this->populateShifts(previous, next);
    emit this->dayNumbersChanged();
    emit this->dayColorsChanged();
    emit this->columnCountChanged();
    emit this->columnsChanged();
}

bool ScheduleStore::canGenerateSchedule()
{
    return this->aSchedules.count() < 3;
}

void ScheduleStore::revertSchedule(int year, int month)
{
    QList<Worker *> workers = this->oWorkers->getData();
    Entry *beginning = this->aSchedules.first();
    Entry *ending = this->aSchedules.last();
    for(int i=0;i<workers.count();i++) {
        workers[i]->setHours(0, year, month);
        workers[i]->setLoad(0, year, month);
        workers[i]->setBonus(0, year, month);
        workers[i]->setSelect(true);
        if(workers[i]->id() == beginning->id1() || workers[i]->id() == beginning->id2()) {
            workers[i]->setBonus(workers[i]->getBonus(year, month) + beginning->bonus(), year, month);
            workers[i]->setLoad(workers[i]->getLoad(year, month) + beginning->load(), year, month);
        }
        if(ending->end().date().month() != month) {
            if(workers[i]->id() == ending->id1() || workers[i]->id() == ending->id2()) {
                workers[i]->setBonus(workers[i]->getBonus(year, month) + ending->bonus(), year, month);
                workers[i]->setLoad(workers[i]->getLoad(year, month) + ending->load(), year, month);
            }
        }
    }
    int startIndex = 1;
    int numberOfShifts = this->aSchedules.count() - 1;
    if(ending->end().date().month() != month)
        numberOfShifts--;
    for(int i=numberOfShifts;i>=startIndex;i--) {
        this->oDatabase->exec(QString("delete from entries where id=%1").arg(this->aSchedules[i]->id()));
        delete this->aSchedules[i];
        this->aSchedules.removeAt(i);
    }
}

QObject *ScheduleStore::generateSchedule(int year, int month, int maxHours)
{
    emit this->generating();
    if(this->aSchedules.count() >= 3) {
        this->revertSchedule(year, month);
    }
    int numberOfShifts = this->oDate.daysInMonth() * 2 + 1;
    QDateTime start(QDate(year, month, 1), QTime(0,0,0));
    start = start.addSecs(-1 * 60 * 60 * 5);
    QDateTime end(QDate(year, month, 1), QTime(0,0,0));
    end = end.addSecs(60 * 60 * 7);
    int startIndex = 0;
    if(this->aSchedules.count() > 0 && this->aSchedules.first()->end().date().month() == month) {
        startIndex++;
        start = start.addSecs(60 * 60 * 12);
        end = end.addSecs(60 * 60 * 12);
    }
    bool append = true;
    if(this->aSchedules.count() > 0 && this->aSchedules.last()->start().date().month() == month) {
        numberOfShifts--;
        append = false;
    }
    //handle event hour calculation
    QList<Worker *> workers = this->oWorkers->getData();
    QDateTime eventStart(QDate(year, month, 1), QTime(0,0,0));
    QDateTime eventEnd(QDate(year, month, 1), QTime(23,59,59));
    int days = eventStart.date().daysInMonth();
    for(int i=0;i<days;i++) {
        for(int j=0;j<workers.count();j++) {
            if(!workers[j]->canSelect(eventStart, eventEnd))
                workers[j]->setHours(workers[j]->getHours(year, month) + 8, year, month);
        }
        eventStart = eventStart.addSecs(60 * 60 * 24);
        eventEnd = eventEnd.addSecs(60 * 60 * 24);
    }
    QDateTime scheduleStart(QDate(year, month, 1), QTime(0,0,0));
    QDateTime scheduleEnd(QDate(year, month, scheduleStart.date().daysInMonth()), QTime(23,59,59));
    Worker *prevWorker1 = 0;
    Worker *prevWorker2 = 0;
    for(int i=startIndex;i<numberOfShifts;i++) {
        Worker *worker1 = 0;
        Worker *worker2 = 0;
        int load1 = 0;
        int bonus1 = 0;
        int hours1 = 0;
        int load2 = 0;
        int bonus2 = 0;
        int hours2 = 0;
        int tBonus = 0;
        int tLoad = 0;
        if(start < scheduleStart) {
            load1 = 5;
            bonus1 = 0;
            hours1 = 5;
            load2 = 7;
            bonus2 = 0;
            hours2 = 7;
            QDateTime d(start.date(), start.time());
            for(int j=0;j<12;j++) {
                if(d < scheduleStart) {
                    if(d.date().dayOfWeek() == 7 || this->oHolidays->isHoliday(d.date()))
                        bonus1 += 1;
                    if(d.time() >= QTime(15, 0) || d.time() <= QTime(7, 0))
                        bonus1 += 2;
                    if(d.time() >= QTime(22, 0) || d.time() <= QTime(7, 0))
                        bonus1 += 3;
                } else {
                    if(d.date().dayOfWeek() == 7 || this->oHolidays->isHoliday(d.date()))
                        bonus2 += 1;
                    if(d.time() >= QTime(15, 0) || d.time() <= QTime(7, 0))
                        bonus2 += 2;
                    if(d.time() >= QTime(22, 0) || d.time() <= QTime(7, 0))
                        bonus2 += 3;
                }
                d = d.addSecs(60 * 60);
            }
            tBonus = bonus2;
            tLoad = load2;
            worker1 = this->getBestWorker(load2 + load1, bonus2 + bonus1, hours2 + hours1, maxHours, start, end);
            if(worker1 == 0) {
                this->revertSchedule(year, month);
                emit this->generated();
                return this->setError(QObject::tr("Schedule generation failed, constraints not satisfied. Try increasing max hours or adding more workers."));
            }
            worker1->setSelect(false);
            worker1->setBonus(worker1->getBonus(year, month) + bonus2, year, month);
            worker1->setLoad(worker1->getLoad(year, month) + load2, year, month);
            //worker1->setHours(worker1->getHours(year, month) + hours2, year, month);
            worker1->setBonus(worker1->getBonus(start.date().year(), start.date().month()) + bonus1, start.date().year(), start.date().month());
            worker1->setLoad(worker1->getLoad(start.date().year(), start.date().month()) + load1, start.date().year(), start.date().month());
            //worker1->setHours(worker1->getHours(start.date().year(), start.date().month()) + hours1, start.date().year(), start.date().month());
            worker2 = this->getBestWorker(load2 + load1, bonus2 + bonus1, hours2 + hours1, maxHours, start, end);
            if(worker2 == 0) {
                this->revertSchedule(year, month);
                emit this->generated();
                return this->setError(QObject::tr("Schedule generation failed, constraints not satisfied. Try increasing max hours or adding more workers."));
            }
            worker2->setSelect(false);
            worker2->setBonus(worker2->getBonus(year, month) + bonus2, year, month);
            worker2->setLoad(worker2->getLoad(year, month) + load2, year, month);
            //worker2->setHours(worker2->getHours(year, month) + hours2, year, month);
            worker2->setBonus(worker2->getBonus(start.date().year(), start.date().month()) + bonus1, start.date().year(), start.date().month());
            worker2->setLoad(worker2->getLoad(start.date().year(), start.date().month()) + load1, start.date().year(), start.date().month());
            //worker2->setHours(worker2->getHours(start.date().year(), start.date().month()) + hours1, start.date().year(), start.date().month());
        } else if(end > scheduleEnd) {
            load1 = 5;
            bonus1 = 0;
            hours1 = 5;
            load2 = 7;
            bonus2 = 0;
            hours2 = 7;
            QDateTime d(start.date(), start.time());
            for(int j=0;j<12;j++) {
                if(d < scheduleEnd) {
                    if(d.date().dayOfWeek() == 7 || this->oHolidays->isHoliday(d.date()))
                        bonus1 += 1;
                    if(d.time() >= QTime(15, 0) || d.time() <= QTime(7, 0))
                        bonus1 += 2;
                    if(d.time() >= QTime(22, 0) || d.time() <= QTime(7, 0))
                        bonus1 += 3;
                } else {
                    if(d.date().dayOfWeek() == 7 || this->oHolidays->isHoliday(d.date()))
                        bonus2 += 1;
                    if(d.time() >= QTime(15, 0) || d.time() <= QTime(7, 0))
                        bonus2 += 2;
                    if(d.time() >= QTime(22, 0) || d.time() <= QTime(7, 0))
                        bonus2 += 3;
                }
                d = d.addSecs(60 * 60);
            }
            tBonus = bonus1;
            tLoad = load1;
            worker1 = this->getBestWorker(load1 + load2, bonus1 + bonus2, hours1 + hours2, maxHours, start, end);
            if(worker1 == 0) {
                this->revertSchedule(year, month);
                emit this->generated();
                return this->setError(QObject::tr("Schedule generation failed, constraints not satisfied. Try increasing max hours or adding more workers."));
            }
            worker1->setSelect(false);
            worker1->setBonus(worker1->getBonus(year, month) + bonus1, year, month);
            worker1->setLoad(worker1->getLoad(year, month) + load1, year, month);
            //worker1->setHours(worker1->getHours(year, month) + hours1, year, month);
            worker1->setBonus(worker1->getBonus(end.date().year(), end.date().month()) + bonus2, end.date().year(), end.date().month());
            worker1->setLoad(worker1->getLoad(end.date().year(), end.date().month()) + load2, end.date().year(), end.date().month());
            //worker1->setHours(worker1->getHours(end.date().year(), end.date().month()) + hours2, end.date().year(), end.date().month());
            worker2 = this->getBestWorker(load1 + load2, bonus1 + bonus2, hours1 + hours2, maxHours, start, end);
            if(worker2 == 0) {
                this->revertSchedule(year, month);
                emit this->generated();
                return this->setError(QObject::tr("Schedule generation failed, constraints not satisfied. Try increasing max hours or adding more workers."));
            }
            worker2->setSelect(false);
            worker2->setBonus(worker2->getBonus(year, month) + bonus1, year, month);
            worker2->setLoad(worker2->getLoad(year, month) + load1, year, month);
            //worker2->setHours(worker2->getHours(year, month) + hours1, year, month);
            worker2->setBonus(worker2->getBonus(end.date().year(), end.date().month()) + bonus2, end.date().year(), end.date().month());
            worker2->setLoad(worker2->getLoad(end.date().year(), end.date().month()) + load2, end.date().year(), end.date().month());
            //worker2->setHours(worker2->getHours(end.date().year(), end.date().month()) + hours2, end.date().year(), end.date().month());
        } else {
            load1 = 12;
            hours1 = 12;
            QDateTime d(start.date(), start.time());
            for(int j=0;j<12;j++) {
                if(d.date().dayOfWeek() == 7 || this->oHolidays->isHoliday(d.date()))
                    bonus1 += 1;
                if(d.time() >= QTime(15, 0) || d.time() <= QTime(7, 0))
                    bonus1 += 2;
                if(d.time() >= QTime(22, 0) || d.time() <= QTime(7, 0))
                    bonus1 += 3;
                d = d.addSecs(60 * 60);
            }
            tBonus = bonus1;
            tLoad = load1;
            worker1 = this->getBestWorker(load1, bonus1, hours1, maxHours, start, end);
            if(worker1 == 0) {
                this->revertSchedule(year, month);
                emit this->generated();
                return this->setError(QObject::tr("Schedule generation failed, constraints not satisfied. Try increasing max hours or adding more workers."));
            }
            worker1->setSelect(false);
            worker1->setBonus(worker1->getBonus(year, month) + bonus1, year, month);
            worker1->setLoad(worker1->getLoad(year, month) + load1, year, month);
            //worker1->setHours(worker1->getHours(year, month) + hours, year, month);
            worker2 = this->getBestWorker(load1, bonus1, hours1, maxHours, start, end);
            if(worker2 == 0) {
                this->revertSchedule(year, month);
                emit this->generated();
                return this->setError(QObject::tr("Schedule generation failed, constraints not satisfied. Try increasing max hours or adding more workers."));
            }
            worker2->setSelect(false);
            worker2->setBonus(worker2->getBonus(year, month) + bonus1, year, month);
            worker2->setLoad(worker2->getLoad(year, month) + load1, year, month);
            //worker2->setHours(worker2->getHours(year, month) + hours, year, month);
        }
        QSqlQuery query = this->oDatabase->query();
        query.prepare("insert into entries(worker_id_1, worker_name_1, worker_id_2, worker_name_2, hours1, bonus1, load1, hours2, bonus2, load2, start, end) values(:Id1, :Name1, :Id2, :Name2, :Hours1, :Bonus1, :Load1, :Hours2, :Bonus2, :Load2, :Start, :End)");
        query.bindValue(":Id1", worker1 == 0 ? -1 : worker1->id());
        query.bindValue(":Name1", worker1 == 0 ? "" : worker1->name());
        query.bindValue(":Id2", worker2 == 0 ? -1 : worker2->id());
        query.bindValue(":Name2", worker2 == 0 ? "" : worker2->name());
        query.bindValue(":Hours1", 0);
        query.bindValue(":Bonus1", bonus1);
        query.bindValue(":Load1", load1);
        query.bindValue(":Hours2", 0);
        query.bindValue(":Bonus2", bonus2);
        query.bindValue(":Load2", load2);
        query.bindValue(":Start", start.toString("yyyy-MM-dd HH:mm:ss"));
        query.bindValue(":End", end.toString("yyyy-MM-dd HH:mm:ss"));
        query.exec();
        int id = query.lastInsertId().toInt();
        if(append)
            this->aSchedules.append(new Entry(id, worker1->id(), worker2->id(), worker1->name(), worker2->name(), tLoad, 0, tBonus, start, end));
        else
            this->aSchedules.insert(this->aSchedules.count() - 1, new Entry(id, worker1->id(), worker2->id(), worker1->name(), worker2->name(), tLoad, 0, tBonus, start, end));
        start = start.addSecs(60 * 60 * 12);
        end = end.addSecs(60 * 60 * 12);
        if(prevWorker1 != 0)
            prevWorker1->setSelect(true);
        if(prevWorker2 != 0)
            prevWorker2->setSelect(true);
        prevWorker1 = worker1;
        prevWorker2 = worker2;
    }
    if(prevWorker1 != 0)
        prevWorker1->setSelect(true);
    if(prevWorker2 != 0)
        prevWorker2->setSelect(true);
    int prevYear = this->oDate.year();
    int prevMonth = this->oDate.month() - 1;
    if(this->oDate.month() == 1) {
        prevYear--;
        prevMonth = 12;
    }
    QDate previous(prevYear, prevMonth, 1);
    int nextYear = this->oDate.year();
    int nextMonth = this->oDate.month() + 1;
    if(this->oDate.month() == 12) {
        nextYear++;
        nextMonth = 1;
    }
    QDate next(nextYear, nextMonth, 1);
    this->populateShifts(previous, next);
    emit this->columnCountChanged();
    emit this->columnsChanged();
    emit this->generated();
    return this->setNoError();
}

Worker *ScheduleStore::getBestWorker(int load, int bonus, int hours, int maximumHours, const QDateTime &start, const QDateTime &end)
{
    int year = this->oDate.year();
    int month = this->oDate.month();
    QList<Worker *> workers = this->oWorkers->getData();
    QList<double> loadData;
    QList<double> bonusData;
    QList<double> hoursData;
    double maxLoad, maxBonus, maxHours;
    for(int i=0;i<workers.count();i++) {
        loadData.append(workers[i]->getLoad(year, month));
        bonusData.append(workers[i]->getBonus(year, month));
        hoursData.append(workers[i]->getHours(year, month));
        if(i == 0) {
            maxLoad = loadData.last();
            maxBonus = bonusData.last();
            maxHours = hoursData.last();
        } else {
            if(loadData.last() > maxLoad)
                maxLoad = loadData.last();
            if(bonusData.last() > maxBonus)
                maxBonus = bonusData.last();
            if(hoursData.last() > maxHours)
                maxHours = hoursData.last();
        }
    }
    double loadSum = 0, bonusSum = 0, hoursSum = 0;
    for(int i=0;i<workers.count();i++) {
        if(maxLoad > 0)
            loadData[i] /= maxLoad;
        if(maxBonus > 0)
            bonusData[i] /= maxBonus;
        if(maxHours > 0)
            hoursData[i] /= maxHours;
        loadSum += loadData[i];
        bonusSum += bonusData[i];
        hoursSum += hoursData[i];
    }
    double loadNorm = load, bonusNorm = bonus, hoursNorm = hours;
    if(maxLoad > 0)
        loadNorm /= maxLoad;
    if(maxBonus > 0)
        bonusNorm /= maxBonus;
    if(maxHours > 0)
        hoursNorm /= maxHours;
    double loadAvg = loadSum + loadNorm / (double)workers.count();
    double bonusAvg = bonusSum + bonusNorm / (double)workers.count();
    double hoursAvg = hoursSum + hoursNorm / (double)workers.count();
    double minDist = 0.0;
    int minIdx = -1;
    for(int i=0;i<workers.count();i++) {
        if(!workers[i]->canSelect(start, end)) continue;
        if(workers[i]->getLoad(year, month) + load + workers[i]->getHours(year, month) > maximumHours) continue;
        double devSumLoad = qPow(loadData[i] + loadNorm - loadAvg, 2);
        double devSumBonus = qPow(bonusData[i] + bonusNorm - bonusAvg, 2);
        double devSumHours = qPow(hoursData[i] + hoursNorm - hoursAvg, 2);
        for(int j=0;j<workers.count();j++) {
            if(j == i) continue;
            devSumLoad += qPow(loadData[j] - loadAvg, 2);
            devSumBonus += qPow(bonusData[j] - bonusAvg, 2);
            devSumHours += qPow(hoursData[i] - hoursAvg, 2);
        }
        devSumLoad *= 1.0 / (double)(workers.count() - 1);
        devSumBonus *= 1.0 / (double)(workers.count() - 1);
        devSumHours *= 1.0 / (double)(workers.count() - 1);
        double dist = qSqrt(qPow(qSqrt(devSumLoad), 2) + qPow(qSqrt(devSumBonus), 2) + qPow(qSqrt(devSumHours), 2));
        if(minIdx == -1) {
            minDist = dist;
            minIdx = i;
        } else if(dist < minDist) {
            minDist = dist;
            minIdx = i;
        }
    }
    if(minIdx == -1)
        return 0;
    return workers[minIdx];
}

void ScheduleStore::setMonthName(const QString &name)
{
    if(name == this->sMonthName)
        return;
    this->sMonthName = name;
    emit this->monthNameChanged();
}

QString ScheduleStore::monthName() const
{
    return this->sMonthName;
}

int ScheduleStore::columnCount() const
{
    return this->aColumns.count();
}

QList<QObject *> ScheduleStore::columns() const
{
    return this->aColumns;
}

QStringList ScheduleStore::dayColors() const
{
    return this->aDayColors;
}
QStringList ScheduleStore::dayNumbers() const
{
    return this->aDayNumbers;
}
