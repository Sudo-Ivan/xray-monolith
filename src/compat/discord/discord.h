#pragma once

// Minimal discord Game SDK surface for the Linux build.
// The vendored SDK (src/3rd party/discord) wraps the Windows C ffi layer;
// on Linux discord integration is compiled out. This header satisfies
// includes so call sites keep their shape. UpdateActivity/SetLogHook/etc
// are no-ops.

namespace discord
{
enum class Result
{
    Ok = 0,
    ServiceUnavailable = 1
};

enum class LogLevel
{
    Error = 1,
    Warn,
    Info,
    Debug
};

enum class ActivityType
{
    Playing = 0,
    Streaming,
    Listening,
    Watching
};

class ActivityAssets
{
public:
    ActivityAssets& SetLargeImage(const char*) { return *this; }
    ActivityAssets& SetLargeText(const char*) { return *this; }
    ActivityAssets& SetSmallImage(const char*) { return *this; }
    ActivityAssets& SetSmallText(const char*) { return *this; }
};

class ActivityTimestamps
{
public:
    ActivityTimestamps& SetStart(long long) { return *this; }
    ActivityTimestamps& SetEnd(long long) { return *this; }
};

class Activity
{
public:
    Activity& SetType(ActivityType) { return *this; }
    Activity& SetState(const char*) { return *this; }
    Activity& SetDetails(const char*) { return *this; }
    ActivityAssets& GetAssets() { return assets_; }
    ActivityTimestamps& GetTimestamps() { return ts_; }

private:
    ActivityAssets assets_;
    ActivityTimestamps ts_;
};

class ActivityManager
{
public:
    template <typename F>
    void UpdateActivity(const Activity&, F cb)
    {
        cb(Result::ServiceUnavailable);
    }
    template <typename F>
    void ClearActivity(F cb)
    {
        cb(Result::ServiceUnavailable);
    }
};

class Core
{
public:
    static Result Create(long long, unsigned long long, Core**)
    {
        return Result::ServiceUnavailable;
    }
    void RunCallbacks() {}
    template <typename F>
    void SetLogHook(LogLevel, F)
    {
    }
    ActivityManager& ActivityManager() { return mgr_; }

private:
    discord::ActivityManager mgr_;
};
} // namespace discord
