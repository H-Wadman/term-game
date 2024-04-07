# TODO
- BUG: If min\_width of menu struct is not at least 2 characters larger than largest choice width menu painting will fail, since diamond + space won't fit.
Box drawing around options also goes weird needs investigation
- Reorganise generalised menu options
    - Check if macros can be moved from menu.h to menu\_constants.h or even menu\_constants.c as that will reduce global macro definitions
    - Update/create documentation, especially those pertaining to generalised menus and things like naming convention for enum of choices
    - Automatically include enum return type somewhere, as in order to force enum definition?
- Change clang-tidy in order to avoid proliferation of //NOLINT
- Do documentation in general
- Continue reading up on gettext in order to prepare internationalisation + posibility to turn nerd font on/off
- Check license for ascii fonts used (should be okay since they're on a site but never hurts to double check)
- Finish promised menu funcionality (justify + left/right pad)