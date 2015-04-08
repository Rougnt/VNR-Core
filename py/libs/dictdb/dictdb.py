# coding: utf8
# dictdb.py
# 1/16/2014
# See: http://zetcode.com/db/sqlitepythontutorial/

if __name__ == '__main__':
  import sys
  sys.path.append("..")

# For better performance
#try: from pysqlite2 import dbapi2 as sqlite3
#except ImportError: import sqlite3
import sqlite3
from sakurakit.skdebug import dwarn
import dbutil

#TABLE_NAME = 'entry'

def queryentry(cur, word='', wordlike='', limit=0):
  """
  @param  cursor
  @param* word  unicode
  @param* wordlike  unicode
  @param* limit  int
  @return  (unicode word, unicode content)
  @raise
  """
  params = []
  sql = "SELECT word,content FROM entry"
  if word:
    sql += ' where word = ?'
    params.append(word)
  elif wordlike:
    sql += ' where word like ?'
    params.append(wordlike)
  if limit:
    sql += ' limit ?'
    params.append(limit)
  cur.execute(sql, params)
  return cur.fetchone()

def queryentries(cur, word='', wordlike='', limit=0):
  """
  @param  cursor
  @param* word  unicode
  @param* wordlike  unicode
  @param* limit  int
  @return  [unicode word, unicode content]
  @raise
  """
  params = []
  sql = "SELECT word,content FROM entry"
  if word:
    sql += ' where word = ?'
    params.append(word)
  elif wordlike:
    sql += ' where word like ?'
    params.append(wordlike)
  if limit:
    sql += ' limit ?'
    params.append(limit)
  cur.execute(sql, params)
  return cur.fetchall()

# http://stackoverflow.com/questions/3785294/best-way-to-iterate-through-all-rows-in-a-db-table
def iterentries(cur, chunk=100):
  """
  @param  cursor
  @yield  unicode word, unicode content
  @raise
  """
  sql = "SELECT word,content FROM entry"
  cur.execute(sql)
  return dbutil.fetchsome(cur, chunk)

def iterwords(cur, chunk=100):
  """
  @param  cursor
  @yield  unicode
  @raise
  """
  sql = "SELECT word FROM entry order by id" # order by id is needed since there is index on word
  cur.execute(sql)
  for it, in dbutil.fetchsome(cur, chunk):
    yield it

def insertentry(cur, entry, ignore_errors=False): # cursor, entry; raise
  """
  @param  cursor
  @param  entry  (uincode word, unicode content)
  @param* ignore_errors  whether ignore exceptions
  @raise
  """
  sql = "INSERT INTO entry(word,content) VALUES(?,?)"
  if ignore_errors:
    try: cur.execute(sql, entry)
    except Exception, e: dwarn(e)
  else:
    cur.execute(sql, entry)

def insertentries(cur, entries, ignore_errors=False): # cursor, [entry]; raise
  """
  @param  cursor
  @param  entries  [uincode word, unicode content]
  @param* ignore_errors  whether ignore exceptions
  @raise
  """
  for it in entries:
    insertentry(cur, it, ignore_errors=ignore_errors)

def createtables(cur): # cursor -> bool
  """
  @param  cursor
  @return  bool
  @raise
  """
  cur.execute('''\
CREATE TABLE entry(
id INTEGER PRIMARY KEY AUTOINCREMENT,
word TEXT NOT NULL UNIQUE,
content TEXT NOT NULL)
''').execute('''\
CREATE UNIQUE INDEX idx_word ON entry(word ASC)
''')
  return True

def createdb(dbpath): # unicode path -> bool
  """
  @param  dbpath  unicode
  @return  bool
  """
  try:
    with sqlite3.connect(dbpath) as conn:
      cur = conn.cursor()
      createtables(cur)
      conn.commit()
      return True
  except Exception, e:
    dwarn(e)
  return False

if __name__ == '__main__':
  import os
  path = 'test.db'
  if os.path.exists(path):
    os.remove(path)
  print createdb(path)

  with sqlite3.connect(path) as conn:
    cur = conn.cursor()
    insertentry(cur, ('a', 'b'))
    insertentry(cur, ('ab', 'b'))
    conn.commit()

    print queryentry(cur, wordlike='a')

    for it in queryentries(cur, wordlike='%a%'):
      print it

# EOF
