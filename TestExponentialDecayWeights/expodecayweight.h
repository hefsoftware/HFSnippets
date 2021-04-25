/* No copyright (2021) Marzocchi Alessandro
The contents of this file is free and unencumbered software released into the
public domain. For more information, please refer to <http://unlicense.org/>
*/

#ifndef EXPODECAYWEIGHT_H
#define EXPODECAYWEIGHT_H
#include <QList>
#include <cmath>

class ExpoDecayWeight
{
public:
  ExpoDecayWeight();
  // Sets the parameters to have a sample undergo decay after historyLength samples are pushed in.
  // After that the sample will be removed from the accumulator
  bool setFiniteDecay(qreal weightEnd, int historyLength);
  // Sets the parameters to have a sample undergo decay after decaySamples are pushed in
  bool setInfiniteDecay(qreal decay, int decaySamples);
  // Push a sample and returns the value of the accumulator
  qreal push(qreal sample);
  inline qreal accumulator() { return m_accumulator; }
  qreal decay() const;
  qreal accumulatorRunoff() const;

protected:
  // Length of history. 0 for infinite history
  int m_historyLen;
  // Length of history (number of samples after which the sample is subctracted again from the accumulator
  QList<qreal> m_history;
  // Decay of the accumulator at each step
  qreal m_decay;
  // Decay a sample has underwent when it arrives at the end of history buffer
  qreal m_histEndWeight;
  // Accumulator
  qreal m_accumulator;
  // Resetting accumulator to prevent run-off in cas of negative decay
  qreal m_accumulatorRunoff;
  // Number of samples in run-off accumulator
  int m_accumulatorSamplesRunoff;
};
template <unsigned int N, typename T> class ExpoDecayWeightMulti
{
public:
  ExpoDecayWeightMulti() {  setInfiniteDecay(0.5, 20); }
  // Sets the parameters to have a sample undergo decay after historyLength samples are pushed in.
  // After that the sample will be removed from the accumulator
  bool setFiniteDecay(qreal weightEnd, int historyLength){
    bool ret=false;
    if(weightEnd>0 && historyLength>1)
    {
      ret=true;
      resetAccumulator();
      m_historyLen=historyLength;
      m_history.clear();
      m_histEndWeight=weightEnd;
      m_decay=std::pow(m_histEndWeight, 1./(historyLength-1));
    }
    return ret;
  }
  // Sets the parameters to have a sample undergo decay after decaySamples are pushed in
  bool setInfiniteDecay(qreal decay, int decaySamples)
  {
    bool ret=false;
    if(decay>0 && decay<1 && decaySamples>1)
    {
      ret=true;
      resetAccumulator();
      m_historyLen=0;
      m_history.clear();
      m_histEndWeight=qQNaN();
      m_decay=std::pow(decay, 1./(decaySamples-1));
    }
    return ret;
  }
  inline const QList<qreal> &accumulator() { return m_accumulator; }
protected:
  // Push a sample and returns the value of the accumulator
  inline void push(const T &sample, const std::function<qreal(const T &, int i)> &itemWeight){
    if(m_historyLen && m_history.size()>=m_historyLen)
    {
      auto item=m_history.takeFirst();
      for(int i=0; i<N; i++)
        m_accumulator-=itemWeight(item, i)*m_histEndWeight;
    }
    m_accumulatorSamplesRunoff++;
    for(int i=0; i<N; i++)
    {
      qreal curW=itemWeight(sample, i);
      m_accumulatorRunoff[i]=m_accumulatorRunoff[i]*m_decay+curW;
      m_accumulator[i]=m_accumulator[i]*m_decay+curW;
    }
    if(m_historyLen && m_accumulatorSamplesRunoff==m_historyLen)
    {
      m_accumulatorSamplesRunoff=0;
      m_accumulator=m_accumulatorRunoff;
      for(int i=0; i<N; i++)
        m_accumulatorRunoff[i]=0.;
    }
    if(m_historyLen)
      m_history.append(sample);
  }
  // Length of history. 0 for infinite history
  int m_historyLen;
  // Length of history (number of samples after which the sample is subctracted again from the accumulator
  QList<T> m_history;
  // Decay of the accumulator at each step
  qreal m_decay;
  // Decay a sample has underwent when it arrives at the end of history buffer
  qreal m_histEndWeight;
  // Accumulator
  QList<qreal> m_accumulator;
  // Resetting accumulator to prevent run-off in cas of negative decay
  QList<qreal> m_accumulatorRunoff;
  // Number of samples in run-off accumulator
  int m_accumulatorSamplesRunoff;
private:
  void resetAccumulator()
  {
    m_accumulator.clear();
    for(int i=0;i<N;i++) m_accumulator.append(0.);
    m_accumulatorRunoff=m_accumulator;
    m_accumulatorSamplesRunoff=0;
  }
};

#endif // EXPODECAYWEIGHT_H
