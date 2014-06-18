# coding: utf8
# gameagent.py
# 5/2/2014 jichi

from functools import partial
from PySide.QtCore import QObject, Signal, QTimer
from sakurakit.skclass import memoized, Q_Q
from sakurakit.skdebug import dprint
from vnragent import vnragent
from mytr import my
import config, growl, sharedmem, settings

@memoized
def global_(): return GameAgent()

class GameAgent(QObject):
  def __init__(self, parent=None):
    super(GameAgent, self).__init__(parent)
    self.__d = _GameAgent(self)

  processAttached = Signal(long) # pid
  processDetached = Signal(long) # pid

  processAttachTimeout = Signal(long)
  engineChanged = Signal(str) # name

  # Not used
  #def clear(self): self.__d.clear()

  ## Inject ##

  def isAttached(self): return bool(self.__d.injectedPid)
  def attachedPid(self): return self.__d.injectedPid # -> long not None

  def isConnected(self): return bool(self.__d.connectedPid)
  def connectedPid(self): return self.__d.connectedPid # -> long not None

  def attachProcess(self, pid): # -> bool
    d = self.__d
    if pid == d.injectedPid:
      return True
    else:
      if d.connectedPid:
        self.detachProcess()
      d.clear()
      import inject
      ok = inject.inject_vnragent(pid=pid)
      if ok:
        d.injectedPid = pid
        d.injectTimer.start()
      return ok

  def detachProcess(self):
    if self.__d.connectedPid:
      rpc = self.__d.rpc
      rpc.disableAgent()
      #rpc.detachAgent()
      rpc.closeAgent()
    self.__d.clear()

  def hasEngine(self): return bool(self.__d.engineName)
  def engine(self): return self.__d.engineName

  ## Query ##

  @staticmethod
  def guessEngine(**kwargs):
    """
    @param* pid  long
    @param* path  unicode  game executable path
    @return  vnragent.Engine
    """
    return vnragent.match(**kwargs)

  ## States ##

  def quit(self):
    d = self.__d
    d.mem.quit()
    if d.connectedPid:
      d.rpc.disableAgent()

  #def setGameLanguage(self, v):
  #  self.__d.gameLanguage = v

  #def setUserEncoding(self, v): # str ->
  #  d = self.__d
  #  if d.userEncoding != v:
  #    d.userEncoding = v

  def sendSettings(self):
    if self.isConnected():
      self.__d.sendSettings()

  def encoding(self): return self.__d.gameEncoding

  def setEncoding(self, v):
    d = self.__d
    if v != d.gameEncoding:
      d.gameEncoding = v
      if d.connectedPid:
        d.sendSetting('gameEncoding', v)

  def scenarioSignature(self): return self.__d.scenarioSignature

  def setScenarioSignature(self, v):
    d = self.__d
    if v != d.scenarioSignature:
      d.scenarioSignature = v
      if d.connectedPid:
        d.sendSetting('scenarioSignature', v)

  def nameSignature(self): return self.__d.nameSignature

  def setNameSignature(self, v):
    d = self.__d
    if v != d.nameSignature:
      d.nameSignature = v
      if d.connectedPid:
        d.sendSetting('nameSignature', v)

  def setExtractsAllTexts(self, v):
    d = self.__d
    if v != d.extractsAllTexts:
      d.extractsAllTexts = v
      if d.connectedPid:
        d.sendSetting('embeddedAllTextsExtracted', v)

  # Shared memory

  def sendEmbeddedTranslation(self, text, hash, role):
    """
    @param  text  unicode
    @param  hash  str or int64
    @param  role  int
    """
    if isinstance(hash, str) or isinstance(hash, unicode):
      hash = long(hash)
    m = self.__d.mem
    if m.isAttached(): # and m.lock():
      # Due to the logic, locking is not needed
      m.setDataStatus(m.STATUS_BUSY)
      m.setDataHash(hash)
      m.setDataRole(role)
      m.setDataText(text)
      m.setDataStatus(m.STATUS_READY)
      #m.unlock()

  #def engine

_SETTINGS_DICT = {
  'windowTranslationEnabled': 'isWindowTranslationEnabled',
  'windowTranscodingEnabled': 'isWindowTranscodingEnabled',
  'windowTextVisible': 'isWindowTextVisible',

  'embeddedTranslationWaitTime': 'embeddedTranslationWaitTime',
  'embeddedTextCancellableByControl': 'isEmbeddedTextCancellableByControl',

  'embeddedScenarioVisible': 'isEmbeddedScenarioVisible',
  'embeddedScenarioTranslationEnabled': 'isEmbeddedScenarioTranslationEnabled',
  'embeddedScenarioTranscodingEnabled': 'isEmbeddedScenarioTranscodingEnabled',
  'embeddedNameVisible': 'isEmbeddedNameVisible',
  'embeddedNameTextVisible': 'isEmbeddedNameTextVisible',
  'embeddedNameTranslationEnabled': 'isEmbeddedNameTranslationEnabled',
  'embeddedNameTranscodingEnabled': 'isEmbeddedNameTranscodingEnabled',
  'embeddedOtherVisible': 'isEmbeddedOtherVisible',
  'embeddedOtherTranslationEnabled': 'isEmbeddedOtherTranslationEnabled',
  'embeddedOtherTranscodingEnabled': 'isEmbeddedOtherTranscodingEnabled',
}

@Q_Q
class _GameAgent(object):
  def __init__(self, q):
    self.mem = sharedmem.VnrAgentSharedMemory()

    import rpcman
    self.rpc = rpcman.manager()

    self.rpc.agentConnected.connect(q.processAttached)
    self.rpc.agentDisconnected.connect(q.processDetached)
    self.rpc.engineReceived.connect(self._onEngineReceived)

    t = self.injectTimer = QTimer(q)
    t.setSingleShot(False)
    t.setInterval(5000)
    t.timeout.connect(self._onInjectTimeout)

    q.processAttached.connect(self._onAttached)
    q.processDetached.connect(self._onDetached)

    self.clear()

    ss = settings.global_()
    for k,v in _SETTINGS_DICT.iteritems():
      sig = getattr(ss, k + 'Changed')

      sig.connect(partial(lambda k, t:
        self.connectedPid and self.sendSetting(k, t)
      , k))

    import textman
    self.textExtractionEnabled = textman.manager().isEnabled()
    textman.manager().enabledChanged.connect(self._setTextExtractionEnabled)

    # Got this value from embeddedprefs.py
    self.extractsAllTexts = False

  def _setTextExtractionEnabled(self, t):
    if self.textExtractionEnabled != t:
      self.textExtractionEnabled = t
      if self.connectedPid:
        self.sendSetting('embeddedTextEnabled', t)

  def clear(self):
    self.injectedPid = 0 # long
    self.engineName = '' # str
    self.gameEncoding = 'shift-jis' # placeholder

    self.scenarioSignature = 0
    self.nameSignature = 0

  @property # read only
  def connectedPid(self): return self.rpc.agentProcessId()

  def _onInjectTimeout(self):
    if self.injectedPid:
      self.q.processAttachTimeout.emit(self.injectedPid)
      self.injectedPid = 0

  def _onAttached(self):
    self.injectTimer.stop()
    self.sendSettings()
    #self.rpc.enableAgent()

  def _onDetached(self, pid): # long ->
    self.mem.detachProcess(pid)

  def _onEngineReceived(self, name): # str
    self.engineName = name
    self.q.engineChanged.emit(name)

    if name and self.connectedPid:
      self.mem.attachProcess(self.connectedPid)

      growl.notify("%s: %s" % (my.tr("Detect game engine"), name))
    else:
      growl.notify(my.tr("Unrecognized game engine. Fallback to ITH."))

  def sendSettings(self):
    ss = settings.global_()
    data = {k:apply(getattr(ss, v)) for k,v in _SETTINGS_DICT.iteritems()}
    data['debug'] = config.APP_DEBUG
    data['gameEncoding'] = self.gameEncoding
    data['embeddedTextEnabled'] = self.textExtractionEnabled
    data['embeddedAllTextsExtracted'] = self.extractsAllTexts
    data['scenarioSignature'] = self.scenarioSignature
    data['nameSignature'] = self.nameSignature
    self.rpc.setAgentSettings(data)

  def sendSetting(self, k, v):
    data = {k:v}
    self.rpc.setAgentSettings(data)

# EOF
