package d;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class SeqGen {
  private String _sql = "values nextval for v1.seq";
  private int _bsize = 100;

  private DB _db = null;

  private int _base = 0;
  private int _counter = 0;

  public SeqGen(String dbname) {
    _db = new DB(dbname);
    _counter = _bsize;
  }

  public SeqGen(DB db) {
    _db = db;
    _counter = _bsize;
  }

  public SeqGen(DB db, String sql, int size) {
    _db = db;
    _sql = sql;
    _bsize = size;
    _counter = _bsize;
  }

  public synchronized int next() {
    if (_counter >= _bsize) {
      Connection conn = null;
      PreparedStatement nextval = null;
      try {
        conn = _db.getConnection();
        nextval = conn.prepareStatement(_sql);

        ResultSet rs = nextval.executeQuery();
        if (rs.next()) {
          _base = rs.getInt(1);
          _counter = 0;
        }
        rs.close();
      } catch (SQLException ex) {
        ex.printStackTrace();
      } finally {
        if (nextval != null)
          try {
            nextval.close();
          } catch (SQLException ex) {
          }
        if (conn != null)
          try {
            conn.close();
          } catch (SQLException ex) {
          }
      }
    }
    return _base + (_counter++);
  }
}
