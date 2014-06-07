-- Create the map database.  This database stores map information and images
-- from the Ordnance Survey Open Data archive.  When the database is used from
-- the photograph album server it is read only.

-- Street Map tile data.
CREATE TABLE IF NOT EXISTS tile_data (
    region    VARCHAR,
    eastings  INTEGER,
    northings INTEGER,
    data      BLOB NOT NULL, -- PNG data
    PRIMARY KEY(region, eastings, northings)
);

CREATE VIEW IF NOT EXISTS tile AS
SELECT region, eastings, northings FROM tile_data;

-- Data for the OS 50k Gazetteer data set.  Field names are identical to those
-- in the documentation for the data set.
CREATE TABLE IF NOT EXISTS gazetteer (
    seq         INTEGER,
    km_ref      VARCHAR,
    def_nam     VARCHAR,
    tile_ref    VARCHAR,
    lat_deg     INTEGER,
    lat_min     FLOAT,
    long_deg    INTEGER,
    long_min    FLOAT,
    north       INTEGER,
    east        INTEGER,
    gmt         VARCHAR,
    co_code     VARCHAR,
    county      VARCHAR,
    full_county VARCHAR,
    f_code      VARCHAR,
    e_date      VARCHAR,
    update_co   VARCHAR,
    sheet_1     INTEGER,
    sheet_2     INTEGER,
    sheet_3     INTEGER
);
