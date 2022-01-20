class Timer {
  public:
    Timer(float completeTimeSec) : 
      _initialTime(completeTimeSec),
      _timeLeft(completeTimeSec)
    {};
    inline void start() { 
      _isStopped = false;
    };
    inline void reset() { 
      _timeLeft = _initialTime;
    };
    inline void restart() {
      reset();
      start();
    }
    void stop() {
      _isStopped = true;
    };
    inline const bool isDone() { 
      return _timeLeft <= 0; 
    }
    inline const bool& isStopped() { 
      return _isStopped; 
    }
    inline bool isRunning() {
      return !(_isStopped || isDone());
    }
    inline bool update(float timePassed) {
      if (isRunning()) 
        _timeLeft -= timePassed;
      return isDone();
    }
  private:
    float _initialTime;
    float _timeLeft;
    bool _isStopped = false;
};
