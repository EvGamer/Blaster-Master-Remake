#pragma once
#include "../typeAliases.h"
#include <memory>

class MainWindow;

using MainWindowPtr = std::shared_ptr<MainWindow>;

class MainWindow {
  public:
    static MainWindowPtr init(uint16_t width, uint16_t height, String title);
    static MainWindowPtr get();
    ~MainWindow();
  private:
    MainWindow(uint16_t width, uint16_t height, String title);
    static bool _isOpened; 
    static MainWindowPtr _instance;
};
