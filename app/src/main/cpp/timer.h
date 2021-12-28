#ifndef MSP_MOBILE_MODEL_VIEWER_TIMER_H
#define MSP_MOBILE_MODEL_VIEWER_TIMER_H

#include <chrono>
#include <string>
#include <map>

class Timer{
public:
    static Timer* GetInstance();
    static void Clear();

    void StartRecord(const std::string& name);
    void StopRecord(const std::string& name);
    float GetRecord(const std::string& name);

    std::string ToString() const;
    void Print() const;

private:
    Timer() {};
    std::map<std::string, std::chrono::high_resolution_clock::time_point > mRecordings;
    std::map<std::string, std::chrono::duration<double, std::milli> > mResults;
    static std::string ParseDate(const int ms);

    static Timer* mInstance;
};

#endif //MSP_MOBILE_MODEL_VIEWER_TIMER_H
