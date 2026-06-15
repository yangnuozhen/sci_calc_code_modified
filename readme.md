# Fuyu-Calc

基于 [shaoxiongduan/sci-calc @ 684103f](https://github.com/shaoxiongduan/sci-calc/tree/684103f791988aa53a99f67ef427c77e82818fa6) 的 Sci-Calc 固件魔改版本。

## 添加功能

- [x] 图形计算器
- [x] 统计计算器
- [x] 美化的 Stopwatch 界面
- [x] 允许无 SD 卡启动
- [x] 以百分比显示电量 (实验性，需要校准数据)
- [x] 修补了一些复杂的 Bug...

## 校准电池放电数据

电压-电量查表，是由我手上设备测量得出的，仅供参考。

你可以在 [src/Utils/Util.cpp](https://github.com/yangnuozhen/fuyu-calc/commit/c8e2581b2e320ec02ed3860fe11ae9700cf049e2#diff-c5b7c56a795d2c037601f0f72e87f0b844fed1cca5d9d13e416373f39643b187R604-R616) 中修改 `battery_table[]` 来校准你的设备的电压-电量数据。

这里提供了一个原位自动放电试验项目，将该固件刷入 Sci-calc 后，其可以自动采集不同负载情况下的时间-电压数据: [fuyu-calc-battery-discharge](https://github.com/yangnuozhen/fuyu-calc-battery-discharge)

另外亦有一个生成电压-电量查表的 C# 脚本:

```csharp
#r "nuget: ScottPlot, 5.1.58"

using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using ScottPlot;
using System.Diagnostics;

// 设置放电数据的 CSV 路径
string csvPath = "curve.csv";
if (!File.Exists(csvPath))
{
    Console.WriteLine($"[错误] 找不到文件: {csvPath}");
    return;
}

var times = new List<double>();
var voltages = new List<double>();

// 逐行解析数据
foreach (var line in File.ReadLines(csvPath))
{
    // 跳过空行、以 # 开头的注释行、以及表头行
    if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#") || line.StartsWith("sample")) 
        continue;

    var parts = line.Split(',');
    
    // 确保这行至少有 4 列数据
    if (parts.Length >= 4)
    {
        // 解析第 2 列为时间 (毫秒)，并除以 1000 转为秒
        if (double.TryParse(parts[1], out double t_ms) && 
            double.TryParse(parts[3], out double v_filtered)) // 解析第 4 列为滤波电压
        {
            times.Add(t_ms / 1000.0); 
            voltages.Add(v_filtered); 
        }
    }
}

if (times.Count == 0)
{
    Console.WriteLine("[错误] CSV 中没有解析到有效数据！请检查格式。");
    return;
}

// ==========================================
// 1. 绘制并保存放电曲线图
// ==========================================
var plot = new ScottPlot.Plot();
plot.Add.ScatterLine(times.ToArray(), voltages.ToArray());
plot.Title("ESP32 Battery Discharge Curve (Filtered)");
plot.XLabel("Time (Seconds)");
plot.YLabel("Voltage (V)");

// 根据你的真实数据，限制 Y 轴范围在 2.5V 到 4.2V 之间，让曲线更清晰
plot.Axes.SetLimitsY(2.5, 4.2); 

plot.SavePng("DischargeCurve.png", 800, 500);
Console.WriteLine($"✅ 放电曲线已成功绘制并保存为图片: {Directory.GetCurrentDirectory()}\\DischargeCurve.png\n");

// ==========================================
// 2. 根据时间轴推断剩余百分比，生成 C/C++ 代码
// ==========================================
double maxTime = times.Last();

Console.WriteLine("/* ========================================================= ");
Console.WriteLine(" * 自动生成的电池电压-电量映射表 (已适配当前硬件与放电曲线)");
Console.WriteLine(" * 测试总耗时: " + (maxTime / 3600.0).ToString("F2") + " 小时");
Console.WriteLine(" * ========================================================= */");
Console.WriteLine("const BatteryLUT battery_table[] = {");

int[] targetPercentages = { 100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 5, 0 };

for (int i = 0; i < targetPercentages.Length; i++)
{
    int p = targetPercentages[i];
    
    // 目标时间点
    double targetTime = maxTime * (100.0 - p) / 100.0;

    // 寻找最接近的时间点
    int closestIndex = 0;
    double minDiff = double.MaxValue;
    for (int j = 0; j < times.Count; j++)
    {
        double diff = Math.Abs(times[j] - targetTime);
        if (diff < minDiff)
        {
            minDiff = diff;
            closestIndex = j;
        }
    }

    double matchedVoltage = voltages[closestIndex];
    string comma = (i == targetPercentages.Length - 1) ? "" : ",";
    
    Console.WriteLine($"    {{{matchedVoltage:F3}, {p,3}}}{comma}");
}

Console.WriteLine("};");
Console.WriteLine("\n🎉 处理完成！");
```

输出可能如下：

```plain
✅ 放电曲线已成功绘制并保存为图片: <path_to_your_directory>\DischargeCurve.png

/* ========================================================= 
 * 自动生成的电池电压-电量映射表 (已适配当前硬件与放电曲线)
 * 测试总耗时: 2.88 小时
 * ========================================================= */
const BatteryLUT battery_table[] = {
    {3.972, 100},
    {3.857,  90},
    {3.817,  80},
    {3.760,  70},
    {3.709,  60},
    {3.671,  50},
    {3.653,  40},
    {3.630,  30},
    {3.588,  20},
    {3.527,  10},
    {3.496,   5},
    {3.014,   0}
};

🎉 处理完成！
```

将 battery_table[] 替换即可。
