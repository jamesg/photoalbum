var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var Collapsable       = require('./ui/collapsable').Collapsable;
var Note              = require('./ui/note').Note;
var PublishedNoteList = require('./ui/publishednotelist').PublishedNoteList;

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

    var publishedNoteListCollapsable = new Collapsable('Published Notes', null);
    this._element(publishedNoteListCollapsable.element());
    var publishedNoteList = new PublishedNoteList(
            (function(noteId) {
                note.setNote(noteId, 1); // 1 = published note
            }).bind(this)
            );
    publishedNoteListCollapsable.setContent(publishedNoteList.element());

    var noteCollapsable = new Collapsable('Note', null);
    noteCollapsable.hideButton();
    this._element(noteCollapsable.element());
    var note = new Note();
    noteCollapsable.setContent(note.element());
}

