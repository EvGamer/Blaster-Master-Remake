#pragma once
#include "../typeAliases.h"

class MainWindow {
  public:
    MainWindow() {};
    MainWindow(int width, int height, String title);
    void operator=(const MainWindow& toCopy);
    ~MainWindow();
  private:
    static bool _isOpened; 
    static UInt _useCounter;
};
