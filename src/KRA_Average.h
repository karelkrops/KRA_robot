#ifndef KRA_AVERAGE_H
#define KRA_AVERAGE_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 0.0.1
 * datum verze: 15.2.2020
 * 
 * Třída je určena k počítání průběžných průměrů
 */

/**
 * 
 * zdroj vzorců pro výpočet: https://en.wikipedia.org/wiki/Moving_average#Relationship_between_SMA_and_EMA
 */

class KRA_Average {
  public:
    KRA_Average(int averageType,int numberItemAverage);
    /*
    bool setN(const int N) {
      if (N > 0) {
        numberN = N;
        return true;
      } else return false;
    }
    */
    enum averageType{WeightedMovingAverageWMA, // věžený průběžný průměr.
    SimpleMovingAverageSMA, // Nejjednoduší průběžně počítaný průměr
    CumulativeMovingAverageCMA,
    ExponentialMovingAverageEMA // Tento průměr výrazně rychleji reaguje na poslední hodnoty
    };
    float calcAverage(const float value,const float weight);
    float calcAverage(const float value){return calcAverage(value,1.0);}
  protected:
    float calcWMA(const float value,const float weight);
    float calcSMA(const float value);
    float calcCMA(const float value);
    float calcEMA(const float value);
    const int numberN;
    float prevValue;
    float prevWeight;
    const int averageTypeSet;
};
#endif // KRA_AVERAGE_H
