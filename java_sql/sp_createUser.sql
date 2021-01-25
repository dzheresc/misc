--<ScriptOptions statementTerminator = "@" /> 
CREATE PROCEDURE MED.createUser(
    IN p_name VARCHAR(20), IN p_phrase VARCHAR(80), IN p_displ VARCHAR(80),
    IN actor INTEGER,
    OUT p_uid INTEGER) LANGUAGE SQL BEGIN DECLARE gid INTEGER DEFAULT 0;

  DECLARE SQLSTATE CHAR(5);
  DECLARE not_found CONDITION FOR SQLSTATE '02000';
  DECLARE EXIT HANDLER FOR not_found SIGNAL SQLSTATE '02444';

  --check if user already exists IF
  EXISTS(SELECT *FROM MED.PASSWD WHERE name = p_name) THEN SET p_uid = -1;
    RETURN;
  END IF;

  --generate new user / group ID 
  VALUES NEXT VALUE FOR MED.seq_uid INTO p_uid;

  --group does not have a password
  -- group has no parent(group)
  -- new user has group id set to UID 
  CASE 
    WHEN p_phrase IS NULL THEN SET gid = -1;
    ELSE SET gid = p_uid;
  END CASE;

  --add new user / group 
  INSERT INTO MED.PASSWD VALUES(p_name, p_uid, gid, p_displ, p_phrase);

  --add entry into histroy(0 - add, 1 - remove, 2 - update) 
  INSERT INTO MED.HISTORY VALUES(NEXT VALUE FOR MED.seq_obj, p_uid, gid, 0, CURRENT TIMESTAMP, actor, p_displ);
END @
