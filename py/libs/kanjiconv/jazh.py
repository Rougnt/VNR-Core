# coding: utf8
# 4/3/2015 jichi
if __name__ == '__main__':
  import sys
  sys.path.append('..')

from unitraits import jpchars, jpmacros
import re

_J2C_NAME = {
  u'ノ': u'之',
  u'の': u'之',
  u'ヶ': '',
}
_rx_name_repeat = re.compile(
  jpmacros.applymacros(
    ur"({{kanji}})々"
  )
)
def ja2zh_name(text): # unicode -> unicode
  for k,v in _J2C_NAME.iteritems():
    text = text.replace(k, v)
  if u'々' in text:
    text = _rx_name_repeat.sub(r'\1\1', text)
  return text

_rx_name_sep = re.compile(
  jpmacros.applymacros(
    r"{{?<=kanji}}[%s]{{?=kanji}}" % (
      u' 　・＝ー' + ''.join(_J2C_NAME.iterkeys())
    )
  )
)

def ja2zh_name_test(text): # unicode -> bool
  text = _rx_name_sep.sub('', text).replace(u'々', '')
  return bool(text) and jpchars.allkanji(text)

def ja2zht_name_fix(text): # unicode -> unicode
  return text.replace(u'裡', u'里').replace(u'裏', u'里')

if __name__ == '__main__':
  t = u'佐藤'
  t = u'乃々華'
  print ja2zh_name_test(t)
  t = u'乃々華'
  print ja2zh_name(t)

# EOF
