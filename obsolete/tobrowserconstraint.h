
/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * TOra - An Oracle Toolkit for DBA's and developers
 * 
 * Shared/mixed copyright is held throughout files in this product
 * 
 * Portions Copyright (C) 2000-2001 Underscore AB
 * Portions Copyright (C) 2003-2005 Quest Software, Inc.
 * Portions Copyright (C) 2004-2013 Numerous Other Contributors
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program as the file COPYING.txt; if not, please see
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 * 
 *      As a special exception, you have permission to link this program
 *      with the Oracle Client libraries and distribute executables, as long
 *      as you follow the requirements of the GNU GPL in regard to all of the
 *      software in the executable aside from Oracle client libraries.
 * 
 * All trademarks belong to their respective owners.
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef TOBROWSERCONSTRAINT_H
#define TOBROWSERCONSTRAINT_H


#include "ui_tobrowserconstraintui.h"
//obsolete #include "core/toextract.h"
#include "core/totool.h"

#include <map>
#include <list>

class toBrowserConstraint : public QDialog, public Ui::toBrowserConstraintUI, public toConnectionWidget
{
private:
    Q_OBJECT

    QString Owner;
    QString Table;

    //obsolete toExtract Extractor;

    std::map<QString, std::list<QString> > OriginalDescription;
    std::map<QString, std::list<QString> > NewDescription;

    QString Current;
    QString BeforeUnquote;  // Hack for not incurring differences with weird MySQL quoting
    QString AfterUnquote;
    std::map<QString, QString> Constraints;

    toBrowserConstraint(toConnection &conn, const QString &owner, const QString &table, QWidget *parent, const char *name = NULL);

    QString table();
    void parseConstraint(const QString &definition);

    void saveChanges();
    void describeTable(const QString &table);

    QString sql(void);
public:
    static void modifyConstraint(toConnection &conn, const QString &owner, const QString &table, QWidget *parent);
public slots:
    virtual void displaySQL();
    virtual void addConstraint();
    virtual void delConstraint();
    virtual void changeConstraint();
    virtual void changeRefered();
    virtual void changeType(int type);
    virtual void changeTable(const QString &table);
};

#endif
