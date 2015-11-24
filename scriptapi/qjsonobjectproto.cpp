/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "qjsonobjectproto.h"

#if QT_VERSION < 0x050000
void setupQJsonObjectProto(QScriptEngine *engine)
{
  // do nothing
}

#else

#include <QJsonObject>

QScriptValue QJsonObjecttoScriptValue(QScriptEngine *engine, QJsonObject::iterator const &iterator)
{
  //return engine->newQObject(item);
  return engine->newVariant(QVariant(&iterator));
}

void QJsonObjectfromScriptValue(const QScriptValue &obj, QJsonObject::iterator &iterator)
{
  //item = qobject_cast<QJsonObject*>(obj.toQObject());
  iterator = qscriptvalue_cast<QJsonObject::iterator>(obj);
}

void setupQJsonObjectProto(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, QJsonObjecttoScriptValue, QJsonObjectfromScriptValue);

  QScriptValue proto = engine->newQObject(new QJsonObjectProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QJsonObject*>(), proto);

  QScriptValue constructor = engine->newFunction(constructQJsonObject,
                                                 proto);
  engine->globalObject().setProperty("QJsonObject",  constructor);
}

QScriptValue constructQJsonObject(QScriptContext * context,
                                    QScriptEngine  *engine)
{
  QJsonObject *obj = 0;
  if (context->argumentCount() == 1 && context->argument(0).isVariant())
    obj = new QJsonObject(context->argument(0).toVariant().toJsonObject());
  else
    obj = new QJsonObject();
  return engine->toScriptValue(obj);
}

QJsonObjectProto::QJsonObjectProto(QObject *parent)
    : QObject(parent)
{
}

QJsonObject::iterator QJsonObjectProto::begin()
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->begin();
  return QJsonObject::iterator();
}

QJsonObject::const_iterator QJsonObjectProto::begin() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->begin();
  return QJsonObject::const_iterator();
}

QJsonObject::const_iterator QJsonObjectProto::constBegin() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->constBegin();
  return QJsonObject::const_iterator();
}

QJsonObject::const_iterator QJsonObjectProto::constEnd() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->constEnd();
  return QJsonObject::const_iterator();
}

QJsonObject::const_iterator QJsonObjectProto::constFind(const QString & key) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->constFind(key);
  return QJsonObject::const_iterator();
}

bool QJsonObjectProto::contains(const QString & key) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->contains(key);
  return false;
}

int QJsonObjectProto::count() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->count();
  return 0;
}

bool QJsonObjectProto::empty() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->empty();
  return false;
}

QJsonObject::iterator QJsonObjectProto::end()
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->end();
  return QJsonObject::iterator();
}

QJsonObject::const_iterator QJsonObjectProto::end() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->end();
  return QJsonObject::const_iterator();
}

QJsonObject::iterator QJsonObjectProto::erase(QJsonObject::iterator it)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->erase(it);
  return QJsonObject::iterator();
}

QJsonObject::iterator QJsonObjectProto::find(const QString & key)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->find(key);
  return QJsonObject::iterator();
}

QJsonObject::const_iterator QJsonObjectProto::find(const QString & key) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->find(key);
  return QJsonObject::const_iterator();
}

QJsonObject::iterator QJsonObjectProto::insert(const QString & key, const QJsonValue & value)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->insert(key, value);
  return QJsonObject::iterator();
}

bool QJsonObjectProto::isEmpty() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->isEmpty();
  return false;
}

QStringList QJsonObjectProto::keys() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->keys();
  return QStringList();
}

int QJsonObjectProto::length() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->length();
  return 0;
}

void QJsonObjectProto::remove(const QString & key)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    item->remove(key);
}

int QJsonObjectProto::size() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->size();
  return 0;
}

QJsonValue QJsonObjectProto::take(const QString & key)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->take(key);
  return QJsonValue();
}

QVariantHash QJsonObjectProto::toVariantHash() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->toVariantHash();
  return QVariantHash();
}

QVariantMap QJsonObjectProto::toVariantMap() const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->toVariantMap();
  return QVariantMap();
}

QJsonValue QJsonObjectProto::value(const QString & key) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->value(key);
  return QJsonValue();
}

bool QJsonObjectProto::operator!=(const QJsonObject & other) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->operator!=(other);
  return false;
}

QJsonObject & QJsonObjectProto::operator=(const QJsonObject & other)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->operator=(other);
  return QJsonObject();
}

bool QJsonObjectProto::operator==(const QJsonObject & other) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->operator==(other);
  return false;
}

QJsonValue QJsonObjectProto::operator[](const QString & key) const
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->operator[](key);
  return QJsonValue();
}

QJsonValueRef QJsonObjectProto::operator[](const QString & key)
{
  QJsonObject *item = qscriptvalue_cast<QJsonObject*>(thisObject());
  if (item)
    return item->operator[](key);
  return QJsonValueRef();
}

QJsonObject QJsonObjectProto::fromVariantHash(const QVariantHash & hash)
{
  return QJsonObject::fromVariantHash(hash);
}

QJsonObject QJsonObjectProto::fromVariantMap(const QVariantMap & map)
{
  return QJsonObject::fromVariantMap(map);
}
#endif
