# TODO
PRIORITY ONE: - Do documentation in general and learn doxygen
Box drawing around options also goes weird needs investigation
    - Check why nerd code point adds a bar after selection in 2nd menu (works on WSL (Ubuntu + Debian))
- Menus:
    - Reorganise generalised menu options
        - Check if macros can be moved from menu.h to menu\_constants.h or even menu\_constants.c as that will reduce global macro definitions
        - Update/create documentation, especially those pertaining to generalised menus and things like naming convention for enum of choices
        - Automatically include enum return type somewhere, as in order to force enum definition?
    - Finish promised menu funcionality (justify + left/right pad (+ optional box?))
    - Generalised menu functions should almost certainly be decoupled further and allow for greater freedom in placement
    - Eventual banner should be able to be moved with the optionsbox
    - Rework print_menu_old and print_menu to work together
- Change clang-tidy in order to avoid proliferation of //NOLINT
- Continue reading up on gettext in order to prepare internationalisation + posibility to turn nerd font on/off
- Check license for ascii fonts used (should be okay since they're on a site but never hurts to double check)
- Add visible cursor support
- Make it possible to only print a menu, and then to select it, and only then interact with it
- Make it so that checking for enter on mac/unix works the same (although worked on WSL, so might be fine?)
- Set up cmake + a header file in order to get hte build path
- get_banner_width currently doesn't take things like trailing whitespade (\n) into account
