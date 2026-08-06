#include "module.hpp"

// runtime pointer
#ifndef CTextEdit
std::weak_ptr<TextEdit::Context> CTextEdit;
#endif

std::shared_ptr<TextEdit::Context> TextEdit::create_context() {
  auto ctx = std::make_shared<TextEdit::Context>();

  set_current_context(ctx);

  return ctx;
}

void TextEdit::destroy_context(std::shared_ptr<TextEdit::Context> ctx) {
  set_current_context(nullptr);
}

void TextEdit::set_current_context(std::shared_ptr<TextEdit::Context> ctx) {
  CTextEdit = ctx;
}

std::shared_ptr<TextEdit::Context> TextEdit::get_current_context() {
  return CTextEdit.lock();
}

std::string TextEdit::get_path(const std::string &path) {
  return get_current_context()->m_interface->cook_path(path);
}

bool TextEdit::IsValidFile(const std::string &path) {
  namespace fs = std::filesystem;

  if (!fs::is_directory(path)) {
    return false;
  }

  for (const auto &entry : fs::directory_iterator(path)) {
    if (entry.is_regular_file() &&
        entry.path().filename() == "SampleConfig.txt") {
      return true;
    }
  }

  return false;
}

void TextEdit::StartTextEditorInstance(const std::string &path) {
  std::string filename = fs::path(path).filename().string();

  const size_t maxLen = 24;
  if (filename.size() > maxLen) {
    filename = filename.substr(0, maxLen - 3) + "...";
  }

  std::string window_name =
      filename + "####" +
      std::to_string(get_current_context()->m_text_editor_instances.size());

  auto inst = ModuleUI::TextEditorAppWindow::Create(path, window_name);
  Cherry::AddAppWindow(inst->GetAppWindow());
  get_current_context()->m_text_editor_instances.push_back(inst);
}

void TextEdit::Hello() { vxe::log_info("Tt", "cc"); }
