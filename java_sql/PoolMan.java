package d;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.SQLWarning;
import java.sql.Savepoint;
import java.sql.Statement;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.sql.DataSource;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class PoolMan {
  public static int POOL_SIZE = 256;

  private static final String CONFIG = "/poolman.xml";
  private static final String DS_TAG = "datasource";
  private static final String DBNAME_TAG = "dbname";
  private static final String DRIVER_TAG = "driver";
  private static final String URL_TAG = "url";
  private static final String USER_TAG = "username";
  private static final String PASSWD_TAG = "password";

  private static HashMap<String, _ds> dsmap = null;

  private static String _tmp_name = null;
  ;
  private static String _tmp_driver = null;
  private static String _tmp_url = null;
  private static String _tmp_user = null;
  private static String _tmp_pwd = null;

  private static interface _foo { void foo(Object p0); }

  private static HashMap<String, _foo> _foo_map = new HashMap<String, _foo>(4);
  static {
    _foo_map.put(DS_TAG, new _foo() {
      public void foo(Object p0) {
        _ds ds = new _ds();
        ds.driver = _tmp_driver;
        ds.name = _tmp_name;
        ds.url = _tmp_url;
        ds.user = _tmp_user;
        ds.passwd = _tmp_pwd;

        dsmap.put(ds.name, ds);

        _tmp_name = null;
        ;
        _tmp_driver = null;
        _tmp_url = null;
        _tmp_user = null;
        _tmp_pwd = null;
      }
    });

    _foo_map.put(DRIVER_TAG, new _foo() {
      public void foo(Object p0) { _tmp_driver = (String)p0; }
    });

    _foo_map.put(DBNAME_TAG, new _foo() {
      public void foo(Object p0) { _tmp_name = (String)p0; }
    });

    _foo_map.put(URL_TAG, new _foo() {
      public void foo(Object p0) { _tmp_url = (String)p0; }
    });

    _foo_map.put(USER_TAG, new _foo() {
      public void foo(Object p0) { _tmp_user = (String)p0; }
    });

    _foo_map.put(PASSWD_TAG, new _foo() {
      public void foo(Object p0) { _tmp_pwd = (String)p0; }
    });
  }

  private static final class PoolManSAXHandler extends DefaultHandler {
    private StringBuffer sb = new StringBuffer();

    @Override
    public void characters(char[] ch, int start, int length)
        throws SAXException {
      sb.append(ch, start, length);
    }

    @Override
    public void endElement(String uri, String lname, String qname)
        throws SAXException {
      _foo f = _foo_map.get(qname);
      if (f != null) {
        f.foo(sb.toString().trim());
      }
      sb.setLength(0);
    }
  }

  private static void loadconfSAX(String xml) {
    try {
      InputStream is = PoolMan.class.getResourceAsStream(xml);
      SAXParser p = SAXParserFactory.newInstance().newSAXParser();
      p.parse(is, new PoolManSAXHandler());
    } catch (ParserConfigurationException ex) {
      ex.printStackTrace();
    } catch (SAXException ex) {
      ex.printStackTrace();
    } catch (IOException ex) {
      ex.printStackTrace();
    }
  }

  public static void shutdown() {
    Iterator<_ds> i = dsmap.values().iterator();
    while (i.hasNext()) {
      shutdownDS(i.next());
    }
  }
  public static void shutdownDS(DataSource __1) {
    _ds ds = null;
    if (__1 instanceof _ds)
      ds = (_ds)__1;

    if (ds != null) {
      for (int i = 0; i < ds.pool.length; i++)
        if (ds.pool[i] != null && ds.pool[i].real != null)
          try {
            ds.pool[i].real.close();
            ds.pool[i].real = null;
            ds.pool[i] = null;
          } catch (Throwable ex) {
            ex.printStackTrace();
          }
      ds.pool = null;
      dsmap.put(ds.name, null);
    }
  }

  public static DataSource findDataSource(String dsname) {
    if (dsmap == null) {
      dsmap = new HashMap<String, _ds>(4);
      loadconfSAX(CONFIG);
    }

    _ds retVal = (_ds)dsmap.get(dsname);
    try {
      Class.forName(retVal.driver);
    } catch (ClassNotFoundException ex) {
      ex.printStackTrace();
      retVal = null;
    }
    return retVal;
  }

  public static int highWatermark(DataSource _1) {
    if (!(_1 instanceof _ds))
      return -1;
    _ds ds = (_ds)_1;

    for (int i = ds.pool.length; --i > 0;) {
      if (ds.pool[i] != null)
        return i + 1;
    }
    return 0;
  }

  private static class _ds implements DataSource {
    private String name;
    private String driver;
    private String url;
    private String user;
    private String passwd;

    private _connection[] pool = new _connection[PoolMan.POOL_SIZE];

    private _connection _get_conn(int fromIndex, int toIndex)
        throws SQLException {
      for (int i = fromIndex; i < toIndex; i++) {
        _connection c = pool[i];
        if (c == null) {
          c = new _connection();
          c.ds = this;
          c.index = i;
          if (user != null && passwd != null)
            c.real = DriverManager.getConnection(url, user, passwd);
          else
            c.real = DriverManager.getConnection(url);
          pool[i] = c;
          c.setAutoCommit(true);
          return c;
        }
        if (c.index == -1) {
          c.index = i;
          c.setAutoCommit(true);
          return c;
        }
      }
      return null;
    }

    synchronized public Connection getConnection() throws SQLException {
      _connection c = _get_conn(0, pool.length);
      if (c == null)
        throw new SQLException("no connections available");
      return c;
    }

    synchronized public void closeConnection(_connection c) {
      if (c.index != -1)
        c.index = -1;
    }

    public Connection getConnection(String __user, String __passwd)
        throws SQLException {
      throw new SQLException("unsupported");
    }

    public int getLoginTimeout() throws SQLException { return 0; }

    public PrintWriter getLogWriter() throws SQLException { return null; }

    public void setLoginTimeout(int seconds) throws SQLException {}

    public void setLogWriter(PrintWriter out) throws SQLException {}
  }

  private static class _connection implements Connection, Transactional {
    private _ds ds = null;
    private Connection real = null;
    private int index = -1;
    private boolean isTr = false;

    public void clearWarnings() throws SQLException { real.clearWarnings(); }

    public void close() throws SQLException {
      if (isTr)
        commit();
      ds.closeConnection(this);
    }

    // first 'begin' starts transaction
    public void begin() throws SQLException {
      if (!isTr) {
        real.setAutoCommit(false);
        isTr = true;
      }
    }

    // first 'commit/rollback' ends transaction
    public void commit() throws SQLException {
      if (isTr) {
        isTr = false;
        real.commit();
      }
    }

    public Statement createStatement() throws SQLException {
      return real.createStatement();
    }

    public Statement createStatement(int resultSetType,
                                     int resultSetConcurrency,
                                     int resultSetHoldability)
        throws SQLException {
      return real.createStatement(resultSetType, resultSetConcurrency,
                                  resultSetHoldability);
    }

    public Statement createStatement(int resultSetType,
                                     int resultSetConcurrency)
        throws SQLException {
      return real.createStatement(resultSetType, resultSetConcurrency);
    }

    public boolean getAutoCommit() throws SQLException {
      return real.getAutoCommit();
    }

    public String getCatalog() throws SQLException { return real.getCatalog(); }

    public int getHoldability() throws SQLException {
      return real.getHoldability();
    }

    public DatabaseMetaData getMetaData() throws SQLException {
      return real.getMetaData();
    }

    public int getTransactionIsolation() throws SQLException {
      return real.getTransactionIsolation();
    }

    public Map getTypeMap() throws SQLException { return real.getTypeMap(); }

    public SQLWarning getWarnings() throws SQLException {
      return real.getWarnings();
    }

    public boolean isClosed() throws SQLException {
      return index == -1 || real.isClosed();
    }

    public boolean isReadOnly() throws SQLException {
      return real.isReadOnly();
    }

    public String nativeSQL(String sql) throws SQLException {
      return real.nativeSQL(sql);
    }

    public CallableStatement prepareCall(String sql, int resultSetType,
                                         int resultSetConcurrency,
                                         int resultSetHoldability)
        throws SQLException {
      return real.prepareCall(sql, resultSetType, resultSetConcurrency,
                              resultSetHoldability);
    }

    public CallableStatement prepareCall(String sql, int resultSetType,
                                         int resultSetConcurrency)
        throws SQLException {
      return real.prepareCall(sql, resultSetType, resultSetConcurrency);
    }

    public CallableStatement prepareCall(String sql) throws SQLException {
      return real.prepareCall(sql);
    }

    public PreparedStatement prepareStatement(String sql, int resultSetType,
                                              int resultSetConcurrency,
                                              int resultSetHoldability)
        throws SQLException {
      return real.prepareStatement(sql, resultSetType, resultSetConcurrency,
                                   resultSetHoldability);
    }

    public PreparedStatement prepareStatement(String sql, int resultSetType,
                                              int resultSetConcurrency)
        throws SQLException {
      return real.prepareStatement(sql, resultSetType, resultSetConcurrency);
    }

    public PreparedStatement prepareStatement(String sql, int autoGeneratedKeys)
        throws SQLException {
      return real.prepareStatement(sql, autoGeneratedKeys);
    }

    public PreparedStatement prepareStatement(String sql, int[] columnIndexes)
        throws SQLException {
      return real.prepareStatement(sql, columnIndexes);
    }

    public PreparedStatement prepareStatement(String sql, String[] columnNames)
        throws SQLException {
      return real.prepareStatement(sql, columnNames);
    }

    public PreparedStatement prepareStatement(String sql) throws SQLException {
      return real.prepareStatement(sql);
    }

    public void releaseSavepoint(Savepoint savepoint) throws SQLException {
      real.releaseSavepoint(savepoint);
    }

    public void rollback() throws SQLException {
      if (isTr) {
        isTr = false;
        real.rollback();
      }
    }

    public void rollback(Savepoint savepoint) throws SQLException {
      real.rollback(savepoint);
    }

    public void setAutoCommit(boolean autoCommit) throws SQLException {
      real.setAutoCommit(autoCommit);
    }

    public void setCatalog(String catalog) throws SQLException {
      real.setCatalog(catalog);
    }

    public void setHoldability(int holdability) throws SQLException {
      real.setHoldability(holdability);
    }

    public void setReadOnly(boolean readOnly) throws SQLException {
      real.setReadOnly(readOnly);
    }

    public Savepoint setSavepoint() throws SQLException {
      return real.setSavepoint();
    }

    public Savepoint setSavepoint(String name) throws SQLException {
      return real.setSavepoint(name);
    }

    public void setTransactionIsolation(int level) throws SQLException {
      real.setTransactionIsolation(level);
    }

    public void setTypeMap(Map map) throws SQLException {
      real.setTypeMap(map);
    }

    public String toString() {
      StringBuffer buf = new StringBuffer();
      buf.append("\nurl = ");
      buf.append(ds.url);
      buf.append("\nindex = ");
      buf.append(index);
      buf.append("\n");
      buf.append(real.toString());
      return buf.toString();
    }
  }
}
