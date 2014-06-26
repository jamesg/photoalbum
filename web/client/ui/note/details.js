var domjs = require('domjs/lib/html5')(document);

var api = require('../../api');

exports.NoteDetails = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.NoteDetails.prototype.element = function() {
    return this._element;
}

exports.NoteDetails.prototype._template = function() {
    this._element = div();
    this._detailTemplate();
}

exports.NoteDetails.prototype._detailTemplate = function() {
    this._element(p('Title: ' + this._note.title));
}


