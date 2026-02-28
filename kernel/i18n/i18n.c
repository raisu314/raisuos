#include "i18n.h"

static language_t current_lang = LANG_EN;

/* String tables */
static const char *strings_en[STR_COUNT] = {"Desktop",
                                            "Terminal",
                                            "Editor",
                                            "Files",
                                            "SysMon",
                                            "Settings",
                                            "About",
                                            "Menu",
                                            "Close",
                                            "Minimize",
                                            "Maximize",
                                            "Restore",
                                            "Language",
                                            "Display",
                                            "System",
                                            "Memory",
                                            "CPU",
                                            "Uptime",
                                            "Processes",
                                            "File Manager",
                                            "Text Editor",
                                            "System Monitor",
                                            "Open",
                                            "Save",
                                            "New File",
                                            "Help",
                                            "Version",
                                            "Search",
                                            "Applications",
                                            "Virtual Desktops",
                                            "Desktop 1",
                                            "Desktop 2",
                                            "Desktop 3",
                                            "Desktop 4",
                                            "Reboot",
                                            "Shutdown",
                                            "Welcome to RaisuOS"};

static const char *strings_ja[STR_COUNT] = {"デスクトップ",
                                            "ターミナル",
                                            "エディタ",
                                            "ファイル",
                                            "システムモニター",
                                            "設定",
                                            "アバウト",
                                            "メニュー",
                                            "閉じる",
                                            "最小化",
                                            "最大化",
                                            "元に戻す",
                                            "言語",
                                            "ディスプレイ",
                                            "システム",
                                            "メモリ",
                                            "CPU",
                                            "稼働時間",
                                            "プロセス",
                                            "ファイルマネージャ",
                                            "テキストエディタ",
                                            "システムモニター",
                                            "開く",
                                            "保存",
                                            "新規作成",
                                            "ヘルプ",
                                            "バージョン",
                                            "検索",
                                            "アプリケーション",
                                            "仮想デスクトップ",
                                            "デスクトップ 1",
                                            "デスクトップ 2",
                                            "デスクトップ 3",
                                            "デスクトップ 4",
                                            "再起動",
                                            "シャットダウン",
                                            "RaisuOS へようこそ"};

static const char *strings_fi[STR_COUNT] = {"Työpöytä",
                                            "Pääte",
                                            "Muokkain",
                                            "Tiedostot",
                                            "Järjestelmän valvonta",
                                            "Asetukset",
                                            "Tietoja",
                                            "Valikko",
                                            "Sulje",
                                            "Pienennä",
                                            "Suurenna",
                                            "Palauta",
                                            "Kieli",
                                            "Näyttö",
                                            "Järjestelmä",
                                            "Muisti",
                                            "Suoritin",
                                            "Käyttöaika",
                                            "Prosessit",
                                            "Tiedostonhallinta",
                                            "Tekstimuokkain",
                                            "Järjestelmän valvonta",
                                            "Avaa",
                                            "Tallenna",
                                            "Uusi tiedosto",
                                            "Ohje",
                                            "Versio",
                                            "Hae",
                                            "Sovellukset",
                                            "Virtuaalityöpöydät",
                                            "Työpöytä 1",
                                            "Työpöytä 2",
                                            "Työpöytä 3",
                                            "Työpöytä 4",
                                            "Käynnistä uudelleen",
                                            "Sammuta",
                                            "Tervetuloa RaisuOS:iin"};

static const char **lang_tables[LANG_COUNT] = {strings_en, strings_ja,
                                               strings_fi};

void i18n_init(void) { current_lang = LANG_EN; /* Default */ }

void i18n_set_language(language_t lang) {
  if (lang >= 0 && lang < LANG_COUNT) {
    current_lang = lang;
  }
}

language_t i18n_get_language(void) { return current_lang; }

const char *i18n_get(string_id_t id) {
  if (id >= 0 && id < STR_COUNT) {
    return lang_tables[current_lang][id];
  }
  return "UNKNOWN";
}

const char *i18n_get_lang_name(language_t lang) {
  switch (lang) {
  case LANG_EN:
    return "English";
  case LANG_JA:
    return "日本語";
  case LANG_FI:
    return "Suomi";
  default:
    return "Unknown";
  }
}
