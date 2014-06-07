var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

/*
 * Albums is a list of the form:
 * [
 *  {
 *   'album_id': id
 *   'name': name
 *  }
 * ]
 *
 * Callback is called with the id of the album selected.
 */
exports.AlbumList = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.AlbumList.prototype.setAlbumList = function(albums) {
    this._element().innerHTML = '';
    this._element(
            domjs.build(
                this._listTemplate.bind(this, albums)
                )
            );
}

exports.AlbumList.prototype.element = function() {
    return this._element;
}

exports.AlbumList.prototype._listTemplate = function(albums) {
    for(a in albums) {
        var _li = li(
                {
                    onclick: this._callback.bind(this, albums[a].album_id)
                },
                albums[a].name
                );
        this._element(_li);
    }
}

/*
 * Template for the album list.
 */
exports.AlbumList.prototype._template = function() {
    this._element = ul({ 'class': 'albumlist' });
    this._element();
}

