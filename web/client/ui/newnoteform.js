var domjs = require('domjs/lib/html5')(document);

var api   = require('../api');
var util  = require('../util');

/*
 * New note form.  Creates a note and a draft version for the note.
 *
 * Callback is called with the ids of newly created database objects (note_id,
 * note_version_id and markdown_id).
 */
exports.NewNoteForm = function(callback) {
    domjs.build(this._template.bind(this));
    this._callback = callback;
}

exports.NewNoteForm.prototype.element = function() {
    return this._element();
}

exports.NewNoteForm.prototype._template = function() {
    var titleInput = input({ 'type': 'text', 'name': 'title' })
    var message = p();
    var _form = form(
            {
                onsubmit: (function() {
                    api.createDraftNote(
                        titleInput().value,
                        (function(err, note) {
                            if(err)
                                message(err);
                            else
                            {
                                message('Draft Created');
                                this._callback(note);
                            }
                        }).bind(this)
                        );
                    return false;
                }).bind(this)
            },
            titleInput,
            input({ 'type': 'submit', 'value': 'Create Draft' })
            );
    _form();
    this._element = _form;
}

