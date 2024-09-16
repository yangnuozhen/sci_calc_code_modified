#include "Statistics.h"

Statistics::Statistics() {}

Statistics::Statistics(std::vector<long double> *datas)
{
    this->datas = *datas;
}

// 添加数据
void Statistics::addData(long double data)
{
    datas.push_back(data);
}

// 清除所有数据
void Statistics::clearData()
{
    datas.clear();
}

// 移除最后一个数据
void Statistics::popData()
{
    if (!datas.empty())
    {
        datas.pop_back();
    }
}

// 移除指定序号的数据
void Statistics::popData(size_t n)
{
    if (n < datas.size())
    {
        datas.erase(datas.begin() + n);
    }
}

// 获取所有数据
const std::vector<long double> *Statistics::getData() const
{
    return &datas;
}

int Statistics::getLastIndex() const
{
    return datas.size() - 1;
}

// 计算总和
long double Statistics::getSum() const
{
    return std::accumulate(datas.begin(), datas.end(), 0.0L);
}

// 计算平均值
long double Statistics::getMean() const
{
    return datas.empty() ? 0.0L : getSum() / datas.size();
}

// 计算中位数
long double Statistics::getMedian() const
{
    if (datas.empty())
        return 0.0L;
    std::vector<long double> sortedData = datas;
    std::sort(sortedData.begin(), sortedData.end());
    size_t mid = sortedData.size() / 2;
    return sortedData.size() % 2 == 0 ? (sortedData[mid - 1] + sortedData[mid]) / 2.0L : sortedData[mid];
}

// 计算众数
long double Statistics::getMode() const
{
    if (datas.empty())
        return 0.0L;
    std::unordered_map<long double, int> frequency;
    for (long double num : datas)
    {
        frequency[num]++;
    }
    typedef std::unordered_map<long double, int>::iterator iter;
    iter maxElement = std::max_element(frequency.begin(), frequency.end(),
                                       [](const std::pair<const long double, int> &a, const std::pair<const long double, int> &b)
                                       {
                                           return a.second < b.second;
                                       });
    return maxElement->first;
}

// 计算极差
long double Statistics::getRange() const
{
    if (datas.empty())
        return 0.0L;
    auto result = std::minmax_element(datas.begin(), datas.end());
    return *result.second - *result.first;
}

// 计算方差
long double Statistics::getVariance() const
{
    if (datas.size() < 2)
        return 0.0L;
    long double meanVal = getMean();
    long double sumSquaredDiff = std::accumulate(datas.begin(), datas.end(), 0.0L,
                                                 [meanVal](long double acc, long double val)
                                                 { return acc + (val - meanVal) * (val - meanVal); });
    return sumSquaredDiff / datas.size();
}

// 计算标准差
long double Statistics::getStandardDeviation() const
{
    return std::sqrt(getVariance());
}

// 计算第一四分位数
long double Statistics::getQ1() const
{
    if (datas.size() < 4)
        return 0.0L;
    std::vector<long double> sortedData = datas;
    std::sort(sortedData.begin(), sortedData.end());
    size_t n = sortedData.size();
    return sortedData[n / 4];
}

// 计算第二四分位数(即中位数)
long double Statistics::getQ2() const
{
    return getMedian();
}

// 计算第三四分位数
long double Statistics::getQ3() const
{
    if (datas.size() < 4)
        return 0.0L;
    std::vector<long double> sortedData = datas;
    std::sort(sortedData.begin(), sortedData.end());
    size_t n = sortedData.size();
    return sortedData[(3 * n) / 4];
}

/**
 * 计算并返回数据集的偏度
 * 偏度是衡量数据分布偏斜方向和程度的统计量
 * 当数据集小于2个元素时，偏度没有意义，返回0.0
 *
 * @return long double 数据集的偏度
 */
long double Statistics::getSkewness() const
{
    // 数据集大小小于2时，直接返回0.0
    if (datas.size() < 2)
        return 0.0L;

    // 计算数据集的平均值
    long double meanVal = getMean();
    // 计算数据集的标准差
    long double stddev = getStandardDeviation();

    // 使用accumulate计算数据集中各值与均值之差的立方和，差值除以标准差后再求立方
    long double sumCubedDiff = std::accumulate(datas.begin(), datas.end(), 0.0L,
                                               [meanVal, stddev](long double acc, long double val)
                                               { return acc + std::pow((val - meanVal) / stddev, 3); });

    // 返回偏度，即差的立方和除以数据集大小
    return sumCubedDiff / datas.size();
}

/**
 * 计算并返回当前数据集的峰度（kurtosis）
 *
 * 峰度是一种描述数据分布峰态的统计量。对于少量数据（少于2个元素），峰度定义为0
 *
 * @return 过剩峰度（Excess Kurtosis），即峰度-3 以相对于正态分布进行比较。
 */
long double Statistics::getKurtosis() const
{
    // 如果数据集大小小于2，返回0.0L
    if (datas.size() < 2)
        return 0.0L;

    // 获取数据集的均值
    long double meanVal = getMean();

    // 获取数据集的标准差
    long double stddev = getStandardDeviation();

    // 计算数据集中各点与均值的标准差的四次方之和
    long double sumQuartDiff = std::accumulate(datas.begin(), datas.end(), 0.0L,
                                               [meanVal, stddev](long double acc, long double val)
                                               { return acc + std::pow((val - meanVal) / stddev, 4); });

    // 返回修正后的峰度值，通过平均四次方差再减去3得到超额峰度
    // 这里的超额峰度是为了与标准正态分布进行比较，标准正态分布的峰度为3
    return sumQuartDiff / datas.size() - 3.0L;
}
