/* No copyright (2021) Marzocchi Alessandro
The contents of this file is free and unencumbered software released into the
public domain. For more information, please refer to <http://unlicense.org/>
*/

#include "expodecayweight.h"
#include <random>
#include <QList>
#include <QDebug>
void testInfiniteDecayDec(qreal decay, int history);
void testFiniteDecayDec(qreal decay, int history);
void testPositiveInfiniteDecay(QList<qreal> samples, qreal finalDecay, int samplesNum);
void testPositiveFiniteDecay(QList<qreal> samples, qreal decay, int history);
void testPositiveFiniteDecay(int nasamples, qreal decay, int history);
qreal calculateAccumulator(const QList<qreal> &samples, int pos, int history, qreal decay)
{
  auto cutSamples=samples.mid(pos+1-history, history);
  qreal w=1.;
  qreal ret=0;
  for(int i=cutSamples.size()-1;i>=0;i--, w*=decay)
    ret+=cutSamples[i]*w;
  return ret;
}
int main(int argc, char *argv[])
{
  int samplesLen=40;
  int samplesLongLen=1000000;
  QList<qreal> samples, samplesLong;
  std::mt19937 generator;
  std::uniform_real_distribution<qreal> distribution(0, 10.);
  for(int i=0;i<samplesLen;i++)
    samples.append(distribution(generator));
  for(int i=0;i<samplesLongLen;i++)
    samplesLong.append(distribution(generator));
  testInfiniteDecayDec(0.8, 10);
  testFiniteDecayDec(0.8, 10);
  testFiniteDecayDec(2, 10);
  testFiniteDecayDec(2, 500);

  testPositiveInfiniteDecay(samples, 0.75, 10);
  testPositiveInfiniteDecay(samplesLong, 0.75, 10);

  testPositiveFiniteDecay(samples, 0.75, 10);
  testPositiveFiniteDecay(samplesLong, 0.75, 10);
  testPositiveFiniteDecay(samples, 2.03, 10);
  testPositiveFiniteDecay(samplesLong, 2.03, 10);

  //  ExpoDecayWeight decay;
//  decay.s
//  QCoreApplication a(argc, argv);

//  return a.exec();
}
// Tests that a sample after n-samples has underwent the correct decay
void testInfiniteDecayDec(qreal decay, int history)
{
  ExpoDecayWeight expo;
  expo.setInfiniteDecay(decay, history);
  for(int i=0;i<history;i++)
    expo.push(i==0);
  if(qAbs(decay-expo.accumulator())>1e-3)
    qDebug()<<"Test infinite decay ("<<decay<<","<<history<<" expected:"<<decay<<"got"<<expo.accumulator();
}
// Tests that a sample after n-samples has underwfent the correct decay and the next samples it is (almost completely removed from the accumulator)
void testFiniteDecayDec(qreal decay, int history)
{
  ExpoDecayWeight expo;
  expo.setFiniteDecay(decay, history);
  for(int i=0;i<history;i++)
    expo.push(i==0);
  if(qAbs(decay-expo.accumulator())>1e-3)
    qDebug()<<"Test finite decay ("<<decay<<","<<history<<"expected:"<<decay<<"got"<<expo.accumulator();
  expo.push(0);
  if(qAbs(expo.accumulator())>1e-3)
    qDebug()<<"Test finite decay ("<<decay<<","<<history<<"expected: 0 got"<<expo.accumulator();
}

void testPositiveInfiniteDecay(QList<qreal> samples, qreal finalDecay, int samplesNum)
{
  ExpoDecayWeight expo;
  expo.setInfiniteDecay(finalDecay, samplesNum);
  qreal accu=0.;
  Q_FOREACH(qreal sample, samples)
  {
    accu=sample+accu*expo.decay();
    expo.push(sample);
  }
  if(qAbs(accu-expo.accumulator())>1e-3)
    qDebug()<<"Infinite decay ("<<finalDecay<<","<<samplesNum<<","<<samples.size()<<") on class: "<<expo.accumulator()<<"vs manual"<<accu;
}


void testPositiveFiniteDecay(QList<qreal> samples, qreal decay, int history)
{
  ExpoDecayWeight expo;
  expo.setFiniteDecay(decay, history);
  int j=0;
  Q_FOREACH(qreal sample, samples)
  {
    expo.push(sample);
    j++;
  }
  qreal accu=0.;
  Q_FOREACH(qreal sample, samples.mid(samples.size()-history))
    accu=sample+accu*expo.decay();
  if(qAbs(accu-expo.accumulator())>1e-3)
    qDebug()<<"Finite decay ("<<decay<<","<<history<<","<<samples.size()<<") on class: "<<expo.accumulator()<<"vs manual"<<accu;
}
