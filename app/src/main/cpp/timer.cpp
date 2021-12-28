#include <iostream>
#include <sstream>
#include "timer.h"

Timer* Timer::mInstance = nullptr;

Timer* Timer::GetInstance() {
    if (mInstance == nullptr) {
        mInstance = new Timer();
    }

    return mInstance;
}

void Timer::Clear()
{
    delete mInstance;
    mInstance = nullptr;
}

void Timer::StartRecord(const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();

    auto it = mRecordings.find(name);
    if (it != mRecordings.end()) {
        it->second = start;
    } else {
        mRecordings.insert(std::pair<std::string, std::chrono::high_resolution_clock::time_point>(name, start));
    }
}

void Timer::StopRecord(const std::string& name) {
    auto end = std::chrono::high_resolution_clock::now();

    auto it = mRecordings.find(name);
    if (it != mRecordings.end()) {
        auto start = it->second;
        auto result = end - start;

        mResults.insert(std::pair<std::string, std::chrono::duration<double, std::milli> >(name, result));
    }
}

float Timer::GetRecord(const std::string& name)
{
    auto it = mResults.find(name);
    if (it != mResults.end())
    {
        return it->second.count();
    }
}

std::string Timer::ToString() const
{
    std::stringstream stream;
    auto it = mResults.begin();
    while(it != mResults.end())
    {
        stream << it->first << ": " << it->second.count() << "ms;";
        it++;
    }
    return stream.str();
}

void Timer::Print() const {
    std::cout << ToString() << std::endl;
}

std::string Timer::ParseDate(const int ms) {
    int minutes = (int) (ms / 1000 / 60);
    int seconds = (int) ((ms % (1000 * 60)) / 1000);
    int milliseconds = (int) (ms % 1000);

    std::ostringstream stringStream;
    if (seconds == 60) {
        stringStream << minutes + 1 << ":00" << seconds << ".";
    } else {
        stringStream << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << ".";
    }

    if (milliseconds < 100) {
        if (milliseconds < 10) {
            stringStream << "00" << milliseconds;
        } else {
            stringStream << "0" << milliseconds;
        }
    } else {
        stringStream << milliseconds;
    }

    return stringStream.str();
}