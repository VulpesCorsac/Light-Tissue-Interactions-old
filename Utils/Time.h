#pragma once

#include <chrono>
#include <string>

namespace Utils_NS {
    /// Utils class to log duration
    class Time {
    public:
        /// Constructor
        explicit Time() noexcept = default;

        /// Destructor
        ~Time() noexcept = default;

        /// Reset time accumulator
        void reset() noexcept;

        /// Start new time range
        void start() noexcept;

        /// Finish time time range
        void finish() noexcept;

        /// Overall duration in microseconds
        /// \return Overall duration in microseconds
        unsigned long long usecOverallDuration() const noexcept;

        /// Overall duration in milliseconds
        /// \return Overall duration in milliseconds
        unsigned long long msecOverallDuration() const noexcept;

        /// Overall duration in seconds
        /// \return Overall duration in seconds
        unsigned long long secOverallDuration() const noexcept;

        /// Set overall duration
        /// \param[in] new_duration new overall duration in microseconds
        void setOverallDuration(const unsigned long long& new_duration) noexcept;

        /// Add overall duration
        /// \param[in] new_duration overall duration increase in microseconds
        void addOverallDuration(const unsigned long long& new_duration) noexcept;

        /// Nanoseconds from start timer to now
        /// \return Nanoseconds from start timer to now
        double nsecElapsedNow() const noexcept;

        /// Nanoseconds from start timer to finish timer
        /// \return Nanoseconds from start timer to finish timer
        double nsecElapsedDuration() const noexcept;

        /// Microseconds from start timer to now
        /// \return Microseconds from start timer to now
        double usecElapsedNow() const noexcept;

        /// Microseconds from start timer to finish timer
        /// \return Microseconds from start timer to finish timer
        double usecElapsedDuration() const noexcept;

        /// Milliseconds from start timer to now
        /// \return Milliseconds from start timer to now
        double msecElapsedNow() const noexcept;

        /// Milliseconds from start timer to finish timer
        /// \return Milliseconds from start timer to finish timer
        double msecElapsedDuration() const noexcept;

        /// Seconds from start timer to now
        /// \return Seconds from start timer to now
        double  secElapsedNow() const noexcept;

        /// Seconds from start timer to finish timer
        /// \return Seconds from start timer to finish timer
        double  secElapsedDuration() const noexcept;

        /// Milliseconds from start of epoch to now
        /// \return Milliseconds from start of epoch to now
        unsigned long long msecSinceEpochNow() const noexcept;

        /// Milliseconds from start of epoch to start timer
        /// \return Milliseconds from start of epoch to start timer
        unsigned long long msecSinceEpochStart() const noexcept;

        /// Milliseconds from start of epoch to finish timer
        /// \return Milliseconds from start of epoch to finish timer
        unsigned long long msecSinceEpochFinish() const noexcept;

        /// Seconds from start of epoch to now
        /// \return Seconds from start of epoch to now
        unsigned long long  secSinceEpochNow() const noexcept;

        /// Seconds from start of epoch to start timer
        /// \return Seconds from start of epoch to start timer
        unsigned long long  secSinceEpochStart() const noexcept;

        /// Seconds from start of epoch to finish timer
        /// \return Seconds from start of epoch to finish timer
        unsigned long long  secSinceEpochFinish() const noexcept;

        /// Get current time in string format
        /// \return Get current time in string format
        std::string getCurrentTime() noexcept;

        /// Get current date in string format
        /// \return Get current date in string format
        std::string getCurrentDate() noexcept;

        /// Get current date and time in string format
        /// \return Get current date and time in string format
        std::string getCurrentDateTime() noexcept;

    protected:
        std::string current_date; ///< Current date in string format
        std::string current_time; ///< Current time in string format

        std::chrono::high_resolution_clock::time_point time_start  = std::chrono::high_resolution_clock::now(); ///< Time start point
        std::chrono::high_resolution_clock::time_point time_finish = std::chrono::high_resolution_clock::now(); ///< Time finish point

        unsigned long long overall_time = 0; ///< Accumulated time in ms

    protected:
        /// Nanoseconds elapsed from timer start point to time point
        /// \param[in] time time point
        /// \return Nanoseconds elapsed from timer start point to time point
        double nsecElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept;

        /// Microseconds elapsed from timer start point to time point
        /// \param[in] time time point
        /// \return Microseconds elapsed from timer start point to time point
        double usecElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept;

        /// Milliseconds elapsed from timer start point to time point
        /// \param[in] time time point
        /// \return Milliseconds elapsed from timer start point to time point
        double msecElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept;

        /// Seconds elapsed from timer start point to time point
        /// \param[in] time time point
        /// \return Seconds elapsed from timer start point to time point
        double  secElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept;

        /// Milliseconds elapsed from epoch point to time point
        /// \param[in] time time point
        /// \return Milliseconds elapsed from epoch point to time point
        unsigned long long msecSinceEpoch(const std::chrono::high_resolution_clock::time_point& time) const noexcept;

        /// Seconds elapsed from epoch point to time point
        /// \param[in] time time point
        /// \return Seconds elapsed from epoch point to time point
        unsigned long long  secSinceEpoch(const std::chrono::high_resolution_clock::time_point& time) const noexcept;

        /// Convert value to string and add leading zeros if necessary
        /// \param[in] value value to convert to string
        /// \param[in] min_length minimal string length
        /// \return Convert value to string and add leading zeros if necessary
        std::string to_string_and_appbegin(const int& value, const size_t& min_length = 0) const noexcept;
    };
}


/******************
 * IMPLEMENTATION *
 ******************/
void Utils_NS::Time::reset() noexcept {
    overall_time = 0;
}

void Utils_NS::Time::start() noexcept {
    time_start = std::chrono::high_resolution_clock::now();
}

void Utils_NS::Time::finish() noexcept {
    time_finish = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast < std::chrono::microseconds > (time_finish - time_start);
    overall_time += elapsed.count();
}

unsigned long long Utils_NS::Time::usecOverallDuration() const noexcept {
    return overall_time;
}

unsigned long long Utils_NS::Time::msecOverallDuration() const noexcept {
    return overall_time / 1000;
}

unsigned long long Utils_NS::Time::secOverallDuration() const noexcept {
    return overall_time / 1000 / 1000;
}

void Utils_NS::Time::setOverallDuration(const unsigned long long& new_duration) noexcept {
    overall_time = new_duration;
}

void Utils_NS::Time::addOverallDuration(const unsigned long long& new_duration) noexcept {
    overall_time += new_duration;
}

double Utils_NS::Time::nsecElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept {
    auto elapsed = std::chrono::duration_cast < std::chrono::nanoseconds > (time - time_start);
    return elapsed.count();
}

double Utils_NS::Time::usecElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept {
    auto elapsed = std::chrono::duration_cast < std::chrono::microseconds > (time - time_start);
    return elapsed.count();
}

double Utils_NS::Time::msecElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept {
    auto elapsed = std::chrono::duration_cast < std::chrono::milliseconds > (time - time_start);
    return elapsed.count();
}

double Utils_NS::Time::secElapsed(const std::chrono::high_resolution_clock::time_point& time) const noexcept {
    auto elapsed = std::chrono::duration_cast < std::chrono::seconds > (time - time_start);
    return elapsed.count();
}

double Utils_NS::Time::nsecElapsedNow() const noexcept {
    return nsecElapsed(std::chrono::high_resolution_clock::now());
}

double Utils_NS::Time::nsecElapsedDuration() const noexcept {
    return nsecElapsed(time_finish);
}

double Utils_NS::Time::usecElapsedNow() const noexcept {
    return usecElapsed(std::chrono::high_resolution_clock::now());
}

double Utils_NS::Time::usecElapsedDuration() const noexcept {
    return usecElapsed(time_finish);
}

double Utils_NS::Time::msecElapsedNow() const noexcept {
    return msecElapsed(std::chrono::high_resolution_clock::now());
}

double Utils_NS::Time::msecElapsedDuration() const noexcept {
    return msecElapsed(time_finish);
}

double Utils_NS::Time::secElapsedNow() const noexcept {
    return secElapsed(std::chrono::high_resolution_clock::now());
}

double Utils_NS::Time::secElapsedDuration() const noexcept {
    return secElapsed(time_finish);
}

unsigned long long Utils_NS::Time::msecSinceEpoch(const std::chrono::high_resolution_clock::time_point& time) const noexcept {
    return 1000 * secSinceEpoch(time);
}

unsigned long long Utils_NS::Time::secSinceEpoch(const std::chrono::high_resolution_clock::time_point& time) const noexcept {
    return time.time_since_epoch() / std::chrono::seconds(1);
}

unsigned long long Utils_NS::Time::msecSinceEpochNow() const noexcept {
    return msecSinceEpoch(std::chrono::high_resolution_clock::now());
}

unsigned long long Utils_NS::Time::msecSinceEpochStart() const noexcept {
    return msecSinceEpoch(time_start);
}

unsigned long long Utils_NS::Time::msecSinceEpochFinish() const noexcept {
    return msecSinceEpoch(time_finish);
}

unsigned long long Utils_NS::Time::secSinceEpochNow() const noexcept {
    return secSinceEpoch(std::chrono::high_resolution_clock::now());
}

unsigned long long Utils_NS::Time::secSinceEpochStart() const noexcept {
    return secSinceEpoch(time_start);
}

unsigned long long Utils_NS::Time::secSinceEpochFinish() const noexcept {
    return secSinceEpoch(time_finish);
}

std::string Utils_NS::Time::to_string_and_appbegin(const int& value, const size_t& min_length) const noexcept {
    std::string result = std::to_string(value);
    while (result.length() < min_length)
        result = "0" + result;
    return result;
}

std::string Utils_NS::Time::getCurrentTime() noexcept {
    getCurrentDateTime();
    return current_time;
}

std::string Utils_NS::Time::getCurrentDate() noexcept {
    getCurrentDateTime();
    return current_date;
}

std::string Utils_NS::Time::getCurrentDateTime() noexcept {
    std::time_t current_date_time = std::time(nullptr);
    std::tm* now = std::localtime(&current_date_time);

    current_date = to_string_and_appbegin(now->tm_year+1900, 4) + "_" + to_string_and_appbegin(now->tm_mon+1, 2) + "_" + to_string_and_appbegin(now->tm_mday, 2);
    current_time = to_string_and_appbegin(now->tm_hour     , 2) + "_" + to_string_and_appbegin(now->tm_min  , 2) + "_" + to_string_and_appbegin(now->tm_sec , 2);

    return current_date + "_" + current_time;
}
