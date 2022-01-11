#include "MainWindow.h"
#include <raylib.h>

MainWindowPtr MainWindow::_instance{nullptr};

MainWindow::MainWindow(uint16_t width, uint16_t height, String title) {
  InitWindow(width, height, title.c_str());
}

MainWindow::~MainWindow() {
  CloseWindow();
}

MainWindowPtr MainWindow::init(uint16_t width, uint16_t height, String title) {
  if (!_instance) {
    _instance = std::make_shared<MainWindow>(width, height, title);
  }
  return _instance;
}

MainWindowPtr MainWindow::get() {
  return _instance;
}