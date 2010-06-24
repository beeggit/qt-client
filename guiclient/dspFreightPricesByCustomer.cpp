/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dspFreightPricesByCustomer.h"

#include <openreports.h>
#include <parameter.h>
#include <metasql.h>

#include "mqlutil.h"

dspFreightPricesByCustomer::dspFreightPricesByCustomer(QWidget* parent, const char* name, Qt::WFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

//  (void)statusBar();

  connect(_print, SIGNAL(clicked()), this, SLOT(sPrint()));
  connect(_query, SIGNAL(clicked()), this, SLOT(sFillList()));

  _price->addColumn(tr("Schedule"),      _itemColumn,     Qt::AlignLeft,   true,  "ipshead_name"  );
  _price->addColumn(tr("Source"),        _itemColumn,     Qt::AlignLeft,   true,  "source"  );
  _price->addColumn(tr("Qty. Break"),    _qtyColumn,      Qt::AlignRight,  true,  "ipsfreight_qtybreak" );
  _price->addColumn(tr("Price"),         _priceColumn,    Qt::AlignRight,  true,  "ipsfreight_price" );
  _price->addColumn(tr("Method"),        _itemColumn,     Qt::AlignLeft,   true,  "method"  );
  _price->addColumn(tr("Currency"),      _currencyColumn, Qt::AlignLeft,   true,  "currConcat");
  _price->addColumn(tr("From"),          _itemColumn,     Qt::AlignLeft,   true,  "warehous_code"  );
  _price->addColumn(tr("To"),            _itemColumn,     Qt::AlignLeft,   true,  "shipzone_name"  );
  _price->addColumn(tr("Freight Class"), _itemColumn,     Qt::AlignLeft,   true,  "freightclass_code"  );
  _price->addColumn(tr("Ship Via"),      _itemColumn,     Qt::AlignLeft,   true,  "ipsfreight_shipvia"  );

}

dspFreightPricesByCustomer::~dspFreightPricesByCustomer()
{
  // no need to delete child widgets, Qt does it all for us
}

void dspFreightPricesByCustomer::languageChange()
{
  retranslateUi(this);
}

void dspFreightPricesByCustomer::sPrint()
{
  ParameterList params;

  params.append("cust_id", _cust->id());

  if(_showExpired->isChecked())
    params.append("showExpired");
  if(_showFuture->isChecked())
    params.append("showFuture");

  orReport report("FreightPricesByCustomer", params);
  if (report.isValid())
    report.print();
  else
    report.reportError(this);
}

void dspFreightPricesByCustomer::sFillList()
{
  _price->clear();

  if (_cust->isValid())
  {
    ParameterList params;

    if (! setParams(params))
      return;

    MetaSQLQuery mql = mqlLoad("freightPrices", "detail");
    q = mql.toQuery(params);
    _price->populate(q, true);
  }
}

bool dspFreightPricesByCustomer::setParams(ParameterList &params)
{
  params.append("byCust");

  params.append("na", tr("N/A"));
  params.append("any", tr("Any"));
  params.append("flatrate", tr("Flat Rate"));
  params.append("peruom", tr("Per UOM"));
  params.append("customer", tr("Customer"));
  params.append("custType", tr("Cust. Type"));
  params.append("custTypePattern", tr("Cust. Type Pattern"));
  params.append("sale", tr("Sale"));

  if (_cust->isValid())
    params.append("cust_id", _cust->id());

  if (!_showExpired->isChecked())
    params.append("showExpired");

  if (!_showFuture->isChecked())
    params.append("showFuture");

  return true;
}
