#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
* Parse origin string in the format : "YYYY-MM-DDTHH:MM:SSZ"
*/ 
static daqErrCode daq_timeOriginToSecondsSince1970(const char* origin, int64_t* out_epoch_sec)
{
    int Y, M, D, h, m, s;
    if (!origin)
        return DAQ_ERR_INVALID_DATA;

    if (strlen(origin) != 20) return 0; // 1970-01-01T00:00:00Z is 20 chars
    if (origin[4] != '-' || origin[7] != '-' || origin[10] != 'T' ||
        origin[13] != ':' || origin[16] != ':' || origin[19] != 'Z')
        return DAQ_ERR_INVALID_DATA;

    if (sscanf(origin, "%4d-%2d-%2dT%2d:%2d:%2dZ", &Y, &M, &D, &h, &m, &s) != 6)
        return DAQ_ERR_INVALID_DATA;

    struct tm t = { 0 };
    t.tm_year = Y - 1900;
    t.tm_mon = M - 1;
    t.tm_mday = D;
    t.tm_hour = h;
    t.tm_min = m;
    t.tm_sec = s;

    // tm structo to seconds
#if defined(_MSC_VER)
    *out_epoch_sec = (daqInt)_mkgmtime(&t);
#else
    *out_epoch_sec = (daqInt)timegm(&t);
#endif
    return DAQ_SUCCESS;
}

/**
* Convert seconds and milliseconds since 1970 to a formatted timestamp.
*/
static void daq_secondsSince1970ToTimestamp(daqInt seconds, daqInt milliSeconds, char* out, size_t out_sz)
{
    struct tm tm_utc;
#if defined(_MSC_VER)
    gmtime_s(&tm_utc, &seconds);
#else
    gmtime_r(&sec, &tm_utc);
#endif

    char tmp[32];
    strftime(tmp, sizeof(tmp), "%Y-%m-%dT%H:%M:%S", &tm_utc);
    snprintf(out, out_sz, "%s.%03lldZ", tmp, milliSeconds);
}

/**
* Get absolute time stamp in ms precision from origin, seconds since origin and remaining milliseconds.
* origin_iso8601 is in format "YYYY-MM-DDTHH:MM:SSZ" in UTC. delta_sec is seconds since origin.
*/
static daqErrCode daq_getAbsoluteTimestampMs(const char* originIso8601, daqInt seconds, daqInt milliSeconds, char* out, size_t out_sz)
{
    daqErrCode err = DAQ_SUCCESS;

    daqInt originSeconds = 0;
    err = daq_timeOriginToSecondsSince1970(originIso8601, &originSeconds);
    if (err != DAQ_SUCCESS)
        return err;

    // Add origin + delta in microseconds.
    // (delta_us can be large; this is still safe as long as you stay within int64 range)
    daqInt totalSeconds = originSeconds + seconds;

    daq_secondsSince1970ToTimestamp(totalSeconds, milliSeconds, out, out_sz);
    return err;
}