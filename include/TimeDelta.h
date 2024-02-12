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
              mWindowSum(0),
              mInsertIdx(0),
              mAvg(0.0) {}

    void insert(const double value) {
        mWindowSum += value;
        mWindowValues.push_back(value);

        if (mWindowValues.size() > mKeepSize) {
            mWindowSum -= mWindowValues.front(), mWindowValues.pop_front();
        }

        if (mInsertIdx % mKeepSize == 0)
            mAvg = mWindowSum / static_cast<double>(mWindowValues.size());

        if (mWindowValues.size() <= mKeepSize) {
            mAvg = mWindowValues.back();
        }

        ++mInsertIdx;
    }

    [[nodiscard]] double getAvg() const {
        return mAvg;
    }

private:
    size_t mKeepSize;

    std::list<double> mWindowValues;
    double mWindowSum;
    size_t mInsertIdx;
    double mAvg;
};

#endif //AMORPHOUS_ENGINE_TIME_DELTA_H
