# TODO
Box drawing around options also goes weird needs investigation
- Menus:
    - Reorganise generalised menu options
        - Check if macros can be moved from menu.h to menu\_constants.h or even menu\_constants.c as that will reduce global macro definitions
        - Update/create documentation, especially those pertaining to generalised menus and things like naming convention for enum of choices
        - Automatically include enum return type somewhere, as in order to force enum definition?
    - Finish promised menu funcionality (justify + left/right pad (+ optional box?))
    - Generalised menu functions should almost certainly be decoupled further and allow for greated freedom in placement
- Change clang-tidy in order to avoid proliferation of //NOLINT
- Do documentation in general
- Continue reading up on gettext in order to prepare internationalisation + posibility to turn nerd font on/off
- Check license for ascii fonts used (should be okay since they're on a site but never hurts to double check)
- Automate static analysis from clang static analyser and cppcheck?
- Add visible cursor support
- Check why nerd code point adds a bar after selection in 2nd menu
