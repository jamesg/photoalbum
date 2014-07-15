var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var AlbumForm      = require('./ui/photograph/album/form').AlbumForm;
var AlbumList      = require('./ui/photograph/album/list').AlbumList;
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

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    photographList.setList([]);

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

    this._element(
            div(
                { class: 'padded pure-g' },
                div(
                    { class: 'padded content-view pure-u-1-1' },
                    h2('Albums'),
                    albumList.element()
                   ),
                div(
                    { class: 'pure-u-1-1' },
                    photographList.element()
                   ),
                div(
                    { class: 'padded content-view pure-u-1-1' },
                    h2('New Album'),
                    newAlbumForm.element()
                   )
               )
            );
}

