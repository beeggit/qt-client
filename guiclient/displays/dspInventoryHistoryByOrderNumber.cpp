/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dspInventoryHistoryByOrderNumber.h"

dspInventoryHistoryByOrderNumber::dspInventoryHistoryByOrderNumber(QWidget* parent, const char*, Qt::WFlags fl)
  : dspInventoryHistoryBase(parent, "dspInventoryHistoryByOrderNumber", fl)
{
  setWindowTitle(tr("Inventory History by Order Number"));
  _parameter->hide();
  _itemGroup->hide();
}
