var _ = require('underscore');
var domjs = require('domjs/lib/html5')(document);

var api = require('../../api');

var confirmButton = require('./../confirmbutton').confirmButton;
var iconTemplate = require('../icon').icon;

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
    this._element(
            thead(
                tr(
                    td('Date'),
                    td('Title'),
                    td({ colspan: 3 }, 'Action')
                    )
                )
            );

    var deleteFunction = function(noteList, noteId) {
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

    this._element( tbody.apply( this, _.map( notes,
        (function(n) {
            return tr(
                td(n.created),
                td(n.title),
                td(
                    confirmButton(
                        function() { return span(iconTemplate('delete'), 'Delete'); },
                        _.partial(deleteFunction, this, n.note_id)
                        )
                  ),
                td(
                    button(
                        {
                            class: 'pure-button',
                            onclick: _.partial(this._callback, n.note_id)
                        },
                        iconTemplate('text'),
                        'Edit'
                        )
                  ),
                td(
                    confirmButton(
                        'Publish',
                        _.partial(
                            function(noteId) {
                            api.publishNote(
                                noteId,
                                function(err) {
                                    if(err)
                                        console.log('publishing note: ' + err);
                                }
                                )
                            },
                            n.note_id
                            )
                        )
                    )
                );
        }).bind(this)
        )));
}

exports.NoteList.prototype._template = function() {
    this._element = table({ 'class': 'notetable' });
    this._element();
}

