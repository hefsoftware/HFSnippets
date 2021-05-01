/*
 * Copyright 2021 Marzocchi Alessandro
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <QCoreApplication>
#include <hfbimap.h>
#include <QDebug>
void testBiMap();
void testBiMapEx();
int main(int argc, char *argv[])
{
  testBiMapEx();
//  testBiMap();
}
struct TestData
{
  TestData(const QString &first, const QString &second): first(first), second(second) { }
  TestData(const QPair<QString, QString> &data): first(data.first), second(data.second) { }
  QString first;
  QString second;
  bool operator <(const TestData &other) { return first<other.first; }
};
template <class K, class V> void printMap(const HFBiMap<K,V> &map)
{
  qDebug()<<"Iterate keys";
  for(auto it=map.begin();it!=map.end();it++)
    qDebug()<<it.key()<<it.value()<<it.id();
  qDebug()<<"Iterate values";
  for(auto it=map.beginValue();it!=map.end();it++)
    qDebug()<<it.key()<<it.value()<<it.id();
}
void testBiMapEx()
{
//  HFBiMultiMap<int, QString> map;
//  map.insertMulti(4, "Foo");
//  map.insertMulti(6, "Tail");
//  map.insertMulti(15, "Fee");
//  map.insertMulti(4, "Foo2");
//  map.insertMulti(7, "Fee");
//  map.insertMulti(14, "Boo");
//  map.insertMulti(9, "Edge");
//  printMap(map);
  HFBiMap<int, QString> mapInitList({{4,"Boo"}, {5,"Hello"},{4,"Hello"}});
  printMap(mapInitList);
  HFBiMultiMap<int, QString> multiMapInitList({{4,"Boo"}, {5,"Hello"},{4,"Hello"}});
  HFBiMultiMap<int, QString> multiMapInitList2({{5,"Baa"}, {6,"Bee"},{7,"Hello"}});
  printMap(multiMapInitList);
  printMap(multiMapInitList+multiMapInitList2);
//  {
//    qDebug()<<"Insert";
//    auto map2=map;
//    map2.insert(4,"Boo");
//    printMap(map2);
//    qDebug()<<"Insert multikey";
//    map2=map;
//    map2.insertMultiKey(4,"Boo");
//    printMap(map2);
//    qDebug()<<"Insert multivalue";
//    map2=map;
//    map2.insertMultiValue(4,"Boo");
//    printMap(map2);
//  }
//  {
//  qDebug()<<"Comparisation";
//  auto map2=map;
//  qDebug()<<(map==map2)<<"Expected true";
//  qDebug()<<(map!=map2)<<"Expected false";
//  map2.take(4);
//  qDebug()<<(map==map2)<<"Expected false";
//  qDebug()<<(map!=map2)<<"Expected true";
// }

//  qDebug()<<"Lower bound";
//  auto it=map.lowerBound(8);
//  qDebug()<<it.key()<<it.value()<<it.id()<<"Expected 9,Edge";
//  it=map.lowerBound(4);
//  qDebug()<<it.key()<<it.value()<<it.id()<<"Expected 4,Foo2";
//  auto it2=map.lowerBoundValue("Entry");
//  qDebug()<<it2.key()<<it2.value()<<it2.id()<<"Expected 7,Fee";
//  it2=map.lowerBoundValue("Edge");
//  qDebug()<<it2.key()<<it2.value()<<it2.id()<<"Expected 9,Edge";

//  {
//    qDebug()<<"Upper bound";
//    auto it=map.upperBound(6);
//    qDebug()<<it.key()<<it.value()<<it.id()<<"Expected 7,Fee";
//    it=map.upperBound(8);
//    qDebug()<<it.key()<<it.value()<<it.id()<<"Expected 9,Edge";
//    it=map.upperBound(15);
//    qDebug()<<(it==map.end())<<"Expected true";
//    auto it2=map.upperBoundValue("Fee");
//    qDebug()<<it2.key()<<it2.value()<<it2.id()<<"Expected 4,Foo";
//    it2=map.upperBoundValue("Entry");
//    qDebug()<<it2.key()<<it2.value()<<it2.id()<<"Expected 7,Fee";
//    it2=map.upperBoundValue("Tail");
//    qDebug()<<(it==map.end())<<"Expected true";
//  }

//  {
//    qDebug()<<"Find";
//    auto it=map.find(6);
//    qDebug()<<((it==map.endValue())?"EOF":it.value())<<"Expected Tail";
//    it=map.find(8);
//    qDebug()<<((it==map.endValue())?"EOF":it.value())<<"Expected EOF";
//    it=map.find(15);
//    qDebug()<<((it==map.endValue())?"EOF":it.value())<<"Expected Fee";
//    it=map.find(16);
//    qDebug()<<((it==map.endValue())?"EOF":it.value())<<"Expected EOF";
//    it=map.find(4, "Foo");
//    qDebug()<<((it==map.endValue())?"EOF":it.value())<<"Expected Foo";
//    it=map.findValue("Foo2");
//    qDebug()<<((it==map.endValue())?-1:it.key())<<"Expected 4";
//    it=map.findValue("Foo3");
//    qDebug()<<((it==map.endValue())?-1:it.key())<<"Expected -1";
//    it=map.findValue("Tail");
//    qDebug()<<((it==map.endValue())?-1:it.key())<<"Expected 6";
//    it=map.findValue("Tail2");
//    qDebug()<<((it==map.endValue())?-1:it.key())<<"Expected -1";
//    it=map.findValue("Fee", 15);
//    qDebug()<<((it==map.endValue())?-1:it.key())<<"Expected 15";
//  }
//    {
//      qDebug()<<"Find const";
//      auto it=map.findConst(6);
//      qDebug()<<((it==map.cend())?"EOF":it.value())<<"Expected Tail";
//      it=map.findConst(8);
//      qDebug()<<((it==map.cend())?"EOF":it.value())<<"Expected EOF";
//      it=map.findConst(15);
//      qDebug()<<((it==map.cend())?"EOF":it.value())<<"Expected Fee";
//      it=map.findConst(16);
//      qDebug()<<((it==map.cend())?"EOF":it.value())<<"Expected EOF";
//      it=map.findConst(4, "Foo");
//      qDebug()<<((it==map.cend())?"EOF":it.value())<<"Expected Foo";
//      it=map.findValueConst("Foo2");
//      qDebug()<<((it==map.cend())?-1:it.key())<<"Expected 4";
//      it=map.findValueConst("Foo3");
//      qDebug()<<((it==map.cend())?-1:it.key())<<"Expected -1";
//      it=map.findValueConst("Tail");
//      qDebug()<<((it==map.cend())?-1:it.key())<<"Expected 6";
//      it=map.findValueConst("Tail2");
//      qDebug()<<((it==map.cend())?-1:it.key())<<"Expected -1";
//      it=map.findValueConst("Fee", 15);
//      qDebug()<<((it==map.cend())?-1:it.key())<<"Expected 15";
//    }
//  {
//    qDebug()<<"Value";
//    qDebug()<<map.value(6)<<map.value(8)<<map.value(15)<<map.value(16)<<"Expected 'Tail', '', 'Fee', ''";
//    qDebug()<<map.key("Foo2")<<map.key("Foo3")<<map.key("Tail")<<map.key("Tail2")<<"Expected 4, 0, 6, 0";
//    qDebug()<<map.count()<<map.count(4)<<map.count(8)<<map.count(9)<<map.count(16)<<"Expected 7, 2, 0, 1, 0";
//    qDebug()<<map.count()<<map.countValue("Fee")<<map.countValue("Foo3")<<map.countValue("Tail")<<map.countValue("Tail2")<<"Expected 7, 2, 0, 1, 0";
//  }

//  {
//    // Contains:
//    qDebug()<<"Contains";
//    qDebug()<<map.contains(4)<<map.contains(8)<<"Expected true false";
//    qDebug()<<map.containsValue("Foo")<<map.containsValue("Foo3")<<"Expected true false";
//  }

//  {
//    auto map2=map;
//    // Erase
//    qDebug()<<"Erase";
//    auto it=map2.find(4);
//    qDebug()<<it.key()<<it.value()<<"Expected 4,Foo2";
//    it=map2.erase(it);
//    qDebug()<<it.key()<<it.value()<<"Expected 4,Foo";
//    it=map2.find(15);
//    qDebug()<<it.key()<<it.value()<<"Expected 15, fee";
//    it=map2.erase(it);
//    qDebug()<<(it==map2.end())<<"Expected true";
//    map2=map;
//    it=map2.findValue("Fee");
//    qDebug()<<it.key()<<it.value()<<"Expected 7,Fee";
//    it=map2.erase(it);
//    qDebug()<<it.key()<<it.value()<<"Expected 15,Fee";
//    it=map2.findValue("Tail");
//    qDebug()<<it.key()<<it.value()<<"Expected 6, Tail";
//    it=map2.erase(it);
//    qDebug()<<(it==map2.end())<<"Expected true";
//    printMap(map2);
//  }
//    {
//      auto map2=map;
//      // Ta
//      qDebug()<<"Take";
//      auto value=map2.take(4);
//      qDebug()<<value<<"Expected Foo2";
//      value=map2.take(4);
//      qDebug()<<value<<"Expected Foo";
//      value=map2.take(4);
//      qDebug()<<value<<"";
//      map2=map;
//      auto key=map2.takeValue("Fee");
//      qDebug()<<key<<"Expected 7";
//      key=map2.takeValue("Fee");
//      qDebug()<<key<<"Expected 15";
//      key=map2.takeValue("Fee");
//      qDebug()<<key<<"Expected 0";
//    }

//  {
//    qDebug()<<"Clear";
//    int n=map.size();
//    auto map2(map);
//    map2.clear();
//    qDebug()<<map2.size()<<map.size()<<"Expected: 0"<<n;
//    map2=map;
//    qDebug()<<"Remove 4";
//    map2.remove(4);
//    printMap(map2);
//    qDebug()<<"Remove 15";
//    map2=map;
//    map2.remove(15);
//    printMap(map2);
//    map2=map;
//    qDebug()<<"Remove 'Fee'";
//    map2.removeValue("Fee");
//    printMap(map2);
//    map2=map;
//    qDebug()<<"Remove 'Foo'";
//    map2.removeValue("Foo");
//    printMap(map2);
//  }

}
//void testBiMap()
//{
//  HFBiMap<int,QString> map;
//  map.insert(4, "Foo");
//  map.insert(6, "Tail");
//  map.insert(15, "Fee");
//  map.insert(4, "Foo2");
//  map.insert(7, "Fee");
//  map.insert(14, "Boo");
//  map.insert(9, "Edge");
//// // Test basic iterator
////  qDebug()<<"Forward iterator";
////  for(auto it=map.begin();it!=map.end();it++)
////  {
////    qDebug()<<it.key()<<it.value();
////  }
////  qDebug()<<"Reverse iterator";
////  for(auto it=map.beginValue();it!=map.endValue();it++)
////  {
////    qDebug()<<it.key()<<it.value();
////  }

////  // Test iterator erase
////  auto it=map.find(6);
////  qDebug()<<"Before erase key '6'"<<it.key()<<it.value();
////  qDebug()<<map.keys()<<map.values();
////  it=map.erase(it);
////  qDebug()<<"After erase"<<it.key()<<it.value();
////  qDebug()<<map.keys()<<map.values();
////  for(;it!=map.end();)
////  {
////    qDebug()<<"Erasing"<<it.key()<<it.value();
////    it=map.erase(it);
////  }
////  qDebug()<<map.keys()<<map.values();

//    // Test iterator reverse erase
//    QMap<int, int> m;
//    auto it=map.findValue("Edge");
//    qDebug()<<"Before erase key 'Edge'"<<it.key()<<it.value();
//    qDebug()<<map.keys()<<map.values();
//    it=map.erase(it);
//    qDebug()<<"After erase"<<it.key()<<it.value();
//    qDebug()<<map.keys()<<map.values();
//    for(;it!=map.end();)
//    {
//      qDebug()<<"Erasing"<<it.key()<<it.value();
//      it=map.erase(it);
//    }

//  //  qDebug()<<(map.end()!=map.endValue());
////  qDebug()<<(map.endValue()!=map.endValue());
////  //Check comparisation
////  qDebug()<<"Begin==endValue (false)";
////  qDebug()<<(map.begin()==map.endValue());
////  qDebug()<<"endValue==begin (false)";
////  qDebug()<<(map.endValue()==map.begin());
////  qDebug()<<"beginValue==end (false)";
////  qDebug()<<(map.begin()==map.endValue());
////  qDebug()<<"end==beginValue (false)";
////  qDebug()<<(map.endValue()==map.begin());
////  qDebug()<<"end==endValue (true)";
////  qDebug()<<(map.end()==map.endValue());
////  qDebug()<<"endValue==end (true)";
////  qDebug()<<(map.endValue()==map.end());
//}
