// settings.cc
// 5/1/2014 jichi

#include "driver/settings.h"

#define DEBUG "settings"
#include "sakurakit/skdebug.h"

/** Private class */

class SettingsPrivate
{
public:
  bool windowTranslationEnabled
     , windowTranscodingEnabled
     , windowTextVisible
     , embeddedScenarioVisible
     , embeddedScenarioTranslationEnabled
     , embeddedScenarioTranscodingEnabled
     , embeddedNameVisible
     , embeddedNameTranslationEnabled
     , embeddedNameTranscodingEnabled
     , embeddedOtherVisible
     , embeddedOtherTranslationEnabled
     , embeddedOtherTranscodingEnabled;

  QString gameEncoding;

  SettingsPrivate()
     : windowTranslationEnabled(false)
     , windowTranscodingEnabled(false)
     , windowTextVisible(false)
     , embeddedScenarioVisible(false)
     , embeddedScenarioTranslationEnabled(false)
     , embeddedScenarioTranscodingEnabled(false)
     , embeddedNameVisible(false)
     , embeddedNameTranslationEnabled(false)
     , embeddedNameTranscodingEnabled(false)
     , embeddedOtherVisible(false)
     , embeddedOtherTranslationEnabled(false)
     , embeddedOtherTranscodingEnabled(false)
     , gameEncoding("shift-jis")
  {}
};

/** Public class */

static Settings *instance_;
Settings *Settings::instance() { return ::instance_; }

Settings::Settings(QObject *parent)
  : Base(parent), d_(new D)
{
  instance_ = this;
}

Settings::~Settings()
{
  instance_ = nullptr;
  delete d_;
}

#define DEFINE_PROPERTY(property, getter, setter, rettype, argtype) \
  rettype Settings::getter() const \
  { return d_->property; } \
  void Settings::setter(argtype value)  \
  { if (d_->property != value) { d_->property = value; emit property##Changed(value); } }

#define DEFINE_STRING_PROPERTY(property, getter, setter)    DEFINE_PROPERTY(property, getter, setter, QString, const QString &)
#define DEFINE_BOOL_PROPERTY(property, getter, setter)      DEFINE_PROPERTY(property, getter, setter, bool, bool)

DEFINE_BOOL_PROPERTY(windowTranslationEnabled, isWindowTranslationEnabled, setWindowTranslationEnabled)
DEFINE_BOOL_PROPERTY(windowTranscodingEnabled, isWindowTranscodingEnabled, setWindowTranscodingEnabled)
DEFINE_BOOL_PROPERTY(windowTextVisible, isWindowTextVisible, setWindowTextVisible)
DEFINE_BOOL_PROPERTY(embeddedScenarioVisible, isEmbeddedScenarioVisible, setEmbeddedScenarioVisible)
DEFINE_BOOL_PROPERTY(embeddedScenarioTranslationEnabled, isEmbeddedScenarioTranslationEnabled, setEmbeddedScenarioTranslationEnabled)
DEFINE_BOOL_PROPERTY(embeddedNameVisible, isEmbeddedNameVisible, setEmbeddedNameVisible)
DEFINE_BOOL_PROPERTY(embeddedNameTranslationEnabled, isEmbeddedNameTranslationEnabled, setEmbeddedNameTranslationEnabled)
DEFINE_BOOL_PROPERTY(embeddedOtherVisible, isEmbeddedOtherVisible, setEmbeddedOtherVisible)
DEFINE_BOOL_PROPERTY(embeddedOtherTranslationEnabled, isEmbeddedOtherTranslationEnabled, setEmbeddedOtherTranslationEnabled)

DEFINE_STRING_PROPERTY(gameEncoding, gameEncoding, setGameEncoding)

// Groupped settings

void Settings::disable()
{
  setWindowTranslationEnabled(false);
  setWindowTextVisible(false);
  //setWindowTranscodingEnabled(false);

  setEmbeddedScenarioVisible(true);
  setEmbeddedScenarioTranslationEnabled(false);
  setEmbeddedNameVisible(true);
  setEmbeddedNameTranslationEnabled(false);
  setEmbeddedOtherVisible(true);
  setEmbeddedOtherTranslationEnabled(false);
}

bool Settings::isWindowDriverNeeded() const
{ return isWindowTranslationEnabled() || isWindowTranscodingEnabled(); }

bool Settings::isEmbeddedTextNeeded() const { return true; } // placeholder

bool Settings::isEmbedDriverNeeded() const
{
  return isEmbeddedTextNeeded()
      || isEmbeddedScenarioTranslationEnabled() || !isEmbeddedScenarioVisible()
      || isEmbeddedNameTranslationEnabled() || !isEmbeddedNameVisible()
      || isEmbeddedOtherTranslationEnabled() || !isEmbeddedOtherVisible();
}

// Marshal

#include "QxtCore/QxtJSON"
#include <QtCore/QVariantMap>
#include "util/msghandler.h"

void Settings::load(const QString &json)
{
  enum {
    H_debug = 6994359 // "debug"
    , H_gameEncoding = 156622791
    , H_windowTranslationEnabled = 79059828
    , H_windowTranscodingEnabled = 219567700
    , H_windowTextVisibleChange = 190229845
    , H_embeddedScenarioVisible = 207043173
    , H_embeddedScenarioTranslationEnabled = 132391348
    , H_embeddedScenarioTranscodingEnabled = 105135476
    , H_embeddedNameVisible = 180590501
    , H_embeddedNameTranslationEnabled = 239147220
    , H_embeddedNameTranscodingEnabled = 19782804
    , H_embeddedOtherVisible = 32685349
    , H_embeddedOtherTranslationEnabled = 9290068
    , H_embeddedOtherTranscodingEnabled = 19782804
  };

  QVariant data = QxtJSON::parse(json);
  if (data.isNull())
    return;
  QVariantMap map = data.toMap();
  if (map.isEmpty())
    return;

  for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
    QString value = it.value().toString();
    bool bValue = value == "true";
    switch (qHash(it.key())) {
    case H_debug: if (bValue) Util::installDebugMsgHandler(); break;
    case H_gameEncoding: setGameEncoding(value); break;
    case H_windowTranslationEnabled: setWindowTranslationEnabled(bValue); break;
    case H_windowTranscodingEnabled: setWindowTranscodingEnabled(bValue); break;
    case H_windowTextVisibleChange: setWindowTextVisible(bValue); break;
    case H_embeddedScenarioVisible: setEmbeddedScenarioVisible(bValue); break;
    case H_embeddedScenarioTranslationEnabled: setEmbeddedScenarioTranslationEnabled(bValue); break;
    case H_embeddedScenarioTranscodingEnabled: setEmbeddedScenarioTranscodingEnabled(bValue); break;
    case H_embeddedNameVisible: setEmbeddedNameVisible(bValue); break;
    case H_embeddedNameTranslationEnabled: setEmbeddedNameTranslationEnabled(bValue); break;
    case H_embeddedNameTranscodingEnabled: setEmbeddedNameTranscodingEnabled(bValue); break;
    case H_embeddedOtherVisible: setEmbeddedOtherVisible(bValue); break;
    case H_embeddedOtherTranslationEnabled: setEmbeddedOtherTranslationEnabled(bValue); break;
    case H_embeddedOtherTranscodingEnabled: setEmbeddedOtherTranscodingEnabled(bValue); break;
    default: DOUT("warning: unknown key:" << it.key());
    }
  }

  emit loadFinished();
}

// EOF
