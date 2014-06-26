var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var AlbumForm      = require('./ui/photograph/album/form').AlbumForm;
var AlbumList      = require('./ui/photograph/album/list').AlbumList;
var Collapsable    = require('./ui/collapsable').Collapsable;
var PhotographList = require('./ui/photograph/list').PhotographList;

exports.Albums = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Albums.prototype.element = function() {
    return this._element;
}

exports.Albums.prototype._template = function() {
    this._element = div();

    // Create the album list.
    var albumListCollapsable = new Collapsable('Albums', null);
    this._element(albumListCollapsable.element());

    var albumList = new AlbumList(
        function(albumId) {
            api.photographsInAlbum(
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

    // Create the new album form.
    var newAlbumForm = new AlbumForm(
        function(err) {
            if(err)
            {
                console.log('error creating album: ' + err);
                return;
            }
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

