#include "text_editor.hpp"

#include <filesystem>
#include <iostream>
#include <string>

namespace ModuleUI {
TextEditorAppWindow::TextEditorAppWindow(const std::string &path) {
  namespace fs = std::filesystem;

  std::string filename = fs::path(path).filename().string();

  const size_t maxLen = 24;
  if (filename.size() > maxLen) {
    filename = filename.substr(0, maxLen - 3) + "...";
  }

  m_AppWindow = std::make_shared<Cherry::AppWindow>(filename, filename);
  m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
  m_AppWindow->SetLeftMenubarCallback([this]() { RenderMenubar(); });
  m_AppWindow->SetRightMenubarCallback([this]() { RenderRightMenubar(); });

  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
  m_FilePath = path;

  RefreshFile();

  this->ctx = VortexMaker::GetCurrentContext();
}

std::shared_ptr<Cherry::AppWindow> &TextEditorAppWindow::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<TextEditorAppWindow>
TextEditorAppWindow::Create(const std::string &path) {
  auto instance =
      std::shared_ptr<TextEditorAppWindow>(new TextEditorAppWindow(path));
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

void TextEditorAppWindow::RenderMenubar() {
  CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + 3.0f);
  CherryNextComponent.SetProperty("padding_y", "6.0f");
  CherryNextComponent.SetProperty("padding_x", "10.0f");

  if (CherryKit::ButtonImageText(
          "Save", GetPath("resources/imgs/icons/misc/icon_add.png"))
          .GetDataAs<bool>("isClicked")) {
    m_SavePending = true;
  }

  CherryNextComponent.SetProperty("padding_y", "6.0f");
  CherryNextComponent.SetProperty("padding_x", "10.0f");
  if (CherryKit::ButtonImageText(
          "Refresh", GetPath("resources/imgs/icons/misc/icon_add.png"))
          .GetDataAs<bool>("isClicked")) {
    m_RefreshReady = true;
  }

  if (m_AutoRefresh) {
    CherryKit::TextSimple("Auto refresh activated");
  }
}

void TextEditorAppWindow::RefreshFile() {
  try {
    if (m_FilePath.empty()) {
      std::cerr << "RefreshFile: no file path set\n";
      return;
    }

    namespace fs = std::filesystem;
    std::error_code ec;

    if (!fs::exists(m_FilePath, ec)) {
      std::cerr << "RefreshFile: file does not exist: " << m_FilePath << "\n";
      return;
    }

    std::ifstream ifs(m_FilePath, std::ios::binary);
    if (!ifs.is_open()) {
      std::cerr << "RefreshFile: failed to open file: " << m_FilePath << "\n";
      return;
    }

    std::string content;
    ifs.seekg(0, std::ios::end);
    auto size = ifs.tellg();
    if (size > 0) {
      content.resize(static_cast<size_t>(size));
      ifs.seekg(0, std::ios::beg);
      ifs.read(&content[0], size);
    } else {
      content.clear();
    }
    ifs.close();

    if (content != m_FileEditBuffer) {
      m_FileEditBuffer = std::move(content);
    }

    m_LastWriteTime = fs::last_write_time(m_FilePath, ec);

  } catch (const std::exception &e) {
    std::cerr << "RefreshFile: exception: " << e.what() << "\n";
  }
}

void TextEditorAppWindow::SaveFile() {
  try {
    if (m_FilePath.empty()) {
      std::cerr << "SaveFile: no file path set\n";
      return;
    }

    namespace fs = std::filesystem;
    std::error_code ec;

    fs::path target = m_FilePath;
    fs::path parent = target.parent_path();
    if (!parent.empty() && !fs::exists(parent, ec)) {
      if (!fs::create_directories(parent, ec)) {
        std::cerr << "SaveFile: unable to create parent directories: " << parent
                  << " (" << ec.message() << ")\n";
        return;
      }
    }

    auto timestamp =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
    fs::path tempPath = parent / (target.filename().string() + ".tmp." +
                                  std::to_string(timestamp));

    {
      std::ofstream ofs(tempPath, std::ios::binary | std::ios::trunc);
      if (!ofs.is_open()) {
        std::cerr << "SaveFile: failed to open temp file for writing: "
                  << tempPath << "\n";
        std::error_code rmec;
        fs::remove(tempPath, rmec);
        return;
      }

      ofs.write(m_FileEditBuffer.data(),
                static_cast<std::streamsize>(m_FileEditBuffer.size()));
      if (!ofs) {
        std::cerr << "SaveFile: write failed to temp file: " << tempPath
                  << "\n";
        ofs.close();
        fs::remove(tempPath, ec);
        return;
      }
      ofs.flush();
      ofs.close();
    }

    if (fs::exists(target, ec)) {
      std::error_code removeEc;
      fs::remove(target, removeEc);
      std::error_code renameEc;
      fs::rename(tempPath, target, renameEc);
      if (renameEc) {
        std::error_code copyEc;
        fs::copy_file(tempPath, target, fs::copy_options::overwrite_existing,
                      copyEc);
        if (copyEc) {
          std::cerr << "SaveFile: failed to replace target file: " << target
                    << " (rename: " << renameEc.message()
                    << ", copy: " << copyEc.message() << ")\n";
          fs::remove(tempPath, ec);
          return;
        }
        fs::remove(tempPath, ec);
      }
    } else {
      std::error_code renameEc;
      fs::rename(tempPath, target, renameEc);
      if (renameEc) {
        std::error_code copyEc;
        fs::copy_file(tempPath, target, fs::copy_options::none, copyEc);
        if (copyEc) {
          std::cerr << "SaveFile: failed to move temp file to target: "
                    << target << " (rename: " << renameEc.message()
                    << ", copy: " << copyEc.message() << ")\n";
          fs::remove(tempPath, ec);
          return;
        }
        fs::remove(tempPath, ec);
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "SaveFile: exception: " << e.what() << "\n";
  }
}

void TextEditorAppWindow::Undo() { m_UndoPending = true; }
void TextEditorAppWindow::Redo() { m_RedoPending = true; }

void TextEditorAppWindow::Render() {
  CherryApp.PushComponentPool(&m_ComponentPool);

  auto test = CherryGUI::GetContentRegionAvail();
  auto &editor = ModuleUI::TextArea(&test.x, &test.y, &m_FileEditBuffer);

  if (editor.GetData("save_ready") == "true") {
    m_SaveReady = true;
    editor.SetData("save_ready", "false");
  }

  if (m_SavePending) {
    editor.SetProperty("save_pending", "true");
    m_SavePending = false;
  }

  if (m_UndoPending) {
    editor.SetProperty("undo_pending", "true");
    m_UndoPending = false;
  }

  if (m_RedoPending) {
    editor.SetProperty("redo_pending", "true");
    m_RedoPending = false;
  }

  if (m_SaveReady) {
    SaveFile();
    m_SaveReady = false;
  }

  if (m_AutoRefresh && !m_FilePath.empty()) {
    namespace fs = std::filesystem;
    std::error_code ec;
    auto currentWriteTime = fs::last_write_time(m_FilePath, ec);
    if (!ec && currentWriteTime != m_LastWriteTime) {
      RefreshFile();
      editor.SetProperty("refresh_pending", "true");
    }
  }

  if (m_RefreshReady) {
    RefreshFile();
    editor.SetProperty("refresh_pending", "true");
    m_RefreshReady = false;
  }

  CherryApp.PopComponentPool();
}

void TextEditorAppWindow::RenderRightMenubar() {
  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.7f));

  CherryNextComponent.SetProperty("padding_y", "6.0f");
  CherryNextComponent.SetProperty("padding_x", "10.0f");
  CherryNextComponent.SetProperty("disable_callback", "true");
  if (CherryKit::ButtonImageTextDropdown(
          "Settings", GetPath("resources/imgs/icons/misc/icon_settings.png"))
          .GetDataAs<bool>("isClicked")) {
    ImVec2 mousePos = CherryGUI::GetMousePos();
    ImVec2 displaySize = CherryGUI::GetIO().DisplaySize;
    ImVec2 popupSize(150, 100);

    if (mousePos.x + popupSize.x > displaySize.x) {
      mousePos.x -= popupSize.x;
    }
    if (mousePos.y + popupSize.y > displaySize.y) {
      mousePos.y -= popupSize.y;
    }

    CherryGUI::SetNextWindowSize(ImVec2(150, 100), ImGuiCond_Appearing);
    CherryGUI::SetNextWindowPos(mousePos, ImGuiCond_Appearing);
    CherryGUI::OpenPopup("SettingsMenuPopup");
  }
  if (CherryGUI::BeginPopup("SettingsMenuPopup")) {
    CherryKit::CheckboxText("Auto refresh", &m_AutoRefresh);
    CherryGUI::EndPopup();
  }

  CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() - 3.0f);

  CherryGUI::PopStyleColor();
  CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() - 1.5f);
}

}; // namespace ModuleUI
