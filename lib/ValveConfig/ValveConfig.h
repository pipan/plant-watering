#ifndef ValveConfig_h
#define ValveConfig_h

struct ValveConfig {
    short cycle;
    short volume;
    char split;
    char autoAdjust;
    long lastWatteringAt;
    char lastWatteringProgress;
    short calibrationFirstVolume;
    short calibrationOtherVolume;
};

#endif