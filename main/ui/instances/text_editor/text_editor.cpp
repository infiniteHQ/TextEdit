#include "text_editor.hpp"

#include <iostream>
#include <string>

namespace ModuleUI {
TextEditorAppWindow::TextEditorAppWindow(const std::string &name,
                                         const std::string &path) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
  file_path = path;

  this->ctx = VortexMaker::GetCurrentContext();
}

std::shared_ptr<Cherry::AppWindow> &TextEditorAppWindow::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<TextEditorAppWindow>
TextEditorAppWindow::Create(const std::string &name, const std::string &path) {
  auto instance =
      std::shared_ptr<TextEditorAppWindow>(new TextEditorAppWindow(name, path));
  instance->SetupRenderCallback();
  return instance;
}

void TextEditorAppWindow::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self) {
      self->Render();
    }
  });
}

void TextEditorAppWindow::Render() {
  static std::string sdf = "Afdfhsdh sdfhSDf";
  auto test = CherryGUI::GetContentRegionAvail();
  ModuleUI::TextArea(&test.x, &test.y, sdf);
}
}; // namespace ModuleUI
