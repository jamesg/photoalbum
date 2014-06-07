CREATE TABLE IF NOT EXISTS  photograph          (
    photograph_id           INTEGER PRIMARY KEY AUTOINCREMENT,
    title                   VARCHAR NOT NULL,
    caption                 VARCHAR NULL,
    filename                VARCHAR NOT NULL,
    location                VARCHAR NULL,
    taken                   VARCHAR NULL,
    UNIQUE(photograph_id, title, caption, filename, location, taken)
    );

CREATE TABLE IF NOT EXISTS  jpeg_data           (
    photograph_id           INTEGER PRIMARY KEY,
    data                    BLOB NOT NULL,
    FOREIGN KEY( photograph_id ) REFERENCES photograph(photograph_id)
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    UNIQUE(photograph_id, data)
    );

CREATE TABLE IF NOT EXISTS  album               (
    album_id                INTEGER PRIMARY KEY AUTOINCREMENT,
    name                    VARCHAR NOT NULL,
    caption                 VARCHAR NULL,
    fromdate                VARCHAR NULL,
    todate                  VARCHAR NULL,
    UNIQUE(album_id, name, caption, fromdate, todate)
    );

CREATE TABLE IF NOT EXISTS  photograph_tagged    (
    photograph_id           INTEGER NOT NULL,
    tag                     VARCHAR NOT NULL,
    FOREIGN KEY( photograph_id ) REFERENCES photograph( photograph_id )
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    UNIQUE(photograph_id, tag)
    );

CREATE TABLE IF NOT EXISTS  photograph_in_album   (
    photograph_id           INTEGER NOT NULL,
    album_id                INTEGER NOT NULL,
    FOREIGN KEY( photograph_id ) REFERENCES photograph( photograph_id )
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    FOREIGN KEY( album_id )      REFERENCES album( album_id )
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    UNIQUE(photograph_id, album_id)
    );

CREATE TABLE IF NOT EXISTS note (
    note_id                 INTEGER PRIMARY KEY AUTOINCREMENT,
    created                 VARCHAR NOT NULL,
    title                   INTEGER NOT NULL
    );

CREATE TABLE IF NOT EXISTS note_version (
    note_version_id         INTEGER PRIMARY KEY AUTOINCREMENT,
    note_id                 INTEGER NOT NULL,
    markdown_id             INTEGER NOT NULL,
    phase                   INTEGER NOT NULL,
    modified                INTEGER NOT NULL,
    FOREIGN KEY(note_id) REFERENCES note(note_id)
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    FOREIGN KEY(markdown_id) REFERENCES markdown(markdown_id)
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    UNIQUE(note_id, phase)
    );

CREATE TRIGGER IF NOT EXISTS note_version_update_modified
AFTER UPDATE ON note_version
FOR EACH ROW
    BEGIN
        UPDATE note_version
        SET modified = '2014-04-16'
        WHERE note_version_id = NEW.note_version_id;
    END;

CREATE TRIGGER IF NOT EXISTS note_version_delete_markdown
AFTER DELETE ON note_version
FOR EACH ROW
    BEGIN
        DELETE FROM markdown
        WHERE markdown_id = OLD.markdown_id;
    END;

CREATE TABLE IF NOT EXISTS note_version_uses_photograph (
    note_version_id         INTEGER NOT NULL,
    photograph_id           INTEGER NOT NULL,
    filename                VARCHAR NOT NULL,
    FOREIGN KEY(note_version_id) REFERENCES note_version(note_version_id)
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id)
    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED
    );

CREATE TABLE IF NOT EXISTS markdown (
    markdown_id             INTEGER PRIMARY KEY AUTOINCREMENT,
    data                    VARCHAR NOT NULL DEFAULT ''
    );

CREATE VIEW draft_note AS
    SELECT note_id, created, modified, data
    FROM note NATURAL JOIN note_version NATURAL JOIN markdown
    WHERE phase = 0;

CREATE VIEW published_note AS
    SELECT note_id, created, modified, data
    FROM note NATURAL JOIN note_version NATURAL JOIN markdown
    WHERE phase = 1;

