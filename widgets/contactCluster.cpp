/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>

#include "contactcluster.h"
#include "contactwidget.h"

ContactClusterLineEdit::ContactClusterLineEdit(QWidget* pParent, const char* pName) :
    VirtualClusterLineEdit(pParent, "cntct", "cntct_id", "formatCntctName(cntct_id)", 0, "cntct_title", 0, pName)
{
    setTitles(tr("Contact"), tr("Contacts"));
    setUiName("contact");
    setEditPriv("MaintainContacts");
    setViewPriv("ViewContacts");
}

ContactCluster::ContactCluster(QWidget* pParent, const char* pName) :
    VirtualCluster(pParent, pName)
{
  addNumberWidget(new ContactClusterLineEdit(this, pName));

  _minLayout = true;
  _fname = new QStringList();
  for (int i = 0; i < 5; ++i)
    _fname->append("");

  _name->setVisible(true);
  _description->setVisible(true);

  _cntctSpacer = new QSpacerItem(15, 3,QSizePolicy::Fixed,QSizePolicy::Fixed);
  _grid->addItem(_cntctSpacer, 1, 1, 1, 1);

  _grid->removeWidget(_description);
  _grid->removeWidget(_name);
  _grid->setVerticalSpacing(0);

  setLabel(tr("Contact:"));
  _number->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed); // This is actually name here
  _titleLit = new QLabel(tr("Title:"),this);
  _phoneLit = new QLabel(tr("Phone:"),this);
  _phone2Lit = new QLabel(tr("Alternate:"),this);
  _faxLit = new QLabel(tr("Fax:"),this);
  _emailLit = new QLabel(tr("Email:"),this);
  _webaddrLit = new QLabel(tr("Web:"),this);

  _phone = new QLabel(this);
  _phone2 = new QLabel(this);
  _fax = new QLabel(this);
  _email = new XURLLabel(this);
  _webaddr = new XURLLabel(this);
  _addr = new QLabel(this);

  _titleLit->setObjectName("_titleLit");
  _phoneLit->setObjectName("_phoneLit");
  _phone2Lit->setObjectName("_phone2Lit");
  _faxLit->setObjectName("_faxLit");
  _emailLit->setObjectName("_emailLit");
  _webaddrLit->setObjectName("_webaddrLit");

  _phone->setObjectName("_phone");
  _phone2->setObjectName("_phone2");
  _fax->setObjectName("_fax");
  _email->setObjectName("_email");
  _webaddr->setObjectName("_webaddr");
  _addr->setObjectName("_addr");

  _number->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
  _phone->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
  _description->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
  _addr->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

  _titleLit->setAlignment(Qt::AlignRight);
  _phoneLit->setAlignment(Qt::AlignRight);
  _phone2Lit->setAlignment(Qt::AlignRight);
  _faxLit->setAlignment(Qt::AlignRight);
  _emailLit->setAlignment(Qt::AlignRight);
  _webaddrLit->setAlignment(Qt::AlignRight);

  _addrLayout = new QVBoxLayout;
  _addrSpacer = new QSpacerItem(20,0,QSizePolicy::Preferred,QSizePolicy::Expanding);
  _addrLayout->addWidget(_addr);
  _addrLayout->addSpacerItem(_addrSpacer);

  // Over-ride standard actions with local versions
  for (int i = 0; i < _number->actions().count(); i++)
    _number->removeAction(_number->actions().at(0));

  QKeySequence listKey = QKeySequence(tr("Ctrl+L"));
  QAction* listAct = new QAction(tr("List"), this);
  listAct->setShortcut(listKey);
  listAct->setShortcutContext(Qt::WidgetShortcut);
  listAct->setToolTip(tr("List all records"));
  connect(listAct, SIGNAL(triggered()), this, SLOT(sList()));
  _number->addAction(listAct);

  QKeySequence searchKey = QKeySequence(tr("Ctrl+S"));
  QAction* searchAct = new QAction(tr("Search"), this);
  searchAct->setShortcut(searchKey);
  searchAct->setShortcutContext(Qt::WidgetShortcut);
  searchAct->setToolTip(tr("Search on specific criteria"));
  connect(searchAct, SIGNAL(triggered()), this, SLOT(sSearch()));
  _number->addAction(searchAct);

  ContactClusterLineEdit* ccle = static_cast<ContactClusterLineEdit* >(_number);

  QMenu* menu = new QMenu;
  menu->addAction(listAct);
  menu->addAction(searchAct);
  menu->addSeparator();
  menu->addAction(ccle->_infoAct);
  _number->setMenu(menu);

  connect(this, SIGNAL(valid(bool)), this, SLOT(populate()));
  connect(_email, SIGNAL(leftClickedURL(QString)), this, SLOT(openUrl(QString)));
  connect(_webaddr, SIGNAL(leftClickedURL(QString)), this, SLOT(openUrl(QString)));
  connect(this, SIGNAL(valid(bool)), ccle->_infoAct, SLOT(setEnabled(bool)));

  setMinimalLayout(false);
  
  ccle->_numClause = QString(" AND ( "
                             " TRIM(REPLACE("
                             " COALESCE(cntct_honorific,'')        || ' '  || "
                             " COALESCE(cntct_first_name,'') || ' '  || "
                             " COALESCE(cntct_middle,'')     || ' '  || "
                             " COALESCE(cntct_last_name,'')  || ' '  || "
                             " COALESCE(cntct_suffix,'') "
                             ",'  ', ' '))"
                             " ~* :number )");
}

void ContactCluster::setMinimalLayout(bool isMinimal)
{
   if (isMinimal == _minLayout)
     return;

   _grid->removeWidget(_titleLit);
   _grid->removeWidget(_description);
   _grid->removeWidget(_phoneLit);
   _grid->removeWidget(_phone);
   _grid->removeWidget(_phone2Lit);
   _grid->removeWidget(_phone2);
   _grid->removeWidget(_emailLit) ;
   _grid->removeWidget(_email);
   _grid->removeWidget(_faxLit);
   _grid->removeWidget(_fax);
   _grid->removeWidget(_webaddrLit);
   _grid->removeWidget(_webaddr);
   _grid->removeWidget(_addr);

   if (isMinimal) {
    _addr->hide();
    _grid->addWidget(_phoneLit, 2, 0, 1, 1);
    _grid->addWidget(_phone, 2, 1, 1 ,1);
    _grid->addWidget(_titleLit, 2, 2, 1, 1);
    _grid->addWidget(_description, 2, 3, 1, 1);
    _grid->addWidget(_phone2Lit, 3, 0, 1, 1);
    _grid->addWidget(_phone2, 3, 1, 1, 1);
    _grid->addWidget(_emailLit, 3, 2, 1, 1) ;
    _grid->addWidget(_email, 3, 3, 1, 1);
    _grid->addWidget(_faxLit, 4, 0, 1, 1);
    _grid->addWidget(_fax,	4, 1, 1, 1);
    _grid->addWidget(_webaddrLit, 4, 2, 1, 1) ;
    _grid->addWidget(_webaddr, 4, 3, 1, 1);
  }
  else {
    _addr->show();
    _grid->addWidget(_titleLit, 2, 0, 1, 1);
    _grid->addWidget(_description, 2, 1, 1, 2);
    _grid->addWidget(_phoneLit, 3, 0, 1, 1);
    _grid->addWidget(_phone, 3, 1, 1 ,1);
    _grid->addWidget(_phone2Lit, 4, 0, 1, 1);
    _grid->addWidget(_phone2, 4, 1, 1, 1);
    _grid->addWidget(_faxLit, 5, 0, 1, 1);
    _grid->addWidget(_fax, 5, 1, 1, 1);
    _grid->addWidget(_emailLit, 6, 0, 1, 1) ;
    _grid->addWidget(_email, 6, 1, 1, 1);
    _grid->addWidget(_webaddrLit, 7, 0, 1, 1) ;
    _grid->addWidget(_webaddr, 7, 1, 1, 1);
    _grid->addLayout(_addrLayout, 3, 2, 4, 1);
  }
  _minLayout = isMinimal;
}

void ContactCluster::setSearchAcct(int crmAcctId)
{
  _searchAcctId = crmAcctId;
  ContactClusterLineEdit* ccle = static_cast<ContactClusterLineEdit* >(_number);
  if (crmAcctId != -1)
    ccle->setExtraClause(QString(" (cntct_crmacct_id=%1)").arg(crmAcctId));
  else
    ccle->setExtraClause("");
}

void ContactCluster::populate()
{
  if (id() == -1) {
    _phone->clear();
    _phone2->clear();
    _fax->clear();
    _email->clear();
    _webaddr->clear();
  }
  else
  {
    if (_number->completer())
      disconnect(_number, SIGNAL(textChanged(QString)), _number, SLOT(sHandleCompleter()));

    XSqlQuery dataQ;
    dataQ.prepare("SELECT cntct_honorific, cntct_first_name, "
                  "  cntct_middle, cntct_last_name, cntct_suffix, "
                  "  cntct_phone, cntct_phone2, "
                  "  cntct_fax, cntct_email, cntct_webaddr, "
                  "  formatAddr(cntct_addr_id) AS address "
                  "FROM cntct "
                  "WHERE (cntct_id=:id);");
    dataQ.bindValue(":id", id());
    dataQ.exec();
    if (dataQ.first()) {
      setName(0, dataQ.value("cntct_honorific").toString());
      setName(1, dataQ.value("cntct_first_name").toString());
      setName(2, dataQ.value("cntct_middle").toString());
      setName(3, dataQ.value("cntct_last_name").toString());
      setName(4, dataQ.value("cntct_suffix").toString());
      _phone->setText(dataQ.value("cntct_phone").toString());
      _phone2->setText(dataQ.value("cntct_phone2").toString());
      _fax->setText(dataQ.value("cntct_fax").toString());
      _email->setText(dataQ.value("cntct_email").toString());
      _webaddr->setText(dataQ.value("cntct_webaddr").toString());
      _email->setURL("mailto:" + dataQ.value("cntct_email").toString());
      _webaddr->setURL("http://" + dataQ.value("cntct_webaddr").toString());
      _addr->setText(dataQ.value("address").toString());
    }
    if (_number->completer())
      connect(_number, SIGNAL(textChanged(QString)), _number, SLOT(sHandleCompleter()));
  }
}

void ContactCluster::openUrl(QString url)
{
  QDesktopServices::openUrl(QUrl(url));
}

void ContactCluster::sList()
{
  ContactList* newdlg = new ContactList(this);
  if (newdlg)
  {
    ParameterList params;
    params.append("titlePlural", tr("Contacts"));
    params.append("searchAcctId", searchAcctId());
    newdlg->set(params);
    int id = newdlg->exec();
    setId(id);
  }
  else
    QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
                          .arg(__FILE__)
                          .arg(__LINE__),
                          tr("Could not instantiate a List Dialog"));
}

void ContactCluster::sSearch()
{
  ContactSearch* newdlg = new ContactSearch(this);
  if (newdlg)
  {
    ParameterList params;
    params.append("titalPlural", tr("Contacts"));
    params.append("searchAcctId", searchAcctId());
    newdlg->set(params);
    int id = newdlg->exec();
    setId(id);
  }
  else
    QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
                          .arg(__FILE__)
                          .arg(__LINE__),
                          tr("Could not instantiate a Search Dialog"));
}

void ContactCluster::addNumberWidget(ContactClusterLineEdit* pNumberWidget)
{
    _number = pNumberWidget;
    if (! _number)
      return;

    _grid->addWidget(_number, 0, 1, 1, 3);
    setFocusProxy(pNumberWidget);

    connect(_list,      SIGNAL(clicked()),      this,   SLOT(sEllipses()));
    connect(_info,      SIGNAL(clicked()),      this,   SLOT(sInfo()));
    connect(_number,	SIGNAL(newId(int)),	this,	SIGNAL(newId(int)));
    connect(_number,	SIGNAL(parsed()), 	this, 	SLOT(sRefresh()));
    connect(_number,	SIGNAL(valid(bool)),	this,	SIGNAL(valid(bool)));
}

void ContactCluster::setName(int segment, const QString name)
{
  if ((segment == 0 || segment == 2) &&
      !name.endsWith('.') &&
      !name.isEmpty())
    _fname->replace(segment, name + ".");
  else
    _fname->replace(segment, name);
  _number->setText(_fname->join(" ").replace("  ", " ").trimmed());
}

void ContactCluster::setHonorific(const QString honorific)
{
  setName(0, honorific);
}

void ContactCluster::setFirst(const QString first)
{
  setName(1, first);
}

void ContactCluster::setMiddle(const QString middle)
{
  setName(2, middle);
}

void ContactCluster::setLast(const QString last)
{
  setName(3, last);
}

void ContactCluster::setSuffix(const QString suffix)
{
  setName(4, suffix);
}

void ContactCluster::setPhone(const QString phone)
{
  _phone->setText(phone);
}

void ContactCluster::setTitle(const QString title)
{
  _description->setText(title);
}

void ContactCluster::setFax(const QString fax)
{
  _fax->setText(fax);
}

void ContactCluster::setEmailAddress(const QString email)
{
  _email->setText(email);
}

