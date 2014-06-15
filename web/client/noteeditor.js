var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var Collapsable       = require('./ui/collapsable').Collapsable;
var NewNoteForm       = require('./ui/newnoteform').NewNoteForm;
var NoteEdit          = require('./ui/noteedit').NoteEdit;
var NoteList          = require('./ui/notelist').NoteList;
var PublishedNoteList = require('./ui/publishednotelist').PublishedNoteList;

exports.NoteEditor = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.NoteEditor.prototype.element = function() {
    return this._element;
}

exports.NoteEditor.prototype._template = function() {
    this._element = div();

    var publishedNoteListCollapsable = new Collapsable('Published Notes', null);
    this._element(publishedNoteListCollapsable.element());

    var publishedNoteList = new PublishedNoteList();
    publishedNoteListCollapsable.setContent(publishedNoteList.element());

    var noteListCollapsable = new Collapsable('Notes', null);
    this._element(noteListCollapsable.element());

    var noteList = new NoteList(
            function(noteId) {
                // draft phase
                noteEdit.setNote(noteId, 0);
            }
            );
    noteListCollapsable.setContent(noteList.element());

    var noteEditCollapsable = new Collapsable('Editor', null);
    this._element(noteEditCollapsable.element());
    var noteEdit = new NoteEdit();
    noteEditCollapsable.setContent(noteEdit.element());
    noteEditCollapsable.hideButton();

    var newNoteFormCollapsable = new Collapsable('New Note', null);
    this._element(newNoteFormCollapsable.element());
    var newNoteForm = new NewNoteForm(
            function(note) {
                api.noteList(
                    function(err, notes) {
                        if(err)
                            console.log('new note: ' + err);
                        else
                            noteList.reload();
                    }
                    );
            }
            );
    newNoteFormCollapsable.setContent(newNoteForm.element());
}

