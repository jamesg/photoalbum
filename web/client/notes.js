var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var Collapsable       = require('./ui/collapsable').Collapsable;
var Note              = require('./ui/note/note').Note;
var PublishedNoteList = require('./ui/note/publishedlist').PublishedNoteList;

exports.Notes = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Notes.prototype.element = function() {
    return this._element;
}

exports.Notes.prototype._template = function() {
    this._element = div();

    var note = new Note();
    var publishedNoteList = new PublishedNoteList(
            (function(noteId) {
                note.setNote(noteId, 1); // 1 = published note
            }).bind(this)
            );

    this._element = div(
        { class: 'pure-g' },
        div(
            { class: 'pure-u-6-24' },
            publishedNoteList.element()
           ),
        div(
            { class: 'pure-u-18-24' },
            note.element()
           )
        );
}

