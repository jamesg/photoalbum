var domjs = require('domjs/lib/html5')(document);

var api = require('../../api');

var icon          = require('../icon').icon;
var confirmButton = require('../confirmbutton').confirmButton;
var MessageBox    = require('../messagebox').MessageBox;

exports.NoteEdit = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.NoteEdit.prototype.setNote = function(noteId, phase) {
    this._noteId = noteId;
    this._phase = phase;

    api.markdownData(
        noteId,
        phase,
        (function(err, markdownData) {
            if(err) {
                console.log(err);
                return;
            }
            this._markdownId = markdownData.markdown_id;
            api.note(
                noteId,
                (function(err, note) {
                    this.displayMarkdown(note.title, markdownData);
                }).bind(this)
                )
        }).bind(this)
        );
}

exports.NoteEdit.prototype.displayMarkdown = function(title, markdownData) {
    console.log('display markdown: ' + markdownData.markdown);
    this._markdownId = markdownData.markdown_id;
    this._element().innerHTML = '';
    this._element(
            domjs.build(
                this._editorTemplate.bind(this, title, markdownData.markdown)
                )
            );
}

exports.NoteEdit.prototype.element = function() {
    return this._element;
}

exports.NoteEdit.prototype._template = function() {
    this._element = div({ class: 'mainmenu pure-g' });

    this._element(this._loadingTemplate());
}

exports.NoteEdit.prototype._loadingTemplate = function() {
    div(
        { class: 'pure-u-1-1' },
        p('Loading')
       );
}

exports.NoteEdit.prototype._editorTemplate = function(title, markdown) {
    var markdownTextarea = textarea(
            {
                name: 'markdown',
                rows: 16,
                cols: 80,
                class: 'note-textarea'
            },
            markdown
            );

    messageBox = new MessageBox();

    div(
        { class: 'pure-u-18-24' },
        h2(icon('pencil'), 'Edit Note'),
        messageBox.element(),
        form(
            {
                class: 'pure-form',
                onsubmit: (function() {
                    api.updateMarkdownData(
                        {
                            'markdown_id': this._markdownId,
                            'markdown': markdownTextarea().value
                        },
                        function(err) {
                            if(err)
                                messageBox.displayError('Updating note: ' + err);
                            else
                                messageBox.displaySuccess('Note updated');
                        }
                        );
                    return false;
                }).bind(this)
            },
            p('Title: ' + title),
            markdownTextarea,
            br(),
            input(
                {
                    class: 'pure-button pure-button-primary',
                    type: 'submit',
                    value: 'Save'
                }
                ),
            confirmButton(
                    'Save & Publish',
                    function() {
                        console.log('save');
                    }
                    )
            )
        );
        div(
            { class: 'pure-u-6-24' },
            h2(icon('check'), 'Publish Note'),
            confirmButton(
                span(icon('check'), 'Publish'),
                function() {
                    if(editorNoteId)
                        api.publishNote(
                            editorNoteId,
                            function(err) {
                                if(err)
                                    console.log('publishing note: ' + err);
                            }
                            );
                }
                )
           );
}

