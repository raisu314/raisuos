#ifndef I18N_H
#define I18N_H

#include "types.h"

typedef enum { LANG_EN, LANG_JA, LANG_FI, LANG_COUNT } language_t;

typedef enum {
  STR_DESKTOP,
  STR_TERMINAL,
  STR_EDITOR,
  STR_FILES,
  STR_SYSMON,
  STR_SETTINGS,
  STR_ABOUT,
  STR_MENU,
  STR_CLOSE,
  STR_MINIMIZE,
  STR_MAXIMIZE,
  STR_RESTORE,
  STR_LANG,
  STR_DISPLAY,
  STR_SYSTEM,
  STR_MEMORY,
  STR_CPU,
  STR_UPTIME,
  STR_PROCESSES,
  STR_FILEMGR,
  STR_TEXTEDIT,
  STR_SYSMON_APP,
  STR_OPEN,
  STR_SAVE,
  STR_NEW,
  STR_HELP,
  STR_VERSION,
  STR_SEARCH,
  STR_APPS,
  STR_VIRTUAL_DESK,
  STR_DESK1,
  STR_DESK2,
  STR_DESK3,
  STR_DESK4,
  STR_REBOOT,
  STR_SHUTDOWN,
  STR_WELCOME,
  STR_COUNT
} string_id_t;

void i18n_init(void);
void i18n_set_language(language_t lang);
language_t i18n_get_language(void);
const char *i18n_get(string_id_t id);
const char *i18n_get_lang_name(language_t lang);

#endif
