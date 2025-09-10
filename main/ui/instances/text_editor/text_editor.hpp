#pragma once
#include "../../../../lib/vortex/main/include/vortex.h"
#include "../../../../lib/vortex/main/include/vortex_internals.h"
#include "text_editor_core.hpp"

#ifndef TEXT_EDITOR_HPP
#define TEXT_EDITOR_HPP

namespace ModuleUI {
class TextEditorAppWindow
    : public std::enable_shared_from_this<TextEditorAppWindow> {
public:
  TextEditorAppWindow(const std::string &name, const std::string &path);

  void menubar();

  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<TextEditorAppWindow> Create(const std::string &name,
                                                     const std::string &path);
  void SetupRenderCallback();
  void Render();

private:
  VxContext *ctx;
  bool opened;
  std::string file_path;
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};
}; // namespace ModuleUI

#endif // LOGUTILITY_H
