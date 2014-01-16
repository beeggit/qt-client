/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef OPPORTUNITYSOURCE_H
#define OPPORTUNITYSOURCE_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>
#include "ui_opportunitySource.h"

class opportunitySource : public XDialog, public Ui::opportunitySource
{
    Q_OBJECT

public:
    opportunitySource(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~opportunitySource();

    virtual void populate();

public slots:
    virtual SetResponse set( const ParameterList & pParams );
    virtual void sCheck();
    virtual void sSave();

protected slots:
    virtual void languageChange();

private:
    int _mode;
    int _opsourceid;

};

#endif // OPPORTUNITYSOURCE_H
