/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef REASSIGNCLASSCODEBYCLASSCODE_H
#define REASSIGNCLASSCODEBYCLASSCODE_H

#include "guiclient.h"
#include "xdialog.h"
#include "ui_reassignClassCodeByClassCode.h"

class reassignClassCodeByClassCode : public XDialog, public Ui::reassignClassCodeByClassCode
{
    Q_OBJECT

public:
    reassignClassCodeByClassCode(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~reassignClassCodeByClassCode();

public slots:
    virtual void sReassign();

protected slots:
    virtual void languageChange();

};

#endif // REASSIGNCLASSCODEBYCLASSCODE_H
