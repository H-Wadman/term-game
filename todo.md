# TODO
PRIORITY: - [x] Let highlights be passed to menu functions so that Commands can restore the right selection afterward
PRIORITY: - [x] Redefine Command to not include label (?)
PRIORITY: - [] Global input function to handle things like ESC, opening a menu anywhere

Box drawing around options also goes weird needs investigation
    - Check why nerd code point adds a bar after selection in 2nd menu (works on WSL (Ubuntu + Debian))
- Menus:
    - [] Finish promised menu funcionality (justify + left/right pad (+ optional box?))
    - Generalised menu functions should almost certainly be decoupled further and allow for greater freedom in placement
    - Eventual banner should be able to be moved with the optionsbox
    - Rework print_menu_old and print_menu to work together
- Change clang-tidy in order to avoid proliferation of //NOLINT
- Continue reading up on gettext in order to prepare internationalisation + posibility to turn nerd font on/off
- Check license for ascii fonts used (should be okay since they're on a site but never hurts to double check)
- Add visible cursor support
- Make it possible to only print a menu, and then to select it, and only then interact with it
- Make it so that checking for enter on mac/unix works the same (although worked on WSL, so might be fine?)
- get_banner_width currently doesn't take things like trailing whitespace (\n) into account
- Remove test_sudoku once the sudoku part has been properly added to the game
- Unify sudoku board
- Remove logging
