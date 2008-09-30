/*
 * Common Public Attribution License Version 1.0. 
 * 
 * The contents of this file are subject to the Common Public Attribution 
 * License Version 1.0 (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License 
 * at http://www.xTuple.com/CPAL.  The License is based on the Mozilla 
 * Public License Version 1.1 but Sections 14 and 15 have been added to 
 * cover use of software over a computer network and provide for limited 
 * attribution for the Original Developer. In addition, Exhibit A has 
 * been modified to be consistent with Exhibit B.
 * 
 * Software distributed under the License is distributed on an "AS IS" 
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See 
 * the License for the specific language governing rights and limitations 
 * under the License. 
 * 
 * The Original Code is xTuple ERP: PostBooks Edition 
 * 
 * The Original Developer is not the Initial Developer and is __________. 
 * If left blank, the Original Developer is the Initial Developer. 
 * The Initial Developer of the Original Code is OpenMFG, LLC, 
 * d/b/a xTuple. All portions of the code written by xTuple are Copyright 
 * (c) 1999-2008 OpenMFG, LLC, d/b/a xTuple. All Rights Reserved. 
 * 
 * Contributor(s): ______________________.
 * 
 * Alternatively, the contents of this file may be used under the terms 
 * of the xTuple End-User License Agreeement (the xTuple License), in which 
 * case the provisions of the xTuple License are applicable instead of 
 * those above.  If you wish to allow use of your version of this file only 
 * under the terms of the xTuple License and not to allow others to use 
 * your version of this file under the CPAL, indicate your decision by 
 * deleting the provisions above and replace them with the notice and other 
 * provisions required by the xTuple License. If you do not delete the 
 * provisions above, a recipient may use your version of this file under 
 * either the CPAL or the xTuple License.
 * 
 * EXHIBIT B.  Attribution Information
 * 
 * Attribution Copyright Notice: 
 * Copyright (c) 1999-2008 by OpenMFG, LLC, d/b/a xTuple
 * 
 * Attribution Phrase: 
 * Powered by xTuple ERP: PostBooks Edition
 * 
 * Attribution URL: www.xtuple.org 
 * (to be included in the "Community" menu of the application if possible)
 * 
 * Graphic Image as provided in the Covered Code, if any. 
 * (online at www.xtuple.com/poweredby)
 * 
 * Display of Attribution Information is required in Larger Works which 
 * are defined in the CPAL as a work which combines Covered Code or 
 * portions thereof with code not governed by the terms of the CPAL.
 */

#include "uninvoicedShipments.h"

#include <QVariant>
#include <QMessageBox>
//#include <QStatusBar>
#include <QMenu>
#include <parameter.h>
#include <openreports.h>
#include "selectOrderForBilling.h"

/*
 *  Constructs a uninvoicedShipments as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
uninvoicedShipments::uninvoicedShipments(QWidget* parent, const char* name, Qt::WFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

  // signals and slots connections
  connect(_print, SIGNAL(clicked()), this, SLOT(sPrint()));
  connect(_close, SIGNAL(clicked()), this, SLOT(close()));
  connect(_showUnselected, SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_coship, SIGNAL(populateMenu(QMenu*,QTreeWidgetItem*,int)), this, SLOT(sPopulateMenu(QMenu*)));
  connect(_warehouse, SIGNAL(updated()), this, SLOT(sFillList()));

  _coship->setRootIsDecorated(TRUE);
  _coship->addColumn(tr("Order/Line #"),           _itemColumn, Qt::AlignRight,  true,  "orderline" );
  _coship->addColumn(tr("Cust./Item Number"),      _itemColumn, Qt::AlignLeft,   true,  "custitem"  );
  _coship->addColumn(tr("Cust. Name/Description"), -1,          Qt::AlignLeft,   true,  "custdescrip"  );
  _coship->addColumn(tr("UOM"),                    _uomColumn,  Qt::AlignLeft,   true,  "uom_name"  );
  _coship->addColumn(tr("Shipped"),                _qtyColumn,  Qt::AlignRight,  true,  "shipped" );
  _coship->addColumn(tr("Selected"),               _qtyColumn,  Qt::AlignRight,  true,  "selected" );
  
  connect(omfgThis, SIGNAL(billingSelectionUpdated(int, int)), this, SLOT(sFillList()));

  sFillList();
}

/*
 *  Destroys the object and frees any allocated resources
 */
uninvoicedShipments::~uninvoicedShipments()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void uninvoicedShipments::languageChange()
{
  retranslateUi(this);
}

void uninvoicedShipments::sPrint()
{
  ParameterList params;
  _warehouse->appendValue(params);

  orReport report("UninvoicedShipments", params);
  if (report.isValid())
    report.print();
  else
    report.reportError(this);
}

void uninvoicedShipments::sPopulateMenu(QMenu *menu)
{
  int menuItem;

  menuItem = menu->insertItem(tr("Select This Order for Billing..."), this, SLOT(sSelectForBilling()), 0);
  if (!_privileges->check("SelectBilling"))
    menu->setItemEnabled(menuItem, FALSE);
}

void uninvoicedShipments::sSelectForBilling()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("sohead_id", _coship->id());

  selectOrderForBilling *newdlg = new selectOrderForBilling();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void uninvoicedShipments::sFillList()
{
  _coship->clear();

  QString sql( "SELECT cohead_id, coship_id, cohead_number, coitem_linenumber,"
               "       cust_number, cust_name,"
               "       item_number, description,"
               "       uom_name, shipped, selected,"
               "       CASE WHEN (level=0) THEN cohead_number"
               "            ELSE CAST(coitem_linenumber AS TEXT)"
               "       END AS orderline,"
               "       CASE WHEN (level=0) THEN cust_number"
               "            ELSE item_number"
               "       END AS custitem,"
               "       CASE WHEN (level=0) THEN cust_name"
               "            ELSE description"
               "       END AS custdescrip,"
               "       'qty' AS shipped_xtnumericrole,"
               "       'qty' AS selected_xtnumericrole,"
               "       level AS xtindentrole "
               "FROM ( " );
  sql +=       "SELECT cohead_id, -1 AS coship_id, cohead_number, -1 AS coitem_linenumber, 0 AS level,"
               "       cust_number, cust_name,"
               "       '' AS item_number, '' AS description,"
               "       '' AS uom_name,"
               "       COALESCE(SUM(coship_qty), 0) AS shipped,"
               "       COALESCE(SUM(cobill_qty), 0) AS selected "
               "FROM cohead, cust, itemsite, item, warehous, coship, cosmisc, uom,"
               "     coitem LEFT OUTER JOIN"
               "      ( cobill JOIN cobmisc"
               "        ON ( (cobill_cobmisc_id=cobmisc_id) AND (NOT cobmisc_posted) ) )"
               "     ON (cobill_coitem_id=coitem_id) "
               "WHERE ( (cohead_cust_id=cust_id)"
               " AND (coship_coitem_id=coitem_id)"
               " AND (coship_cosmisc_id=cosmisc_id)"
               " AND (coitem_cohead_id=cohead_id)"
               " AND (coitem_itemsite_id=itemsite_id)"
               " AND (coitem_qty_uom_id=uom_id)"
               " AND (itemsite_item_id=item_id)"
               " AND (itemsite_warehous_id=warehous_id)"
               " AND (cosmisc_shipped)"
               " AND (NOT coship_invoiced)";

  if (_warehouse->isSelected())
    sql += " AND (itemsite_warehous_id=:warehous_id)";

  sql += ") "
         "GROUP BY cohead_id, cohead_number, cust_number, cust_name ";

  sql +=       "UNION "
               "SELECT cohead_id, coship_id, cohead_number, coitem_linenumber, 1 AS level,"
               "       cust_number, cust_name,"
               "       item_number, (item_descrip1 || ' ' || item_descrip2) AS description,"
               "       uom_name,"
               "       COALESCE(SUM(coship_qty), 0) AS shipped,"
               "       COALESCE(SUM(cobill_qty), 0) AS selected "
               "FROM cohead, cust, itemsite, item, warehous, coship, cosmisc, uom,"
               "     coitem LEFT OUTER JOIN"
               "      ( cobill JOIN cobmisc"
               "        ON ( (cobill_cobmisc_id=cobmisc_id) AND (NOT cobmisc_posted) ) )"
               "     ON (cobill_coitem_id=coitem_id) "
               "WHERE ( (cohead_cust_id=cust_id)"
               " AND (coship_coitem_id=coitem_id)"
               " AND (coship_cosmisc_id=cosmisc_id)"
               " AND (coitem_cohead_id=cohead_id)"
               " AND (coitem_itemsite_id=itemsite_id)"
               " AND (coitem_qty_uom_id=uom_id)"
               " AND (itemsite_item_id=item_id)"
               " AND (itemsite_warehous_id=warehous_id)"
               " AND (cosmisc_shipped)"
               " AND (NOT coship_invoiced)";

  if (_warehouse->isSelected())
    sql += " AND (itemsite_warehous_id=:warehous_id)";

  sql += ") "
         "GROUP BY cohead_number, cust_number, cust_name,"
         "         coitem_id, coitem_linenumber, item_number,"
         "         item_descrip1, item_descrip2, cohead_id, coship_id, uom_name "
         
         "   ) AS data ";

  if (_showUnselected->isChecked())
    sql += " WHERE (selected = 0) ";

  sql += "ORDER BY cohead_number DESC, level, coitem_linenumber DESC;";

  q.prepare(sql);
  _warehouse->bindValue(q);
  q.exec();
  _coship->populate(q, true);
}
