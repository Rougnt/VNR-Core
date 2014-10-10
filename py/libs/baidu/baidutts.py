# coding: utf8
# baidutts.py
# 10/7/2014 jichi
#
# Example: http://tts.baidu.com/text2audio?lan=jp&ie=UTF-8&text=hello

from sakurakit.sknetio import topercentencoding
import baidudef

API = "http://tts.baidu.com/text2audio"

def url(text, language, encoding='UTF-8'):
  """
  @param  text  unicode  not None
  @param  language  str  not None
  @param* encoding  utf8 or UTF-8 or None
  @return  unicode or str not None
  """
  if language:
    text = topercentencoding(text, encoding)
    if text:
      language = baidudef.bdlang(language)
      return API + "?ie=%s&lan=%s&text=%s" % (encoding, language, text)
  return ''

if __name__ == '__main__':
  print url("hello", 'ja')
  print url(u"こんにちは", 'ja')

# EOF
