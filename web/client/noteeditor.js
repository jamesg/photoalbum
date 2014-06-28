var domjs = require('domjs/lib/html5')(document);

var api  = require('./api');
var util = require('./util');

var icon              = require('./ui/icon').icon;
var ConfirmButton     = require('./ui/confirmbutton').ConfirmButton;
var NewNoteForm       = require('./ui/note/newnoteform').NewNoteForm;
var NoteEdit          = require('./ui/note/edit').NoteEdit;
var NoteList          = require('./ui/note/list').NoteList;
var PublishedNoteList = require('./ui/note/publishedlist').PublishedNoteList;

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

    this._element(
            div(
                { class: 'mainmenu pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2(icon('book'), 'Published Notes'),
                    (new PublishedNoteList()).element()
                    )
               )
            );

    var editorNoteId;

    var noteList = new NoteList(
            function(noteId) {
                editorNoteId = noteId;
                // draft phase
                noteEdit.setNote(editorNoteId, 0);
            }
            );
    var noteEdit = new NoteEdit();

    this._element(
            div(
                { class: 'mainmenu pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2(icon('text'), 'Draft Notes'),
                    noteList.element()
                    )
               )
            );

    this._element(noteEdit.element());

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
    this._element(
            div(
                { class: 'mainmenu pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2(icon('pencil'), 'Create Note'),
                    newNoteForm.element()
                   )
               )
            );

}

