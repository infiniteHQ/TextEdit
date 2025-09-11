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
  TextEditorAppWindow(const std::string &path);

  void menubar();

  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<TextEditorAppWindow> Create(const std::string &path);
  void SetupRenderCallback();
  void Render();
  void RenderMenubar();
  void RenderRightMenubar();

  void RefreshFile();
  void SaveFile();
  void Undo();
  void Redo();

private:
  VxContext *ctx;
  bool opened;
  std::string m_FileEditBuffer;
  std::string m_FilePath;

  // Editor actions
  bool m_UndoPending = false;
  bool m_RedoPending = false;
  bool m_SavePending = false;

  // Editor flags
  bool m_SaveReady = false;
  bool m_RefreshReady = false;

  bool m_AutoRefresh = false;
  std::filesystem::file_time_type m_LastWriteTime{};

  // Cherry
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
  ComponentsPool m_ComponentPool;
};
}; // namespace ModuleUI

#endif // LOGUTILITY_H
