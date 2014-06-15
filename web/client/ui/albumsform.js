var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

var ConfirmButton = require('./confirmbutton').ConfirmButton;

exports.AlbumsForm = function(photographId) {
    this._photographId = photographId;
    this._documentFragment = domjs.build(this._template.bind(this));
    this._rebuild();
}

exports.AlbumsForm.prototype.element = function() {
    return this._element;
}

exports.AlbumsForm.prototype._rebuild = function() {
    api.albumList((function(err, albums) {
        api.photographAlbums(this._photographId, (function(err, photographAlbums) {
            domjs.build(
                this._formTemplate.bind(this, albums, photographAlbums)
                );
        }).bind(this)
        )
    }).bind(this));
}

exports.AlbumsForm.prototype._template = function() {
    this._element = div();
}

exports.AlbumsForm.prototype._liTemplate = function(album) {
    return div(
        { class: 'pure-u-1-1' },
        album.name,
        (new ConfirmButton(
                   'Remove',
                   (function(albumId) {
                       api.removePhotographFromAlbum(
                           this._photographId,
                           albumId,
                           this._rebuild.bind(this)
                           );
                   }).bind(this, album.album_id)
            )
            ).element()
        );
}

exports.AlbumsForm.prototype._listTemplate = function(albums) {
    var _ul = div({ class: 'pure-g' });
    for(album in albums)
        _ul(this._liTemplate(albums[album]));
    return _ul;
}

exports.AlbumsForm.prototype._selectTemplate = function(albumList) {
    var _select = select();
    for(albumI in albumList)
        _select(
            option(
                { value: albumList[albumI].album_id },
                albumList[albumI].name
                )
            );
    return _select;
}

exports.AlbumsForm.prototype._formTemplate = function(albums, photographAlbums) {
    var _select = this._selectTemplate(albums);
    var submit = function() {
        api.addPhotographToAlbum(
                this._photographId,
                _select().value,
                function() {}
                );
        this._rebuild();
        return false;
    };
    this._element().innerHTML = '';
    this._element(
            div(
                h3(
                    'Albums ',
                    small('Add the photograph to appropriate albums')
                    ),
                this._listTemplate(photographAlbums),
                form(
                    {
                        class: 'pure-form',
                        onsubmit: submit.bind(this)
                    },
                    _select,
                    input(
                        {
                            class: 'pure-button pure-button-primary',
                            type: 'submit',
                            value: 'Add'
                        }
                        )
                    )
                )
        );
}

