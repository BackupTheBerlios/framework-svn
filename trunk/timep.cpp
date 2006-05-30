#ifdef WIN32
  #include <windows.h>
  //#include <mmsystem.h>
  //#define RESOLUTION 1

  double get_time()
  {
  return (double) GetTickCount() / 1000;
  /*
    static bool being_here = 0;
    static UINT res;
    static TIMECAPS tc;
    if(!being_here){
      being_here = 1;
      if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) throw "Clock function did not work.";
      res = min(max(tc.wPeriodMin, RESOLUTION), tc.wPeriodMax);
      timeBeginPeriod(res);
    }
    return 0.001 * timeGetTime();
    */
  }

#else // LINUX
  #include <time.h>

  double get_time(){
    struct timespec t;
    if(clock_gettime(CLOCK_MONOTONIC, &t)) throw "Clock function did not work.";
    return (double)(t.tv_sec) + 1e-9 * t.tv_nsec;
  };

#endif // LINUX
