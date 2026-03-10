#include "../ui/instances/text_editor/text_editor.hpp"
#include <vxcore/include/vortex.h>
#include <vxcore/include/vortex_internals.h>
#include <vxgui/editor/app/src/editor.hpp>

#ifndef SAMPLE_MODULE_HPP
#define SAMPLE_MODULE_HPP

namespace TextEdit {
struct Context {
  std::shared_ptr<ModuleInterface> m_interface;
  std::vector<std::shared_ptr<ModuleUI::TextEditorAppWindow>>
      m_text_editor_instances;
};
} // namespace TextEdit

#ifndef TEXTEDIT_API
#define TEXTEDIT_API
#endif

#ifndef CTextEdit
extern TEXTEDIT_API TextEdit::Context
    *CTextEdit; // Current implicit context pointer
#endif

// The code API of the module.
namespace TextEdit {
TEXTEDIT_API void CreateContext();
TEXTEDIT_API void DestroyContext();
TEXTEDIT_API void HelloWorld();
TEXTEDIT_API void FunctionWithArg(ArgumentValues &val);
TEXTEDIT_API void FunctionWithRet(ReturnValues &ret);
TEXTEDIT_API void FunctionWithArgRet(ArgumentValues &val, ReturnValues &ret);
TEXTEDIT_API void OutputHandleHello();
TEXTEDIT_API void InputHello();
TEXTEDIT_API std::string GetPath(const std::string &path);

TEXTEDIT_API void StartTextEditorInstance(const std::string &path);
TEXTEDIT_API bool IsValidFile(const std::string &path);
} // namespace TextEdit

#endif // SAMPLE_MODULE_HPP