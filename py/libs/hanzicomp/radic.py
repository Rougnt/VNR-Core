# coding: utf8
# radic.py
# 5/6/2015 jichi
# See: https://github.com/nieldlr/hanzi

def parse(path, encoding='utf8'):
  """
  @return
  """
  import codecs, json
  try:
    with codecs.open(path, 'r', encoding) as f:
      data = f.read()
    _, _, data = data.partition(' = ')
    data, _, _ = data.rpartition(';')
    return json.loads(data)
  except Exception, e:
    from sakurakit.skdebug import dwarn
    dwarn(e)

if __name__ == '__main__':
  path = '../../../../../Dictionaries/hanzi/radicalListWithMeaning.js'
  print parse(path)

# EOF
