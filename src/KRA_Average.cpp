#include "KRA_Average.h"

/* ************* konstruktor ****************** */
KRA_Average::KRA_Average(int averageType, int numberItemAverage):
  averageTypeSet(averageType), numberN(numberItemAverage)
{
  prevValue = 0;
  prevWeight = 0;
}
//float KRA_Average::calcAverage(const float value){calcAverage(value,1);}

float KRA_Average::calcAverage(const float value, const float weight) {
  switch  (averageTypeSet) {
    case WeightedMovingAverageWMA:
      return calcWMA(value, weight);
      break;
    case SimpleMovingAverageSMA:
      return calcSMA(value);
      break;
    case CumulativeMovingAverageCMA:
      return calcCMA(value);
      break;
    case ExponentialMovingAverageEMA:
      return calcEMA(value);
      break;
    default:
      return calcCMA(value);
      break;
  }
  return 0.0;
}


/* ****************  vlastní výpočet průměru  ************** */

float KRA_Average::calcWMA(const float value, const float weight) {
  prevValue=prevValue+(weight*value-prevWeight*prevValue)/numberN;
  prevWeight=prevWeight+(weight-prevWeight)/numberN;
  return prevValue;
}
float KRA_Average::calcSMA(const float value) {
  prevValue=prevValue+(value-prevValue)/numberN;
  return prevValue;
}
float KRA_Average::calcCMA(const float value) {
  prevValue=(value+numberN*prevValue)/(numberN+1);
  return prevValue;
}
float KRA_Average::calcEMA(const float value) {
  float K=2/(numberN+1);
  prevValue=value*K+prevValue*(1-K);
  return prevValue;
}
