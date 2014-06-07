var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

// Ui
var AlbumForm         = require('./ui/albumform').AlbumForm;
var AlbumList         = require('./ui/albumlist').AlbumList;
var Collapsable       = require('./ui/collapsable').Collapsable;
var DateRangeSelector = require('./ui/daterangeselector').DateRangeSelector;
var LocationList      = require('./ui/locationlist').LocationList;
var PhotographFilter  = require('./ui/photographfilter').PhotographFilter;
var PhotographForm    = require('./ui/photographform').PhotographForm;
var PhotographList    = require('./ui/photographlist').PhotographList;
var Statistics        = require('./ui/statistics').Statistics;
var TagList           = require('./ui/taglist').TagList;

exports.PhotoAlbum = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.PhotoAlbum.prototype.element = function() {
    return this._element;
}

exports.PhotoAlbum.prototype._template = function() {
    this._element = div();

    var statisticsCollapsable = new Collapsable('Statistics', null);
    this._element(statisticsCollapsable.element());
    var statistics = new Statistics();
    statisticsCollapsable.setContent(statistics.element());

    // Create the album list.
    var albumListCollapsable = new Collapsable('Albums', null);
    this._element(albumListCollapsable.element());

    var albumList = new AlbumList(
        function(albumId) {
            api.photographList(
                albumId,
                function(err, list) {
                    photographList.setList(list);
                }
                );
            photographList
        }
        );
    api.albumList(
        function(err, albums) {
            albumList.setAlbumList(albums);
        }
        );
    albumListCollapsable.setContent(albumList.element());

    // Create the photograph filter (list of special albums).
    var photographFilterCollapsable = new Collapsable('Filters', null);
    this._element(photographFilterCollapsable.element());
    var photographFilter = new PhotographFilter(
            function(apiFunction, params) {
                api.apiRequest(
                    apiFunction, params,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    photographFilterCollapsable.setContent(photographFilter.element());

    // Create the location list.
    var locationListCollapsable = new Collapsable('Locations', null);
    var locationList = new LocationList(
            function(location) {
                api.photographsWithLocation(
                    location,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    api.locations(
            function(err, locations) {
                locationList.setLocationList( util.values(
                        util.mapValues(
                            locations,
                            function(location) { return location.location; }
                            )
                        ) );
            }
            );
    locationListCollapsable.setContent(locationList.element());
    this._element(locationListCollapsable.element());

    // Create the tag list.
    var tagListCollapsable = new Collapsable('Tags', null);
    var tagList = new TagList(
            function(tag) {
                api.photographsTagged(
                    tag,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    api.tags(
            function(err, tags) {
                tagList.setTagList( util.values(
                        util.mapValues(tags,
                            function(tag) { return tag.tag; }
                            )
                        ) );
            }
            );
    tagListCollapsable.setContent(tagList.element());
    this._element(tagListCollapsable.element());

    // Create the photograph list.
    var photographListCollapsable = new Collapsable('Photographs', null);
    this._element(photographListCollapsable.element());

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    photographList.setList([]);
    photographListCollapsable.setContent(photographList.element());

    // Create the new photograph form.
    var newPhotographForm = new PhotographForm();
    var photographFormCollapsable = new Collapsable(
        'New Photograph',
        newPhotographForm.element()
        );
    this._element(photographFormCollapsable.element());

    // Create the new album form.
    var newAlbumForm = new AlbumForm(
        function() {
            api.albumList(
                function(err, albums) {
                    albumList.setAlbumList(albums);
                }
                );
        }
        );
    var albumFormCollapsable =
        new Collapsable('New Album', newAlbumForm.element());
    this._element(albumFormCollapsable.element());
}
