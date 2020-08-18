import sqlite3
from . import SQL

class sqlite( object ):
	def __init__(self,db):
		self.conn = sqlite3.connect(db)
		self.cur = self.conn.cursor()
		self.conn.row_factory = sqlite3.Row
	def end(self):
		self.conn.close()
	def fin(self):
		self.conn.commit()
	def tbl_list(self):
		r = self.select('sqlite_master', ['name'], " WHERE type='table' ORDER BY name")
		return [t[0] for t in r]
	def clmn_list(self, t):
		self.cur.execute("PRAGMA table_info("+t+")")
		r = self.cur.fetchall()
		return [f[1] for f in r]
	def select(self, t, n, u=''):
		q = SQL.select(t, n, u)
		rs = self.cur.execute(q)
		return rs.fetchall()
	def insert(self, t, v, k = None):
		q = SQL.insert(t, v, k)
		try:
			self.cur.execute(q)
			return self.cur.lastrowid
		except sqlite3.Error:
			return 0
