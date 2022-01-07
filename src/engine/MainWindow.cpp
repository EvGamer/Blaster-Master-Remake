#include "MainWindow.h"
#include <raylib.h>

bool MainWindow::_isOpened = false;
UInt MainWindow::_useCounter = 0;

MainWindow::MainWindow(int width, int height, String title) {
  if (!_isOpened) {

    InitWindow(width, height, title.c_str());
    _isOpened = true;
  }
  _useCounter++;
}

void MainWindow::operator=(const MainWindow& toCopy) {
  _useCounter++;
}

MainWindow::~MainWindow() {
  _useCounter--;
  if (_isOpened && _useCounter == 0) 
    CloseWindow();
}
