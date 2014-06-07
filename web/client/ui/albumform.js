var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

/*
 * New album form.
 */
exports.AlbumForm = function(callback) {
    domjs.build(this._template.bind(this, callback));
}

exports.AlbumForm.prototype.element = function() {
    return this._element();
}

/*
 * Template for the album form.  The callback function is called with an object
 * of the form:
 *
 * {
 *  'title': title,
 *  'caption': caption
 * }
 */
exports.AlbumForm.prototype._template = function(callback) {
    var titleInput    = input({ type: 'text', name: 'title' });
    var captionInput  = input({ type: 'text', name: 'caption' });

    this._element = form(
        {
            onsubmit: function() {
                try {
                    api.insertAlbum(
                        {
                            'name':    titleInput().value,
                            'caption': captionInput().value
                        },
                        callback
                        );
                } catch (e) {
                    console.log('while creating album: ' + e);
                }
                return false;
            }
        },
        table(
            tr(td('Title'),   titleInput),
            tr(td('Caption'), captionInput),
            tr(td(), input({ type: 'submit', value: 'Create' }))
            )
        );
}

