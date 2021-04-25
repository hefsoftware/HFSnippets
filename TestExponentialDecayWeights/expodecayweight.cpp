/* No copyright (2021) Marzocchi Alessandro
The contents of this file is free and unencumbered software released into the
public domain. For more information, please refer to <http://unlicense.org/>
*/

#include "expodecayweight.h"
#include <cmath>
#include <QDebug>
ExpoDecayWeight::ExpoDecayWeight()
{
  setInfiniteDecay(0.5, 20);
}

bool ExpoDecayWeight::setFiniteDecay(qreal weightEnd, int historyLength)
{
  bool ret=false;
  if(weightEnd>0 && historyLength>1)
  {
    ret=true;
    m_accumulator=0.;
    m_historyLen=historyLength;
    m_history.clear();
    m_histEndWeight=weightEnd;
    m_decay=std::pow(m_histEndWeight, 1./(historyLength-1));
    m_accumulatorRunoff=0.;
    m_accumulatorSamplesRunoff=0;
//    m_unitaryAccuSum=0.;
//    qreal temp=1.;
//    for(int i=0;i<m_historyLen;i++)
//    {
//      m_unitaryAccuSum+=temp;
//      temp*=m_decay;
//    }
  }
  return ret;
}

bool ExpoDecayWeight::setInfiniteDecay(qreal decay, int decaySamples)
{
  bool ret=false;
  if(decay>0 && decay<1 && decaySamples>1)
  {
    ret=true;
    m_accumulator=0.;
    m_historyLen=0;
    m_history.clear();
    m_histEndWeight=qQNaN(); //m_unitaryAccuSum=qQNaN();
    m_decay=std::pow(decay, 1./(decaySamples-1));
  }
  return ret;
}

qreal ExpoDecayWeight::push(qreal sample)
{
  if(m_historyLen && m_history.size()>=m_historyLen)
  {
    m_accumulator-=m_history.takeFirst()*m_histEndWeight;
  }
  m_accumulatorSamplesRunoff++;
  m_accumulatorRunoff=m_accumulatorRunoff*m_decay+sample;
  m_accumulator=m_accumulator*m_decay+sample;
  if(m_historyLen && m_accumulatorSamplesRunoff==m_historyLen)
  {
    m_accumulatorSamplesRunoff=0;
    m_accumulator=m_accumulatorRunoff;
    m_accumulatorRunoff=0.;
  }
  if(m_historyLen)
    m_history.append(sample);
  return m_accumulator;
}

qreal ExpoDecayWeight::decay() const
{
  return m_decay;
}

qreal ExpoDecayWeight::accumulatorRunoff() const
{
  return m_accumulatorRunoff;
}

