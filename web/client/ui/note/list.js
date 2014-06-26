var domjs = require('domjs/lib/html5')(document);

var api = require('../../api');

var ConfirmButton = require('./../confirmbutton').ConfirmButton;

/*
 * Notes is a list of the form:
 *
 * [
 *  {
 *   'note_id': note id
 *   'title': note title
 *   'created': creation date
 *  }
 * ]
 *
 * Callback is called with the id of the note selected.
 */
exports.NoteList = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );

    this.reload();
}

exports.NoteList.prototype.reload = function() {
    api.noteList(
            (function(err, notes) {
                if(err)
                    console.log('fetching note list: ' + e);
                else
                    this._setNoteList(notes);
            }).bind(this)
            );
}

exports.NoteList.prototype._setNoteList = function(notes) {
    this._element().innerHTML = '';
    this._element(
            {
                class: 'pure-table'
            },
            domjs.build(
                this._listTemplate.bind(this, notes)
                )
            );
}

exports.NoteList.prototype.element = function() {
    return this._element;
}

exports.NoteList.prototype._listTemplate = function(notes) {
    this._element(thead(tr(td('Date'), td('Title'), td('Action'))));
    var _tbody = tbody();
    this._element(_tbody);
    for(n in notes) {
        var deleteFunction = function(noteList, noteId) {
            var noteList = noteList, noteId = noteId;
            return function() {
                api.deleteNote(
                    noteId,
                    function(err) {
                        if(err)
                            console.log('deleting note: ' + e);
                        else
                            noteList.reload();
                    }
                    );
            }
        }
        var _tr = tr(
                td(notes[n].created),
                td(notes[n].title),
                td(
                    (new ConfirmButton(
                        'Delete',
                        deleteFunction(this, notes[n].note_id)
                        )).element(),
                    button(
                        {
                            class: 'pure-button',
                            onclick: this._callback.bind(this, notes[n].note_id)
                        },
                        'Edit'
                        ),
                    (new ConfirmButton(
                        'Publish',
                        (function(noteId) {
                            api.publishNote(
                                noteId,
                                function(err) {
                                    if(err)
                                        console.log('publishing note: ' + err);
                                }
                                )
                        }).bind(this, notes[n].note_id)
                        )).element()
                    )
                );
        _tbody(_tr);
    }
}

exports.NoteList.prototype._template = function() {
    this._element = table({ 'class': 'notetable' });
    this._element();
}

