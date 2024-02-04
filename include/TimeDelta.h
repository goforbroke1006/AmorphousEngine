//
// Created by goforbroke on 2/4/24.
//

#ifndef AMORPHOUS_ENGINE_TIME_DELTA_H
#define AMORPHOUS_ENGINE_TIME_DELTA_H

#include <list>

class TimeDelta {
public:
    explicit TimeDelta(size_t keepSize)
            : mKeepSize(keepSize),
              mWindowSize(0),
              mWindowSum(0),
              mInsertIdx(0),
              mAvg(0.0) {}

    void insert(const double value) {
        ++mWindowSize;
        mWindowSum += value;
        mWindowValues.push_back(value);

        if (mWindowSize > mKeepSize) {
            --mWindowSize;
            mWindowSum -= mWindowValues.front(), mWindowValues.pop_front();
        }

        mInsertIdx = mInsertIdx % mKeepSize;
        if (mInsertIdx == 0)
            mAvg = mWindowSum / static_cast<double>(mWindowSize);
        ++mInsertIdx;
    }

    [[nodiscard]] double getAvg() const {
        return mAvg;
    }

private:
    size_t mKeepSize;

    size_t mWindowSize;
    std::list<double> mWindowValues;
    double mWindowSum;
    size_t mInsertIdx;
    double mAvg;
};

#endif //AMORPHOUS_ENGINE_TIME_DELTA_H
