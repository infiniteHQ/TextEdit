#include <vxcore/include/modules/interface.hpp>
#include <vxcore/include/vortex.h>
#include <vxcore/include/vortex_internals.h>

#include <vxgui/editor/main/editor.hpp>

#include "../ui/instances/text_editor/text_editor.hpp"

#ifndef TEXT_EDIT_HPP
#define TEXT_EDIT_HPP

#ifndef TEXTEDIT_API
#define TEXTEDIT_API
#endif

namespace TextEdit {
struct Context {
  std::shared_ptr<ModuleInterface> m_interface;
  std::vector<std::shared_ptr<ModuleUI::TextEditorAppWindow>>
      m_text_editor_instances;
};
} // namespace TextEdit

// context pointer
#ifndef CTextEdit
extern TEXTEDIT_API std::weak_ptr<TextEdit::Context> CTextEdit;
#endif

namespace TextEdit {
// Context
TEXTEDIT_API std::shared_ptr<TextEdit::Context> create_context();
TEXTEDIT_API void destroy_context(std::shared_ptr<TextEdit::Context> ctx);
TEXTEDIT_API void set_current_context(std::shared_ptr<TextEdit::Context> ctx);
TEXTEDIT_API std::shared_ptr<TextEdit::Context> get_current_context();

TEXTEDIT_API std::string get_path(const std::string &path);

TEXTEDIT_API void Hello();

TEXTEDIT_API void StartTextEditorInstance(const std::string &path);
TEXTEDIT_API bool IsValidFile(const std::string &path);
} // namespace TextEdit

#endif // TEXT_EDIT_HPP