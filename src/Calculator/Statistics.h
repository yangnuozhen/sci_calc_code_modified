#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <unordered_map>

class Statistics
{
    public:
        Statistics();
        Statistics(std::vector<long double>* data);
        void addData(long double data);
        void clearData();
        void popData();
        void popData(size_t n);
        const std::vector<long double>* getData() const;
        int getLastIndex() const;
        
        long double getSum() const;
        long double getMean() const;
        long double getMedian() const;
        long double getMode() const;
        long double getRange() const;
        long double getVariance() const;
        long double getStandardDeviation() const;
        long double getQ1() const;
        long double getQ2() const;
        long double getQ3() const;
        long double getSkewness() const;
        long double getKurtosis() const;
    private:
        std::vector<long double> datas;
};
#endif