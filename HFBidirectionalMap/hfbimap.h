/*
 * Copyright 2021 Marzocchi Alessandro
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef HFBiMap_Header
#define HFBiMap_Header

#include <QMap>
#include <QSharedDataPointer>

template <class T> struct HFBiMapFirst {
  constexpr HFBiMapFirst(const QPair<const T *, quint64> &init): d(init.first), id(init.second) { }
  constexpr HFBiMapFirst(const T *data, quint64 id): d(data), id(id) { }
  constexpr operator const T*() {return d;}
  const T *d;
  quint64 id;
};

template <class T> struct HFBiMapSecond {
  constexpr HFBiMapSecond(T *data): d(data) { }
  constexpr operator T *() {return d;}
  T *d;
};

template <class Key, class Value> struct HFBiMapData: public QSharedData
{
  HFBiMapData(): id(0) { }
  HFBiMapData(const HFBiMapData<Key,Value> &base)
  {
    id=base.id;
    for(auto it=base.forward.begin();it!=base.forward.end();++it)
    {
      auto keyCopy=new Key(*it.key().d);
      auto dataCopy=new Value(*it.value());
      forward.insertMulti({keyCopy, it.key().id}, dataCopy);
      reverse.insertMulti({dataCopy, it.key().id}, keyCopy);
    }
  }
  ~HFBiMapData() { clear(); }
  QMap<HFBiMapFirst<Key>, HFBiMapSecond<Value> > forward;
  QMap<HFBiMapFirst<Value>, HFBiMapSecond<Key> > reverse;
  quint64 id;
  void clear(){
    for(auto it=forward.begin(); it!=forward.end(); it++)
    {
      delete (const Key*)it.key();
      delete (Value*)it.value();
    }
    forward.clear();
    reverse.clear();
  }
};
template <class T> inline bool qMapLessThanKey(const HFBiMapFirst<T> &key1, const HFBiMapFirst<T> &key2)
{
  bool less=qMapLessThanKey((*key1), (*key2)), more=qMapLessThanKey((*key2), (*key1));
  return less || (!more && key1.id>key2.id);
}

/** This class provides all the functions of QMap but has simmetrical behaviour regarding Value->Key association.
 *
 */
template <class Key, class Value> class HFBiMap
{
public:
  typedef HFBiMapFirst<Key> ForwardFirst;
  typedef HFBiMapFirst<Value> ReverseFirst;
  typedef HFBiMapSecond<Value> ForwardSecond;
  typedef HFBiMapSecond<Key> ReverseSecond;

  class iterator
  {
    friend class HFBiMap<Key,Value>;
  protected:
    bool m_isForward;
    typename QMap<ForwardFirst, ForwardSecond >::iterator m_forwardIt;
    typename QMap<ReverseFirst, ReverseSecond>::iterator m_reverseIt;
  public:
    inline iterator(bool isForward): m_isForward(isForward) { }

    inline const Key &key() const { return *(m_isForward?m_forwardIt.key().d:m_reverseIt.value().d); }
    inline const Value &value() const { return *(m_isForward?m_forwardIt.value().d:m_reverseIt.key().d); }
    inline qint64 id() const { return m_isForward?m_forwardIt.key().id:m_reverseIt.key().id; }

    inline bool operator!=(const iterator &o) const {
      return !(*this==o);
    }
    inline bool operator==(const iterator &o) const {
      if(m_isForward) {
        return o.m_isForward?m_forwardIt==o.m_forwardIt : (m_forwardIt==o.m_forwardIt) && (m_reverseIt==o.m_reverseIt);
      }
      else {
        return !o.m_isForward?m_reverseIt==o.m_reverseIt : (m_forwardIt==o.m_forwardIt) && (m_reverseIt==o.m_reverseIt);
      }
    }

    inline iterator &operator++() { if(m_isForward)m_forwardIt++; else m_reverseIt++; return *this; }
    inline iterator operator++(int) { iterator r=*this; if(m_isForward)m_forwardIt++; else m_reverseIt++; return r; }
    inline iterator &operator--() { if(m_isForward)m_forwardIt++; else m_reverseIt++; return *this; }
    inline iterator operator--(int) { iterator r=*this; if(m_isForward)m_forwardIt--; else m_reverseIt--; return r; }
    inline iterator operator+(int j) { auto r=*this; if(m_isForward)r->m_forwardIt+=j; else r->m_reverseIt+=j; return r; }
    inline iterator operator-(int j) { auto r=*this; if(m_isForward)r->m_forwardIt-=j; else r->m_reverseIt-=j; return r; }
    inline iterator &operator+=(int j) { if(m_isForward)m_forwardIt+=j; else m_reverseIt+=j; return *this; }
    inline iterator &operator-=(int j) { if(m_isForward)m_forwardIt-=j; else m_reverseIt-=j; return *this; }
  };

  class const_iterator
  {
    friend class HFBiMap<Key,Value>;
  protected:
    bool m_isForward;
    typename QMap<ForwardFirst, ForwardSecond >::const_iterator m_forwardIt;
    typename QMap<ReverseFirst, ReverseSecond>::const_iterator m_reverseIt;
  public:
    inline const_iterator(bool isForward): m_isForward(isForward) { }

    inline const Key &key() const { return *(m_isForward?m_forwardIt.key().d:m_reverseIt.value().d); }
    inline const Value &value() const { return *(m_isForward?m_forwardIt.value().d:m_reverseIt.key().d); }
    inline const qint64 id() const { return m_isForward?m_forwardIt.key().id:m_reverseIt.key().id; }

    inline bool operator!=(const const_iterator &o) const {
      return !(*this==o);
    }
    inline bool operator==(const const_iterator &o) const {
      if(m_isForward) {
        return o.m_isForward?m_forwardIt==o.m_forwardIt : (m_forwardIt==o.m_forwardIt) && (m_reverseIt==o.m_reverseIt);
      }
      else {
        return !o.m_isForward?m_reverseIt==o.m_reverseIt : (m_forwardIt==o.m_forwardIt) && (m_reverseIt==o.m_reverseIt);
      }
    }

    inline const_iterator &operator++() { if(m_isForward)m_forwardIt++; else m_reverseIt++; return *this; }
    inline const_iterator operator++(int) { const_iterator r=*this; if(m_isForward)m_forwardIt++; else m_reverseIt++; return r; }
    inline const_iterator &operator--() { if(m_isForward)m_forwardIt++; else m_reverseIt++; return *this; }
    inline const_iterator operator--(int) { const_iterator r=*this; if(m_isForward)m_forwardIt--; else m_reverseIt--; return r; }
    inline const_iterator operator+(int j) { auto r=*this; if(m_isForward)r->m_forwardIt+=j; else r->m_reverseIt+=j; return r; }
    inline const_iterator operator-(int j) { auto r=*this; if(m_isForward)r->m_forwardIt-=j; else r->m_reverseIt-=j; return r; }
    inline const_iterator &operator+=(int j) { if(m_isForward)m_forwardIt+=j; else m_reverseIt+=j; return *this; }
    inline const_iterator &operator-=(int j) { if(m_isForward)m_forwardIt-=j; else m_reverseIt-=j; return *this; }
  };

  HFBiMap(): m_data(new HFBiMapData<Key,Value>()) { }
  HFBiMap(HFBiMap<Key, Value> &&other) { swap(other); }
  HFBiMap(const HFBiMap<Key, Value> &other) = default;
  inline HFBiMap(std::initializer_list<std::pair<Key,Value> > list): m_data(new HFBiMapData<Key,Value>())
  {
    for (typename std::initializer_list<std::pair<Key,Value> >::const_iterator it = list.begin(); it != list.end(); ++it)
      insert(it->first, it->second);
  }
  HFBiMap<Key, Value> &operator=(HFBiMap<Key, Value> &&other) { m_data.swap(other); }
  HFBiMap<Key, Value> &operator=(const HFBiMap<Key, Value> &other) = default;

  inline iterator begin() { return createForward(m_data->forward.begin()); }
  inline const_iterator begin() const { return constBegin(); }
  inline const_iterator cbegin() const { return constBegin(); }
  inline iterator beginValue() { return createReverse(m_data->reverse.begin()); }
  inline const_iterator beginValue() const { return constBeginValue(); }
  inline const_iterator cbeginValue() const { return constBeginValue(); }
  inline void clear() { m_data->clear(); }
  inline bool contains(const Key &key) const
  {
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    return (it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d));
  }
  inline bool containsValue(const Value &value) const
  {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    return (it!=m_data->reverse.end() && !qMapLessThanKey(value,*it.key().d));
  }
  inline const_iterator constBegin() const { return createForward(m_data->forward.constBegin()); }
  inline const_iterator constBeginValue() const { return createReverse(m_data->reverse.constBegin()); }
  inline const_iterator constEnd() const { return createForward(m_data->forward.constEnd()); }
  inline const_iterator constEndValue() const { return createReverse(m_data->reverse.constEnd()); }
  inline int count() const {return m_data->forward.count();}

  inline bool empty() const { return isEmpty(); }
  inline iterator end() { return createForward(m_data->forward.end()); }
  inline const_iterator end() const { return constEnd(); }
  inline const_iterator cend() const { return constEnd(); }
  inline iterator endValue() { return createReverse(m_data->reverse.end()); }
  inline const_iterator endValue() const { return constEndValue(); }
  inline const_iterator cendValue() const { return constEndValue(); }
  iterator erase(iterator pos)
  {
    if(pos.m_isForward) {
      if(pos.m_forwardIt!=m_data->forward.end())
      {
        const Key *key=pos.m_forwardIt.key();
        const Value *value=pos.m_forwardIt.value();
        m_data->reverse.remove({value, pos.m_forwardIt.key().id});
        pos.m_forwardIt=m_data->forward.erase(pos.m_forwardIt);
        delete key;
        delete value;
      }
    }
    else {
      if(pos.m_reverseIt!=m_data->reverse.end())
      {
        const Key *key=pos.m_reverseIt.value();
        const Value *value=pos.m_reverseIt.key();
        m_data->forward.remove({key, pos.m_reverseIt.key().id});
        pos.m_reverseIt=m_data->reverse.erase(pos.m_reverseIt);
        delete key;
        delete value;
      }
    };
    return pos;
  }
  inline iterator find(const Key &key) {
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    return createForward(it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d)?it:m_data->forward.end());
  }
  inline const_iterator findConst(const Key &key) const {
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    return createForward(it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d)?it:m_data->forward.end());
  }
  inline iterator findValue(const Value &value) {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    return createReverse(it!=m_data->reverse.end() && !qMapLessThanKey(value,*it.key().d)?it:m_data->reverse.end());
  }
  inline const_iterator findValueConst(const Value &value) const {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    return createReverse(it!=m_data->reverse.end() && !qMapLessThanKey(value,*it.key().d)?it:m_data->reverse.end());
  }
  inline const Key &firstKey() const {return *m_data->forward.firstKey();}
  inline const Value &firstValue() const {return *m_data->reverse.firstKey();}
  inline void insert(const Key &key, const Value &value)
  {
    remove(key);
    removeValue(value);
    insertMulti(key, value);
  }
  // Allows multiple entries with same key to exist, but not multiple values
  inline void insertMultiKey(const Key &key, const Value &value)
  {
    removeValue(value);
    insertMulti(key, value);
  }
  // Allows multiple entries with same valuev to exist, but not multiple keys
  inline void insertMultiValue(const Key &key, const Value &value)
  {
    remove(key);
    insertMulti(key, value);
  }
  inline void insertMulti(const Key &key, const Value &value)
  {
    Key *keyCopy=new Key(key);
    Value *valueCopy=new Value(value);
    quint64 id=++m_data->id;
    m_data->forward.insertMulti({keyCopy, id}, valueCopy);
    m_data->reverse.insertMulti({valueCopy, id}, keyCopy);
  }
  inline bool isEmpty() const { return m_data->forward.isEmpty(); }
  inline const Key key(const Value &value, const Key &defaultKey = Key()) const
  {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    return it!=m_data->reverse.end() && !qMapLessThanKey(value,*it.key().d)?*it.value():defaultKey;
  }
  inline QList<Key> keys() const { QList<Key> ret; Q_FOREACH(auto k, m_data->forward.keys()) { ret.append(*k); } return ret; }
  inline const Key &last() const { return *m_data->forward.lastKey(); }
  inline const Value &lastValue() const { return *m_data->reverse.lastKey(); }
  inline iterator lowerBound(const Key &key) { return createForward(m_data->forward.lowerBound({(Key *)&key, std::numeric_limits<quint64>::max()-1})); }
  inline const_iterator lowerBound(const Key &key) const { return createForward(m_data->forward.lowerBound({(Key *)&key, std::numeric_limits<quint64>::max()-1})); }
  inline iterator lowerBoundValue(const Value &value) { return createReverse(m_data->reverse.lowerBound({(Value *)&value, std::numeric_limits<quint64>::max()-1})); }
  inline const_iterator lowerBoundValue(const Value &value) const { return createReverse(m_data->reverse.lowerBound({(Value *)&value, std::numeric_limits<quint64>::max()-1})); }
  inline int remove(const Key &key) {
    int ret=0;
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d);)
    {
      const Key *key=it.key().d;
      const Value *value=it.value();
      m_data->reverse.remove({value, it.key().id});
      it=m_data->forward.erase(it);
      delete key;
      delete value;
      ret++;
    }
    return ret;
  }
  inline int removeValue(const Value &value) {
    int ret=0;
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->reverse.end() && !qMapLessThanKey(value,*it.key().d);)
    {
      const Key *key=it.value();
      const Value *value=it.key().d;
      m_data->forward.remove({key, it.key().id});
      it=m_data->reverse.erase(it);
      delete key;
      delete value;
      ret++;
    }
    return ret;
  }
  inline int size() const {return m_data->forward.size();}
  inline void swap(HFBiMap<Key, Value> &other) { m_data.swap(other.m_data); }
  Value take(const Key &key, const Value &defaultValue=Value())
  {
    auto it=find(key);
    if(it!=end()) { auto ret=it.value(); erase(it); return ret;}
    return defaultValue;
  }
  Key takeValue(const Value &value, const Key &defaultKey=Key())
  {
    auto it=findValue(value);
    if(it!=end()) { auto ret=it.key(); erase(it); return ret;}
    return defaultKey;
  }
  inline iterator upperBound(const Key &key) { return createForward(m_data->forward.upperBound({(Key *)&key, 0})); }
  inline iterator upperBoundValue(const Value &value) { return createReverse(m_data->reverse.upperBound({(Value *)&value, 0})); }
  inline const Value value(const Key &key, const Value &defaultValue = Value()) const
  {
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    return it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d)?*it.value():defaultValue;
  }
  inline QList<Value> values() const { QList<Value> ret; Q_FOREACH(auto v, m_data->reverse.keys()) { ret.append(*v); } return ret; }
  inline bool operator==(const HFBiMap<Key, Value> &other) const {
    if(m_data==other.m_data) // Easy case
      return true;
    auto it=m_data->forward.cbegin(), it2=other.m_data->forward.cbegin();
    for(;it!=m_data->forward.cend() && it2!=other.m_data->forward.cend(); ++it, ++it2)
    {
      if(*it.key().d!=*it2.key().d || *it.value()!=*it2.value())
        return false;
    }
    return (it==m_data->forward.cend() && it==other.m_data->forward.cend());
  }
  inline bool operator!=(const HFBiMap<Key, Value> &other) const { return !(*this==other); }
protected:
  QSharedDataPointer<HFBiMapData<Key, Value> > m_data;

  inline iterator createForward(typename QMap<ForwardFirst,ForwardSecond>::iterator iter) { auto it=iterator(true); it.m_forwardIt=iter; it.m_reverseIt=m_data->reverse.end(); return it; }
  inline iterator createReverse(typename QMap<ReverseFirst,ReverseSecond>::iterator iter) { auto it=iterator(false); it.m_reverseIt=iter; it.m_forwardIt=m_data->forward.end(); return it; }
  inline const_iterator createForward(typename QMap<ForwardFirst,ForwardSecond>::const_iterator iter) const { auto it=const_iterator(true); it.m_forwardIt=iter; it.m_reverseIt=m_data->reverse.end(); return it; }
  inline const_iterator createReverse(typename QMap<ReverseFirst,ReverseSecond>::const_iterator iter) const { auto it=const_iterator(false); it.m_reverseIt=iter; it.m_forwardIt=m_data->forward.end(); return it; }
};

template <class Key, class Value> class HFBiMultiMap: public HFBiMap<Key, Value>
{
protected:
  using HFBiMap<Key, Value>::m_data;
  using HFBiMap<Key, Value>::createForward;
  using HFBiMap<Key, Value>::createReverse;
public:
  inline HFBiMultiMap() {}
  HFBiMultiMap(const HFBiMultiMap<Key, Value> &other) : HFBiMap<Key, Value>(other) {}
  HFBiMultiMap(HFBiMultiMap<Key, Value> &&other): HFBiMap<Key, Value>(std::move(other)) {}
  inline HFBiMultiMap(std::initializer_list<std::pair<Key,Value> > init)
  {
    for (typename std::initializer_list<std::pair<Key,Value> >::const_iterator it = init.begin(); it != init.end(); ++it)
     insert(it->first, it->second);
  }

  void insert(const Key &key, const Value &value)
  {
    HFBiMap<Key, Value>::insertMulti(key, value);
  }
  void replace(const Key &key, const Value &value)
  {
    HFBiMap<Key, Value>::insert(key, value);
  }
  using typename HFBiMap<Key, Value>::iterator;
  using typename HFBiMap<Key, Value>::const_iterator;
  using HFBiMap<Key, Value>::find;
  using HFBiMap<Key, Value>::findConst;
  using HFBiMap<Key, Value>::findValue;
  using HFBiMap<Key, Value>::findValueConst;
  using HFBiMap<Key, Value>::constEnd;
  using HFBiMap<Key, Value>::count;
  using HFBiMap<Key, Value>::lowerBound;
  using HFBiMap<Key, Value>::lowerBoundValue;

  inline bool contains(const Key &key, const Value &value) const
  {
    return (findConst(key, value)!=constEnd());
  }
  inline int count(const Key &key) const {
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    int ret=0;
    for(;it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d);++it, ++ret) { }
    return ret;
  }
  inline int countValue(const Value &value) const {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    int ret=0;
    for(;it!=m_data->reverse.end() && !qMapLessThanKey(value,*it.key().d);++it, ++ret) { }
    return ret;
  }
  inline iterator find(const Key &key, const Value &value) {
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->forward.end() && !qMapLessThanKey(key, *it.key());++it)
    {
      if(!qMapLessThanKey(*it.value(), value) && !qMapLessThanKey(value, *it.value()))
        return createForward(it);
    }
    return createForward(m_data->forward.end());
  }
  inline const_iterator findConst(const Key &key, const Value &value) const{
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->forward.end() && !qMapLessThanKey(key, *it.key());++it)
    {
      if(!qMapLessThanKey(*it.value(), value) && !qMapLessThanKey(value, *it.value()))
        return createForward(it);
    }
    return createForward(m_data->forward.end());
  }
  inline iterator findValue(const Value &value, const Key &key) {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->reverse.end() && !qMapLessThanKey(value, *it.key());++it)
    {
      if(!qMapLessThanKey(*it.value(), key) && !qMapLessThanKey(key, *it.value()))
        return createReverse(it);
    }
    return createForward(m_data->forward.end());
  }
  inline const_iterator findValueConst(const Value &value, const Key &key) const {
    auto it=m_data->reverse.lowerBound({&value, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->reverse.end() && !qMapLessThanKey(value, *it.key());++it)
    {
      if(!qMapLessThanKey(*it.value(), key) && !qMapLessThanKey(key, *it.value()))
        return createReverse(it);
    }
    return createForward(m_data->forward.end());
  }
  inline int remove(const Key &key, const Value &value) {
    int ret=0;
    auto it=m_data->forward.lowerBound({&key, std::numeric_limits<quint64>::max()-1});
    for(;it!=m_data->forward.end() && !qMapLessThanKey(key,*it.key().d);)
    {
      if(!qMapLessThanKey(value, *it.value()) && !qMapLessThanKey(*it.value(),value))
      {
        const Key *key=it.key().d;
        const Value *value=it.value();
        m_data->reverse.remove({value, it.key().id});
        it=m_data->forward.erase(it);
        delete key;
        delete value;
        ret++;
      }
    }
    return ret;
  }
  void swap(HFBiMultiMap<Key, Value> &other) { HFBiMap<Key, Value>::swap(other); }
  HFBiMultiMap<Key,Value> &operator +=(const HFBiMultiMap<Key,Value> &other)
  {
    for(auto it=other.m_data->forward.begin();it!=other.m_data->forward.end();it++)
    {
      insert(*it.key(), *it.value());
    }
    return *this;
  }
  HFBiMultiMap<Key,Value> operator +(const HFBiMultiMap<Key,Value> &other) const
  {
    HFBiMultiMap<Key,Value> ret(*this);
    ret+=other;
    return ret;
  }
};

#endif // HFBiMapEx_H
